/**
 *    Name:        video/unpack.c
 *    Description: unpacking the tile pages and the lists of packed blocks, a step each frame
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    30-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Unpacks the packed data at R10 into R11: flag bits read the highest first, 8 to a byte as needed; 1
   copies a byte; 01 takes a halfword (an offset back of its value >> 3, -1 to -8192, and a length of its
   low three bits + 2, or when they are 0 the next byte + 1; a 0 byte ends the data); 00 takes two bits
   more (a length of 2-5) and a byte (an offset back of -1 to -256); a length copies that many bytes from
   that far back in what has been unpacked. */
void unpack(uint32_t source, uint32_t destination)
{
	uint32_t carry = 0;
	uint8_t flags = 0, length_byte = 0;
	uint32_t copy_count = 0, copy_source = 0, flags_left = 0;
	uint32_t length_bits = 0;

	flags = PACKED_BYTES(source)[0];
	flags_left = 9u;
	source++;
	for (;;)
	{
		flags_left--;
		if (flags_left == 0u)
		{
			flags = PACKED_BYTES(source)[0];
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			UNPACKED_BYTES(destination)[0] = PACKED_BYTES(source)[0];
			source++;
			destination++;
			continue;
		}
		flags_left--;
		if (flags_left == 0u)
		{
			flags = PACKED_BYTES(source)[0];
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			copy_source = 0xFFFF0000u | AS16(PACKED_BYTES(source)[0]);
			length_byte = (uint8_t)copy_source;
			copy_source = (int32_t)copy_source >> 3;
			length_bits = length_byte & 7u;
			copy_count = length_bits;
			source += 2u;
			if (length_bits != 0u)
				copy_count += 2u;
			else
			{
				copy_count = PACKED_BYTES(source)[0];
				source++;
				if (copy_count == 0u)
					break;
				copy_count++;
			}
		}
		else
		{
			copy_count = 0u;
			flags_left--;
			if (flags_left == 0u)
			{
				flags = PACKED_BYTES(source)[0];
				flags_left = 8u;
				source++;
			}
			carry = (uint8_t)(flags * 2u) < flags;
			flags = (uint8_t)(flags * 2u);
			copy_count = copy_count * 2u + carry;
			flags_left--;
			if (flags_left == 0u)
			{
				flags = PACKED_BYTES(source)[0];
				flags_left = 8u;
				source++;
			}
			carry = (uint8_t)(flags * 2u) < flags;
			flags = (uint8_t)(flags * 2u);
			copy_count = copy_count * 2u + carry;
			copy_source = 0xFFFFFF00u | PACKED_BYTES(source)[0];
			source++;
			copy_count += 2u;
		}
		copy_source += destination;
		do
		{
			UNPACKED_BYTES(destination)[0] = UNPACKED_BYTES(copy_source)[0];
			copy_count--;
			copy_source++;
			destination++;
		} while (copy_count != 0u);
	}
}

/* Unpacks a little of the screen's queued tile pages (queue_tile_pages), from the frame loop's wait
   while unpack_state says there is work: with none under way, takes the next request of the ring
   unpack_queue (unpack_read_place; unpack_state 0 when it is empty); else goes on where it stopped, by
   unpack_state's second byte: FF one byte of a match (unpack_copy_source, unpack_copy_count), 1 one step
   of unpack's format (unpack_flags, unpack_flags_left, unpack_source, unpack_destination): a byte, or a
   match's first byte. */
void unpack_queued_step(void)
{
	uint32_t carry = 0;
	uint8_t copy_count = 0, flags = 0, length_byte = 0;
	uint32_t copy_source = 0, destination = 0, flags_left = 0, source = 0, unpack_copy_source = 0, unpack_destination = 0, unpack_read_place = 0;
	struct unpack_request *request = NULL;
	uint8_t unpack_copy_count = 0;
	uint32_t length_bits = 0;

	if ((int8_t)(uint8_t)(screen->unpack_state >> 8u) <= 0)
	{
		if ((uint8_t)(screen->unpack_state >> 8u) != 0u)
		{
			unpack_copy_source = screen->unpack_copy_source;
			unpack_destination = screen->unpack_destination;
			UNPACKED_BYTES(unpack_destination)[0] = UNPACKED_BYTES(unpack_copy_source)[0];
			screen->unpack_copy_source = unpack_copy_source + 1u;
			screen->unpack_destination = unpack_destination + 1u;
			unpack_copy_count = screen->unpack_copy_count;
			screen->unpack_copy_count = unpack_copy_count - 1u;
			if (unpack_copy_count == 1u)
			{
				screen->unpack_state = (screen->unpack_state & 0xFFu) | 0x100u;
				return;
			}
			return;
		}
		unpack_read_place = screen->unpack_read_place;
		if (unpack_read_place == screen->unpack_write_place)
		{
			screen->unpack_state = 0u;
			return;
		}
		request = pointer_to(address_of(screen) + 0xC00u + unpack_read_place * 2u * 4u);
		screen->unpack_read_place = (unpack_read_place + 1u) & 0x3Fu;
		source = request->source;
		destination = request->destination;
		flags = PACKED_BYTES(source)[0];
		flags_left = 9u;
		screen->unpack_state = (screen->unpack_state & 0xFFu) | 0x100u;
		source++;
	}
	else
	{
		flags = screen->unpack_flags;
		flags_left = screen->unpack_flags_left;
		source = screen->unpack_source;
		destination = screen->unpack_destination;
	}
	for (;;)
	{
		flags_left--;
		if (flags_left == 0u)
		{
			flags = PACKED_BYTES(source)[0];
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry)
		{
			UNPACKED_BYTES(destination)[0] = PACKED_BYTES(source)[0];
			screen->unpack_flags = flags;
			screen->unpack_flags_left = flags_left;
			screen->unpack_source = source + 1u;
			screen->unpack_destination = destination + 1u;
			return;
		}
		flags_left--;
		if (flags_left == 0u)
		{
			flags = PACKED_BYTES(source)[0];
			flags_left = 8u;
			source++;
		}
		carry = (uint8_t)(flags * 2u) < flags;
		flags = (uint8_t)(flags * 2u);
		if (carry == 0u)
			break;
		copy_source = 0xFFFF0000u | AS16(PACKED_BYTES(source)[0]);
		length_byte = (uint8_t)copy_source;
		copy_source = (int32_t)copy_source >> 3;
		length_bits = length_byte & 7u;
		copy_count = (uint8_t)length_bits;
		source += 2u;
		if (length_bits != 0u)
		{
			copy_count++;
			copy_source += destination;
			UNPACKED_BYTES(destination)[0] = UNPACKED_BYTES(copy_source)[0];
			screen->unpack_flags = flags;
			screen->unpack_flags_left = flags_left;
			screen->unpack_copy_source = copy_source + 1u;
			screen->unpack_copy_count = copy_count;
			screen->unpack_source = source;
			screen->unpack_destination = destination + 1u;
			screen->unpack_state = (screen->unpack_state & 0xFFu) | 0xFF00u;
			return;
		}
		copy_count = PACKED_BYTES(source)[0];
		source++;
		if (copy_count != 0u)
		{
			copy_source += destination;
			UNPACKED_BYTES(destination)[0] = UNPACKED_BYTES(copy_source)[0];
			screen->unpack_flags = flags;
			screen->unpack_flags_left = flags_left;
			screen->unpack_copy_source = copy_source + 1u;
			screen->unpack_copy_count = copy_count;
			screen->unpack_source = source;
			screen->unpack_destination = destination + 1u;
			screen->unpack_state = (screen->unpack_state & 0xFFu) | 0xFF00u;
			return;
		}
		unpack_read_place = screen->unpack_read_place;
		if (unpack_read_place == screen->unpack_write_place)
		{
			screen->unpack_state = 0u;
			return;
		}
		request = pointer_to(address_of(screen) + 0xC00u + unpack_read_place * 2u * 4u);
		screen->unpack_read_place = (unpack_read_place + 1u) & 0x3Fu;
		source = request->source;
		destination = request->destination;
		flags = PACKED_BYTES(source)[0];
		flags_left = 9u;
		screen->unpack_state = (screen->unpack_state & 0xFFu) | 0x100u;
		source++;
	}
	copy_count = 0u;
	flags_left--;
	if (flags_left == 0u)
	{
		flags = PACKED_BYTES(source)[0];
		flags_left = 8u;
		source++;
	}
	carry = (uint8_t)(flags * 2u) < flags;
	flags = (uint8_t)(flags * 2u);
	copy_count = (uint8_t)(copy_count * 2u + carry);
	flags_left--;
	if (flags_left == 0u)
	{
		flags = PACKED_BYTES(source)[0];
		flags_left = 8u;
		source++;
	}
	carry = (uint8_t)(flags * 2u) < flags;
	flags = (uint8_t)(flags * 2u);
	copy_count = (uint8_t)(copy_count * 2u + carry);
	copy_source = 0xFFFFFF00u | PACKED_BYTES(source)[0];
	source++;
	copy_count++;
	copy_source += destination;
	UNPACKED_BYTES(destination)[0] = UNPACKED_BYTES(copy_source)[0];
	screen->unpack_flags = flags;
	screen->unpack_flags_left = flags_left;
	screen->unpack_copy_source = copy_source + 1u;
	screen->unpack_copy_count = copy_count;
	screen->unpack_source = source;
	screen->unpack_destination = destination + 1u;
	screen->unpack_state = (screen->unpack_state & 0xFFu) | 0xFF00u;
}

/* Unused: unpacks a list of packed blocks at once (unpack): 8-byte entries, the packed data's address
   and where to unpack it, closed by a negative entry (the first is taken without the check).
   unpack_tile_pages does the same for a list of tile pages. */
void unpack_list(uint32_t block_list)
{
	uint32_t destination = 0, source = 0, source_2 = 0;
	uint32_t next_entry = 0;

	do
	{
		source_2 = PACKED_BLOCK_ENTRY(block_list)->source;
		block_list += 4u;
		destination = PACKED_BLOCK_ENTRY(block_list - 0x4u)->destination;
		next_entry = block_list + 4u;
		source = source_2;
		r12 = next_entry;
		unpack(source, destination);
		block_list = r12;
	} while ((int32_t)PACKED_BLOCK_ENTRY(block_list)->source >= 0);
}

/* Unpacks a list of tilemap pages into video RAM at once (unpack): each 32-bit entry a page number in
   its top byte (the page at 300000 + 400 * page) and the address of its packed data below, closed by a
   negative entry; the list format of queue_tile_pages. */
uint32_t unpack_tile_pages(uint32_t page_list, uint32_t list_next)
{
	uint32_t destination = 0, entry_word = 0, page_bits = 0, source = 0;
	uint32_t next_entry = 0;

	while ((int32_t)TILE_PAGE_ENTRIES(page_list)[0] >= 0)
	{
		page_bits = TILE_PAGE_ENTRIES(page_list)[0];
		entry_word = page_bits;
		page_bits &= ~0xFFFFFFu;
		destination = 0x300000u + (byte_swap32(page_bits) << 8) * 4u;
		next_entry = page_list + 4u;
		source = entry_word ^ page_bits;
		list_next = next_entry;
		unpack(source, destination);
		page_list = list_next;
	}
	return list_next;
}

/* Unused: queues the entries of an unpack_list list (the packed data's address, where to unpack it;
   closed by a negative entry, the first taken unchecked) as they are in the screen's ring unpack_queue
   (unpack_write_place), with unpack_state set to say there is work, for unpack_queued_step.
   queue_tile_pages does the same for a list of tile pages. */
void queue_unpack_list(const struct packed_block_entry *block_list)
{
	uint32_t unpack_write_place = 0;
	struct unpack_request *request = NULL;
	uint32_t destination = 0;

	do
	{
		unpack_write_place = screen->unpack_write_place;
		request = pointer_to(address_of(screen) + 0xC00u + unpack_write_place * 2u * 4u);
		screen->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		destination = block_list->destination;
		request->source = block_list->source;
		request->destination = destination;
		screen->unpack_state = (screen->unpack_state & 0xFF00u) | 0xFFu;
		block_list = block_list + 1;
	} while ((int32_t)block_list->source >= 0);
}

/* Queues the tilemap pages of the list R10 to be unpacked into video RAM: each 32-bit entry a page
   number in its top byte (the page at 300000 + 400 * page) and the address of its packed data below,
   closed by a negative entry; the requests go into the screen's ring of 64 at C00 (unpack_write_place
   the place) and unpack_state says there is work. Used by set_up_background, ranking_background_load and
   race_background_update. */
void queue_tile_pages(uint32_t page_list)
{
	uint32_t page_bits = 0, page_entry = 0, unpack_write_place = 0;
	struct unpack_request *request = NULL;

	while ((int32_t)TILE_PAGE_ENTRIES(page_list)[0] >= 0)
	{
		unpack_write_place = screen->unpack_write_place;
		request = pointer_to(address_of(screen) + 0xC00u + unpack_write_place * 2u * 4u);
		screen->unpack_write_place = (unpack_write_place + 1u) & 0x3Fu;
		page_bits = TILE_PAGE_ENTRIES(page_list)[0];
		page_entry = page_bits;
		page_bits &= ~0xFFFFFFu;
		request->source = page_entry ^ page_bits;
		request->destination = 0x300000u + (byte_swap32(page_bits) << 8) * 4u;
		screen->unpack_state = (screen->unpack_state & 0xFF00u) | 0xFFu;
		page_list += 4u;
	}
}

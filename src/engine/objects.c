/**
 *    Name:        engine/objects.c
 *    Description: the objects: the object loop, starting and freeing objects and their lists, the random numbers
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Ends all of the screen's objects at once: every slot free (screen - 7FF0), both object lists empty
   (this frame's at - 7E00, the next one's at - 7D00, with run_objects' pointers to them at - 8000, -
   7FFC, - 7FF4 and - 7FF8), and the queue of palette uploads (palette_upload_count) emptied. At boot and
   as each game mode starts. */
void clear_objects(void)
{
	uint32_t halfword_count = 0;

	move_halfwords(screen - 0x7FF0u, zero_block_ADDRESS, 0x7Cu);
	OBJECT_AREA(screen - 0x8000u)->list = screen - 0x7E00u;
	OBJECT_AREA(screen - 0x8000u)->list_first = screen - 0x7DFEu;
	OBJECT_AREA(screen - 0x8000u)->list_end = screen - 0x7DFEu;
	OBJECT_AREA(screen - 0x8000u)->next_list = screen - 0x7D00u;
	halfword_count = (uint8_t)OBJECT_LIST(screen - 0x7E00u)->count;
	halfword_count = (uint16_t)(halfword_count + 1u);
	move_halfwords(screen - 0x7E00u, zero_block_ADDRESS, halfword_count);
	halfword_count = (uint8_t)OBJECT_LIST(screen - 0x7D00u)->count;
	halfword_count = (uint16_t)(halfword_count + 1u);
	move_halfwords(screen - 0x7D00u, zero_block_ADDRESS, halfword_count);
	SCREEN(screen)->palette_upload_count &= 0xFFFF0000u;
}

/* The object loop, once a frame for the screen: each live object (flags bit 0) of this frame's list
   (screen - 8000) has its handler run and goes on the next frame's list (screen - 7FF8); a dead one
   frees its slot (its byte at screen - 7FF0, by its slot number at 8). Then this frame's list is cleared
   and the two lists swap. */
void run_objects(void)
{
	uint32_t blocks_left = 0, count = 0, list_block = 0, list_entry = 0, zero_high = 0, zero_low = 0;
	uint32_t next_list = 0;

	current_object = screen;
	list_entry = OBJECT_AREA(screen - 0x8000u)->list + 2u;
	OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->next_list)->count = 0u;
	for (;;)
	{
		current_object = LINKED(current_object, OBJECT_LINKS(list_entry)[0]);
		list_entry += 2u;
		if ((uint16_t)current_object == 0u)
			break;
		if ((OBJECT(current_object)->flags & 1u) != 0u)
		{
			call_code(OBJECT(current_object)->handler);
			next_list = OBJECT_AREA(screen - 0x8000u)->next_list;
			OBJECT_LIST(next_list)->count++;
			OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->next_list)->objects[(int32_t)(OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->next_list)->count - 1u)] = current_object;
			continue;
		}
		OBJECT_AREA(screen - 0x8000u)->slots[OBJECT(current_object)->slot] = 0u;
	}
	list_block = OBJECT_AREA(screen - 0x8000u)->list;
	blocks_left = OBJECT_LIST_BLOCK(list_block)->count;
	blocks_left = (uint16_t)(blocks_left + 4u);
	blocks_left = blocks_left >> 2;
	zero_low = 0u;
	zero_high = 0u;
	do
	{
		OBJECT_LIST_BLOCK(list_block)->low = zero_low;
		OBJECT_LIST_BLOCK(list_block)->high = zero_high;
		blocks_left--;
		list_block += 8u;
	} while (blocks_left != 0u);
	OBJECT_AREA(screen - 0x8000u)->list ^= 0x100u;
	OBJECT_AREA(screen - 0x8000u)->next_list ^= 0x100u;
	count = OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->list)->count;
	OBJECT_AREA(screen - 0x8000u)->list_first = OBJECT_AREA(screen - 0x8000u)->list + 2u;
	OBJECT_AREA(screen - 0x8000u)->list_end = OBJECT_AREA(screen - 0x8000u)->list_first + count * 2u;
}

/* Takes the first free object slot of the screen (FF into its byte at screen - 7FF0), clears its 100
   bytes, makes it live (flags bit 0) with its slot number at 8, puts it on this frame's list and returns
   it; with no slot free, it returns the spare object at screen - 7F00 without any of that. */
uint32_t start_object(void)
{
	uint32_t zero = 0;
	uint32_t free_flag = 0, free_index = 0, object = 0, object_bytes = 0;
	uint32_t list = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &free_index, &free_flag);
	zero = free_index == 0x7Cu;
	if (zero)
	{
		object = screen - 0x7F00u;
		return object;
	}
	OBJECT_SLOT_FLAGS(free_flag)[0] = 0xFFu;
	object = screen - 0x7C00u + (free_index << 8);
	object_bytes = object;
	for (uint32_t repeat = 0; repeat < 31u; repeat++)
	{
		OBJECT_WORDS(object_bytes)[0] = 0u;
		OBJECT_WORDS(object_bytes)[1] = 0u;
		object_bytes += 8u;
	}
	OBJECT_WORDS(object_bytes)[0] = 0u;
	OBJECT_WORDS(object_bytes)[1] = 0u;
	OBJECT(object)->flags |= 1u;
	OBJECT(object)->slot = free_index;
	list = OBJECT_AREA(screen - 0x8000u)->list;
	OBJECT_LIST(list)->count++;
	OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->list)->objects[(int32_t)(OBJECT_LIST(OBJECT_AREA(screen - 0x8000u)->list)->count - 1u)] = object;
	return object;
}

/* Unused: takes the first free object slot as start_object does (FF into its byte of the 7C at screen -
   7FF0) and clears 100 bytes for it, but at the slot byte's address with its low half shifted 8 left
   (201000 + 100 * slot for screen 1), not where start_object puts slot n (screen - 7C00 + 100 * n), and
   without making it live, its slot number or the frame's list; with no slot free, the spare object at
   screen - 7F00. Returns the object. */
uint32_t reserve_object_slot(void)
{
	uint32_t zero = 0;
	uint32_t free_flag = 0, free_index = 0, object = 0;

	search_bytes(screen - 0x7FF0u, 0x7Cu, 0u, 1u, &free_index, &free_flag);
	zero = free_index == 0x7Cu;
	if (zero)
	{
		object = screen - 0x7F00u;
		return object;
	}
	OBJECT_SLOT_FLAGS(free_flag)[0] = 0xFFu;
	object = LINKED(free_flag, (uint16_t)(free_flag << 8));
	move_halfwords(object, zero_block_ADDRESS, 0x80u);
	return object;
}

/* Unused: frees the slot of the object R21: clears the slot byte (screen - 7FF0) numbered by bits 8-15
   of the object's address, which is not the object's own slot: for start_object's slot n it clears slot
   n + 4, for reserve_object_slot's slot n slot n + 10. */
void free_object_slot(uint16_t object)
{
	OBJECT_AREA(screen - 0x8000u)->slots[(uint16_t)object >> 8] = 0u;
}

/* Starts an object for each handler of the list R10 (a 32-bit count, then the handlers: the
   object_handlers_* tables), each with its place in the list at 9, and returns the last one. Used by the
   game modes (game_mode_car_presentation, game_mode_ranking, game_mode_play, initialise_board...),
   name_entry and ranking. */
uint32_t start_object_list(uint32_t handler_list)
{
	uint8_t list_index = 0;
	uint32_t object = 0, objects_left = 0;

	objects_left = OBJECT_START_LIST(handler_list)->count;
	list_index = 0u;
	handler_list += 4u;
	do
	{
		object = start_object();
		OBJECT(object)->handler = OBJECT_HANDLERS(handler_list)[0];
		OBJECT(object)->number = list_index;
		list_index++;
		objects_left--;
		handler_list += 4u;
	} while (objects_left != 0u);
	return object;
}

/* Returns the next pseudo-random number of the screen's sequence (kept in random_state). */
uint32_t random_number(void)
{
	uint32_t number = 0, state = 0;

	state = SCREEN(screen)->random_state;
	if ((uint16_t)state == 0u)
		state ^= 0x2A6D365Bu;
	number = state;
	state = (state * 2u * 2u + number) * 2u * 2u * 2u + number;
	number = (number & ~0xFFFFu) | (uint16_t)state;
	state = (state << 16) | (state >> 16);
	number = (number & ~0xFFFFu) | (uint16_t)(number + state);
	state = (state & ~0xFFFFu) | (uint16_t)number;
	SCREEN(screen)->random_state = (state << 16) | (state >> 16);
	return number;
}

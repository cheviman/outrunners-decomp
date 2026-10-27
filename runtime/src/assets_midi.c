/**
 *    Name:        assets_midi.c
 *    Description: the sound program's sequences from the assets' MIDI files: the driver's bytes back
 *    Version:     0.1.0
 *    Created:     27-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The way back of tools/formats/sound_music.py (midi_to_sequence and effect_from_mid, whose docstring has
 *    the whole mapping): the file's tracks and their events; the sequencer-specific meta events FF 7F 7D TAG
 *    (H: the effect's flags byte, A: the bytes after the end, C and E: an event as the driver has it, T: the
 *    rest of a stream that reads to no end); the track with the driver's events (several: merged in time
 *    order); then the driver's stream, its delta times after each event, its running status and its chains
 *    (bit 7 on an event's last byte).
 */
#include "assets_internal.h"

#include <stdlib.h>
#include <string.h>

#define PRIVATE      0x7D
#define TAG_HEADER   'H'
#define TAG_AFTER    'A'
#define TAG_CHANNEL  'C'
#define TAG_EVENT    'E'
#define TAG_REST     'T'

enum { KIND_CHANNEL, KIND_META, KIND_SYSEX };

typedef struct
{
	uint32_t time;
	uint8_t delta[8];            /* the delta time's bytes as the file has them */
	int delta_length;
	int kind;
	int status;                  /* a channel event's status byte, a meta event's type, F0 or F7 */
	int explicit;                /* whether the file has the status byte */
	const uint8_t *payload;
	size_t payload_length;
	int track;
	int order;
} midi_event;

typedef struct
{
	midi_event *events;
	size_t count, room;
} event_list;

static int add(event_list *list, const midi_event *event)
{
	if (list->count == list->room)
	{
		midi_event *grown;

		list->room = list->room ? list->room * 2 : 256;
		if (!(grown = realloc(list->events, list->room * sizeof(*grown))))
			return 0;
		list->events = grown;
	}
	list->events[list->count++] = *event;
	return 1;
}

/* A variable-length quantity at track[*at]: its value, and its bytes into 'bytes' (up to 8) */
static int read_vlq(const uint8_t *track, size_t size, size_t *at, uint32_t *value, uint8_t *bytes, int *length)
{
	*value = 0;
	*length = 0;
	for (;;)
	{
		uint8_t byte;

		if (*at >= size)
			return 0;
		byte = track[(*at)++];
		if (bytes && *length < 8)
			bytes[*length] = byte;
		(*length)++;
		*value = *value << 7 | (byte & 0x7fu);
		if (!(byte & 0x80))
			return 1;
	}
}

static int read_track(const uint8_t *track, size_t size, int number, event_list *list)
{
	size_t at = 0;
	uint32_t time = 0;
	int running = -1, order = 0;

	while (at < size)
	{
		midi_event event;
		uint32_t delta, length;
		int length_bytes;
		uint8_t byte;

		memset(&event, 0, sizeof(event));
		if (!read_vlq(track, size, &at, &delta, event.delta, &event.delta_length) || at >= size)
			return 0;
		time += delta;
		event.time = time;
		event.track = number;
		event.order = order++;
		byte = track[at];
		if (byte == 0xff)
		{
			if (at + 1 >= size)
				return 0;
			event.kind = KIND_META;
			event.status = track[at + 1];
			event.explicit = 1;
			at += 2;
			if (!read_vlq(track, size, &at, &length, NULL, &length_bytes) || length > size - at)
				return 0;
			event.payload = track + at;
			event.payload_length = length;
			at += length;
			if (!add(list, &event))
				return 0;
			if (event.status == 0x2f)
				break;
			continue;
		}
		if (byte == 0xf0 || byte == 0xf7)
		{
			event.kind = KIND_SYSEX;
			event.status = byte;
			event.explicit = 1;
			at++;
			if (!read_vlq(track, size, &at, &length, NULL, &length_bytes) || length > size - at)
				return 0;
			event.payload = track + at;
			event.payload_length = length;
			at += length;
			if (!add(list, &event))
				return 0;
			continue;
		}
		if (byte & 0x80)
		{
			if (byte > 0xef)
				return 0;
			event.status = byte;
			event.explicit = 1;
			at++;
		}
		else if (running < 0)
			return 0;
		else
			event.status = running;
		running = event.status;
		event.kind = KIND_CHANNEL;
		event.payload = track + at;
		event.payload_length = ((event.status & 0xf0) == 0xc0 || (event.status & 0xf0) == 0xd0) ? 1 : 2;
		if (event.payload_length > size - at)
			return 0;
		at += event.payload_length;
		if (!add(list, &event))
			return 0;
	}
	return 1;
}

/* The tag of one of these files' sequencer-specific meta events (FF 7F 7D TAG ...), or 0 */
static int tag_of(const midi_event *event)
{
	if (event->kind == KIND_META && event->status == 0x7f && event->payload_length >= 2 && event->payload[0] == PRIVATE)
		return event->payload[1];
	return 0;
}

static int is_end(const midi_event *event)
{
	return event->kind == KIND_META && event->status == 0x2f;
}

static int drives(const midi_event *event)
{
	const int tag = tag_of(event);

	return event->kind == KIND_CHANNEL || tag == TAG_CHANNEL || tag == TAG_EVENT || tag == TAG_REST || is_end(event);
}

static int by_time(const void *first, const void *second)
{
	const midi_event *one = first, *other = second;

	if (one->time != other->time)
		return one->time < other->time ? -1 : 1;
	if (one->track != other->track)
		return one->track < other->track ? -1 : 1;
	return one->order < other->order ? -1 : one->order > other->order;
}

/* The shortest variable-length quantity of 'value' into bytes */
static int write_vlq(uint32_t value, uint8_t *bytes)
{
	uint8_t reversed[5];
	int count = 0;

	do
	{
		reversed[count++] = (uint8_t)(value & 0x7f);
		value >>= 7;
	} while (value);
	for (int index = 0; index < count; index++)
		bytes[index] = (uint8_t)(reversed[count - 1 - index] | (index < count - 1 ? 0x80 : 0));
	return count;
}

typedef struct
{
	uint8_t *out;
	size_t room, at;
} output;

static int emit(output *into, const uint8_t *bytes, size_t count)
{
	if (into->at + count > into->room)
		return 0;
	memcpy(into->out + into->at, bytes, count);
	into->at += count;
	return 1;
}

/* The driver's delta time for the file's one (sound_music.py _driver_delta): 1 with its bytes in 'out'
   (none: a chain), 0 when the driver has no such delta time */
static int driver_delta(const uint8_t *raw, int raw_length, uint32_t value, int plain, int can_chain,
	uint8_t *out, int *length, int *chain)
{
	*chain = 0;
	*length = 0;
	if (plain)
	{
		if (raw_length == 2 && raw[0] == 0x80 && raw[1] == 0x00)
		{
			out[(*length)++] = 0x00;
			return 1;
		}
		if (raw_length == 3 && raw[0] == 0x84 && raw[1] == 0x80 && raw[2] == 0x00)
		{
			out[(*length)++] = 0x80;
			out[(*length)++] = 0x00;
			return 1;
		}
		if ((raw_length == 1 || raw_length == 2) && value)
		{
			memcpy(out, raw, (size_t)raw_length);
			*length = raw_length;
			return 1;
		}
	}
	if (value == 0)
	{
		if (can_chain)
			*chain = 1;
		else
			out[(*length)++] = 0x00;
		return 1;
	}
	if (value < 0x80)
		out[(*length)++] = (uint8_t)value;
	else if (value < 0x4000)
	{
		out[(*length)++] = (uint8_t)(0x80 | value >> 7);
		out[(*length)++] = (uint8_t)(value & 0x7f);
	}
	else if (value == 0x10000)
	{
		out[(*length)++] = 0x80;
		out[(*length)++] = 0x00;
	}
	else
		return 0;
	return 1;
}

int assets_midi_sequence(const uint8_t *blob, size_t size, uint8_t *out, size_t room, size_t *length, int *flags)
{
	event_list all = { 0 }, playing = { 0 };
	const midi_event *events;
	size_t event_count;
	const uint8_t *after = (const uint8_t *)"\0";
	size_t after_length = 1;
	int have_after = 0, track_count = 0, playing_tracks = 0, first_playing = -1;
	size_t at;
	output into = { out, room, 0 };
	int running = -1, started = 0, plain = 1, read = 1, ended = 0;
	long last = -1;
	uint32_t pending = 0;

	*flags = -1;
	if (size < 8 || memcmp(blob, "MThd", 4) != 0)
		return 0;
	at = 8 + ((size_t)blob[4] << 24 | (size_t)blob[5] << 16 | (size_t)blob[6] << 8 | blob[7]);
	while (read && at + 8 <= size)
	{
		const size_t chunk = (size_t)blob[at + 4] << 24 | (size_t)blob[at + 5] << 16 | (size_t)blob[at + 6] << 8 | blob[at + 7];

		if (chunk > size - at - 8)
			read = 0;
		else if (memcmp(blob + at, "MTrk", 4) == 0)
			read = read_track(blob + at + 8, chunk, track_count++, &all);
		at += 8 + chunk;
	}
	/* the attributes, and which tracks play */
	for (size_t index = 0; read && index < all.count; index++)
	{
		const midi_event *event = &all.events[index];
		const int tag = tag_of(event);

		if (tag == TAG_HEADER && *flags < 0 && event->payload_length > 2)
			*flags = event->payload[2];
		if (tag == TAG_AFTER && !have_after)
		{
			after = event->payload + 2;
			after_length = event->payload_length - 2;
			have_after = 1;
		}
	}
	for (int track = 0; read && track < track_count; track++)
		for (size_t index = 0; index < all.count; index++)
			if (all.events[index].track == track && !is_end(&all.events[index]) && drives(&all.events[index]))
			{
				playing_tracks++;
				if (first_playing < 0)
					first_playing = track;
				break;
			}
	if (read && playing_tracks > 1)
	{
		/* several: their events but the ends in time order, then an end at the latest time */
		uint32_t end = 0, time = 0;

		for (size_t index = 0; read && index < all.count; index++)
		{
			const midi_event *event = &all.events[index];
			int plays = 0;

			for (size_t other = 0; other < all.count && !plays; other++)
				plays = all.events[other].track == event->track && !is_end(&all.events[other]) && drives(&all.events[other]);
			if (!plays)
				continue;
			if (event->time > end)
				end = event->time;
			if (!is_end(event))
				read = add(&playing, event);
		}
		qsort(playing.events, playing.count, sizeof(midi_event), by_time);
		if (read)
		{
			midi_event closing;

			memset(&closing, 0, sizeof(closing));
			closing.kind = KIND_META;
			closing.status = 0x2f;
			closing.time = end;
			read = add(&playing, &closing);
		}
		for (size_t index = 0; read && index < playing.count; index++)
		{
			playing.events[index].delta_length = write_vlq(playing.events[index].time - time, playing.events[index].delta);
			time = playing.events[index].time;
		}
		events = playing.events;
		event_count = playing.count;
	}
	else
	{
		/* one (or none: the first track's events) */
		const int track = first_playing >= 0 ? first_playing : 0;

		for (size_t index = 0; read && index < all.count; index++)
			if (all.events[index].track == track)
				read = add(&playing, &all.events[index]);
		events = playing.events;
		event_count = playing.count;
	}
	/* the driver's stream */
	for (size_t index = 0; read && index < event_count && !ended; index++)
	{
		const midi_event *event = &events[index];
		const int tag = tag_of(event);
		uint32_t value = 0;
		size_t vlq_at = 0;
		int vlq_length;

		read_vlq(event->delta, (size_t)event->delta_length, &vlq_at, &value, NULL, &vlq_length);
		if (!drives(event))
		{
			pending += value;
			plain = 0;
			continue;
		}
		if (started)
		{
			uint8_t delta[4];
			int delta_length, chain;

			if (!driver_delta(event->delta, event->delta_length, pending + value, plain, last >= 0, delta, &delta_length, &chain))
			{
				read = 0;
				break;
			}
			if (chain)
				into.out[last] |= 0x80;
			else
				read = emit(&into, delta, (size_t)delta_length);
		}
		started = 1;
		pending = 0;
		plain = 1;
		if (is_end(event))
		{
			read = read && emit(&into, (const uint8_t *)"\xff\x2f", 2) && emit(&into, after, after_length);
			ended = 1;
		}
		else if (tag == TAG_REST)
		{
			read = read && emit(&into, event->payload + 2, event->payload_length - 2);
			ended = 1;
		}
		else if (event->kind == KIND_CHANNEL)
		{
			const uint8_t status = (uint8_t)event->status;

			if (event->explicit || event->status != running)
			{
				read = read && emit(&into, &status, 1);
				running = event->status;
			}
			read = read && emit(&into, event->payload, (event->status & 0xf0) == 0x80 ? 1 : event->payload_length);
			last = (long)into.at - 1;
		}
		else
		{
			const uint8_t *raw = event->payload + 2;
			const size_t raw_length = event->payload_length - 2;

			read = read && emit(&into, raw, raw_length);
			if (raw_length && (raw[0] & 0x80))
				running = raw[0];
			last = (tag == TAG_CHANNEL && raw_length) ? (long)into.at - 1 : -1;
		}
	}
	if (read && !ended)
	{
		/* no end in the file: the sequence ends right after its last event */
		if (started)
		{
			uint8_t delta[4];
			int delta_length, chain;

			if (!driver_delta(NULL, 0, pending, 0, last >= 0, delta, &delta_length, &chain))
				read = 0;
			else if (chain)
				into.out[last] |= 0x80;
			else
				read = emit(&into, delta, (size_t)delta_length);
		}
		read = read && emit(&into, (const uint8_t *)"\xff\x2f", 2) && emit(&into, after, after_length);
	}
	free(all.events);
	free(playing.events);
	*length = into.at;
	return read;
}

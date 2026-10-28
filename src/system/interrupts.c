/**
 *    Name:        system/interrupts.c
 *    Description: the interrupts: the VBLANK (the tasks, the mixer and scrolls, the test button, the sound requests, the lamps), the timers and the link
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    27-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* The vertical blank interrupt (IRQ 0), every video frame: counts vblank_counter and
   vblanks_since_start, starts timer 0 for the LCD (int_vector_43, 34 times: timer_interrupts_left); when
   it interrupted screen 2's task: both screens' mixers and palettes (vblank_update_mixer), the video
   registers (all 72 halfwords from video_registers when send_video_registers asks for them, which it
   clears, else the 8 at video_registers[16] and the 8 at video_registers[32]) and the external tile
   banks of chip 0's port H (2 bits each of tile_banks-tile_banks[3]); then for the interrupted task's
   screen the sprite list (finish_sprite_list), send_sound_requests, and vblank_update_scrolls unless the
   screen's skip_scroll_update is set (cleared here); outside the test mode the inputs
   (read_switches_and_analog), pack_seat_inputs (screen 1) or queue_seat_inputs (screen 2),
   check_test_button and, without free play, the coins (update_coins); then update_lamps, the LCD
   (lcd_refresh), count_total_time; sets frame_ready; swaps the tasks (task_swap) unless single_task is
   set, when task_swapping is set or when the test mode is on and screen 1's task is running; counts the
   frame for the running task's screen (screen_1_vblanks, screen_2_vblanks). */
void int_vector_40(void)
{
	uint32_t calibration = 0, screen_vblank_counter = 0, tile_bank_bits = 0;
	uint32_t saved_screen = 0, saved_r12 = 0, saved_screen_2 = 0;

	saved_screen = screen;
	saved_r12 = r12;
	vblank_counter++;
	vblanks_since_start++;
	io_write16(IRQ_TIMER_0, vblank_timer_period);
	timer_interrupts_left = 0x22u;
	debug_timer_rounds--;
	if (SCREEN(screen)->index != 0u)
	{
		saved_screen_2 = screen;
		screen = SCREEN_1;
		vblank_update_mixer();
		screen = SCREEN_2;
		vblank_update_mixer();
		screen = saved_screen_2;
		if (send_video_registers == 0u)
		{
			move_halfwords(NBG2_SCROLL_X_ADDRESS, video_registers_ADDRESS + 0x20u, 8u);
			move_halfwords(VIDEO_RAM + 0x1FF40u, video_registers_ADDRESS + 0x40u, 8u);
		}
		else
		{
			send_video_registers = 0u;
			move_halfwords(VIDEO_CONTROL_ADDRESS, video_registers_ADDRESS, 0x48u);
		}
		tile_bank_bits = tile_banks[0];
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[1]);
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[2]);
		tile_bank_bits = (uint8_t)((tile_bank_bits << 6) | (tile_bank_bits >> 2) | tile_banks[3]);
		io_write8(IO_1_PORT_H, (tile_bank_bits << 6) | (tile_bank_bits >> 2));
	}
	finish_sprite_list();
	send_sound_requests();
	if (SCREEN(screen)->skip_scroll_update == 0u)
		vblank_update_scrolls();
	SCREEN(screen)->skip_scroll_update = 0u;
	if (test_mode_on == 0u)
	{
		calibration = read_switches_and_analog();
		switch (SCREEN(screen)->index)
		{
		case 0:
			pack_seat_inputs();
			break;
		case 1:
			tile_bank_bits = queue_seat_inputs();
			break;
		}
		check_test_button(tile_bank_bits);
		if (free_play == 0u)
			update_coins(calibration);
	}
	update_lamps();
	lcd_refresh();
	count_total_time();
	frame_ready = 0xFFu;
	if (single_task == 0u && ((test_mode_on != 0u && SCREEN(screen)->index == 0u) || task_swapping != 0u))
		swap_tasks();
	screen_vblank_counter = screen_vblank_counters[SCREEN(screen)->index];
	VBLANK_COUNT(screen_vblank_counter)[0]++;
	r12 = saved_r12;
	screen = saved_screen;
	io_write8(IRQ_ACKNOWLEDGE, 0xFEu);
}

/* In the vertical blank, a screen's mixer and colours: while the screen's flags bit 0 is set, a step of
   its colour offsets (step_screen_fade) into the mixer's colour offset registers (3E-4B); flags bit 2
   (cleared) sends the mixer settings at 160 of the screen (registers 00-3D and 4C-4F); flags bit 3
   (cleared) makes the set at 1B0 the current one and sends it; then the queued palettes
   (upload_queued_palettes). */
void vblank_update_mixer(void)
{
	uint32_t mixer = 0;
	uint32_t flags = 0, flags_2 = 0;

	if ((SCREEN(screen)->flags & 1u) != 0u)
	{
		step_screen_fade();
		move_halfwords(SCREEN(screen)->mixer + 0x3Eu, screen + 0x19Eu, 7u);
	}
	flags = SCREEN(screen)->flags;
	SCREEN(screen)->flags = flags & ~4u;
	if ((flags & 4u) != 0u)
	{
		mixer = SCREEN(screen)->mixer;
		move_halfwords(mixer, screen + 0x160u, 0x1Fu);
		move_halfwords(mixer + 0x4Cu, screen + 0x1ACu, 2u);
	}
	flags_2 = SCREEN(screen)->flags;
	SCREEN(screen)->flags = flags_2 & ~8u;
	if ((flags_2 & 8u) != 0u)
	{
		mixer = SCREEN(screen)->mixer;
		move_halfwords(screen + 0x160u, screen + 0x1B0u, 0x1Fu);
		move_halfwords(screen + 0x1ACu, screen + 0x1FCu, 2u);
		move_halfwords(mixer, screen + 0x1B0u, 0x1Fu);
		move_halfwords(mixer + 0x4Cu, screen + 0x1FCu, 2u);
	}
	if (SCREEN(screen)->palette_upload_count != 0u)
		upload_queued_palettes();
}

/* In the vertical blank, the tilemaps' scrolls (two per screen): those of the screen whose task was
   running from video_registers[8] and video_registers[16] as they are; those of the other screen
   half-way between the values they have (video_registers[8], video_registers[16] by screen) and the new
   ones (scroll_targets, scroll_targets[4]), which become their values: the in-between frame of the
   screen whose task did not run (each task's scrolls change every other frame); and the byte at 31FF05
   from video_registers[2]'s second byte, which steps by 4 (bits 2-3). */
void vblank_update_scrolls(void)
{
	uint8_t control_byte = 0;
	uint32_t new_scroll = 0, old_scroll = 0, other_layer = 0, running_layer = 0;

	running_layer = SCREEN(screen)->index * 2u;
	NBG0_SCROLL_X[running_layer] = RAM32(video_registers_ADDRESS + 0x10u + running_layer * 4u);
	NBG0_SCROLL_Y[running_layer] = RAM32(video_registers_ADDRESS + 0x14u + running_layer * 4u);
	other_layer = 0u - running_layer + 2u;
	old_scroll = RAM32(video_registers_ADDRESS + 0x10u + other_layer * 4u);
	new_scroll = scroll_targets[other_layer];
	RAM32(video_registers_ADDRESS + 0x10u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 6;
	NBG0_SCROLL_X[other_layer] = (((int32_t)((old_scroll << 6) - new_scroll) >> 1) + new_scroll) >> 6;
	old_scroll = RAM32(video_registers_ADDRESS + 0x14u + other_layer * 4u);
	new_scroll = scroll_targets[1 + other_layer];
	RAM32(video_registers_ADDRESS + 0x14u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 7;
	NBG0_SCROLL_Y[other_layer] = (((int32_t)((old_scroll << 7) - new_scroll) >> 1) + new_scroll) >> 7;
	NBG2_SCROLL_X[running_layer] = RAM32(video_registers_ADDRESS + 0x20u + running_layer * 4u);
	NBG2_SCROLL_Y[running_layer] = RAM32(video_registers_ADDRESS + 0x24u + running_layer * 4u);
	old_scroll = RAM32(video_registers_ADDRESS + 0x20u + other_layer * 4u);
	new_scroll = scroll_targets[4 + other_layer];
	RAM32(video_registers_ADDRESS + 0x20u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 6;
	NBG2_SCROLL_X[other_layer] = (((int32_t)((old_scroll << 6) - new_scroll) >> 1) + new_scroll) >> 6;
	old_scroll = RAM32(video_registers_ADDRESS + 0x24u + other_layer * 4u);
	new_scroll = scroll_targets[5 + other_layer];
	RAM32(video_registers_ADDRESS + 0x24u + other_layer * 4u) = new_scroll;
	new_scroll = new_scroll << 7;
	NBG2_SCROLL_Y[other_layer] = (((int32_t)((old_scroll << 7) - new_scroll) >> 1) + new_scroll) >> 7;
	control_byte = (uint8_t)(video_registers[2] >> 8u);
	VIDEO_LINE_CONTROL_HIGH = control_byte;
	video_registers[2] = (video_registers[2] & 0xFFu) | (((control_byte + 4u) & 0xECu & 0xFFu) << 8u);
}

/* The end of the vertical blank (IRQ 1): only acknowledged. */
void int_vector_41(void)
{
	io_write8(IRQ_ACKNOWLEDGE, 0xFDu);
}

/* The sound CPU's interrupt (IRQ 2): only acknowledged. The sound program never raises it. */
void int_vector_42(void)
{
	io_write8(IRQ_ACKNOWLEDGE, 0xFBu);
}

/* Timer 0 (IRQ 3), which int_vector_40 starts each frame at its shortest period: sends the LCD its next
   piece (lcd_send_next_nibble) and starts the timer again until timer_interrupts_left runs out, 34 times
   a frame. When it runs out it tests the screen's byte 1 and test_mode_on but goes the same way; the
   code after them (unused_03F7D8: counts debug_timer_rounds and shows it on the text layer) is never
   reached. */
void int_vector_43(void)
{
	uint8_t interrupts_left = 0;

	interrupts_left = timer_interrupts_left;
	timer_interrupts_left = interrupts_left - 1u;
	if (interrupts_left != 1u)
		io_write16(IRQ_TIMER_0, lcd_timer_period);
	lcd_send_next_nibble();
	io_write8(IRQ_ACKNOWLEDGE, 0xF7u);
}

/* Timer 1 (IRQ 4), the link board's: outside the ISDN mode (isdn_mode) only acknowledged, which leaves
   the timer stopped; in it counts link_wait_count and takes the kind of the link board's message (its
   byte at 800010, cleared as it is read): 3, a message, is copied out of the board
   (link_isdn_receive_records: 800600 on to 21E000), link_wait_count cleared and the timer started at
   168; any other kind polls again at the shortest period. link_unused_case is a fourth case (the
   controls, pack_seat_inputs, then 96) its table does not list. */
void int_vector_44(void)
{
	uint16_t timer_period = 0;
	uint32_t message_kind = 0;
	uint8_t received_kind = 0;

	if (isdn_mode == 0u)
	{
		io_write8(IRQ_ACKNOWLEDGE, 0xEFu);
		return;
	}
	link_wait_count++;
	timer_period = 1u;
	received_kind = io_read8(LINK_MESSAGE_KIND);
	message_kind = received_kind;
	io_write8(LINK_MESSAGE_KIND, 0u);
	if (message_kind < 4u)
	{
		switch (message_kind)
		{
		case 3:
			link_isdn_receive_records();
			timer_period = 0x168u;
			link_wait_count = 0u;
			break;
		case 0:
		case 1:
		case 2:
			break;
		}
	}
	io_write16(IRQ_TIMER_1, timer_period);
	io_write8(IRQ_ACKNOWLEDGE, 0xEFu);
}

/* The vectors no interrupt uses (250 of the 256): halt instructions, on into int_vector_02's. */
void int_default(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	int_vector_02();
}

/* The NMI: halt instructions, then on into check_test_button (whose return is not an interrupt's). */
void int_vector_02(void)
{
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	halt();
	check_test_button(0u);
}

/* Turns the test mode on (test_mode_on FF) when TEST has just been pressed (bit 7 of switches_pressed,
   the inputs newly pressed); the boot task's loop then runs the test mode. */
void check_test_button(uint32_t leftover)
{
	if ((((leftover & ~0xFFu) | switches_pressed) & 0x80u) != 0u)
		test_mode_on = 0xFFu;
}

/* In the vertical blank, sends the screen's next sound request to the sound CPU: one from its ring of 32
   (queue_sound's: sound_queue_read, sound_queue_write, the requests after them) to its command byte
   (SOUND_COMMAND_1 for screen 1, SOUND_COMMAND_2 for screen 2), and one from its second ring at 840
   (second_sound_queue_read, second_sound_queue_write; only the unused request_second_sound fills it) to
   the byte 8 after it, which the sound program never reads. */
void send_sound_requests(void)
{
	uint32_t command_offset = 0, queue = 0, second_queue = 0, second_sound_queue_read = 0, sound_queue_read = 0;
	uint8_t sound_queue_write = 0, second_sound_queue_write = 0;

	command_offset = (uint32_t)SCREEN(screen)->index << 8;
	queue = screen + 0x800u;
	sound_queue_read = SCREEN(queue - 0x800u)->sound_queue_read;
	queue++;
	sound_queue_write = SCREEN(queue - 0x801u)->sound_queue_write;
	queue++;
	if (sound_queue_read != sound_queue_write)
	{
		SOUND_COMMAND_1[command_offset] = SCREEN(queue - 0x802u)->sound_queue[(int32_t)sound_queue_read];
		SCREEN(queue - 0x802u)->sound_queue_read = (sound_queue_read + 1u) & 0x1Fu;
	}
	second_queue = screen + 0x840u;
	second_sound_queue_read = SCREEN(second_queue - 0x840u)->second_sound_queue_read;
	second_queue++;
	second_sound_queue_write = SCREEN(second_queue - 0x841u)->second_sound_queue_write;
	second_queue++;
	if (second_sound_queue_read != second_sound_queue_write)
	{
		SOUND_SECOND_COMMAND_1[command_offset] = SCREEN(second_queue - 0x842u)->second_sound_queue[(int32_t)second_sound_queue_read];
		SCREEN(second_queue - 0x842u)->second_sound_queue_read = (second_sound_queue_read + 1u) & 0x1Fu;
	}
}

/* Counts the bookkeeping's TOTAL TIME (701F10, seconds, kept with its copy and checksum), outside the
   test mode only: total_time_count gains 17 every vertical blank and a second passes at each 1000 (2000
   for every 73rd second). */
void count_total_time(void)
{
	uint16_t time_count = 0;
	uint32_t total_time = 0;

	if (test_mode_on != 0u)
		return;
	time_count = (uint16_t)(total_time_count + 0x11u);
	if (time_count < 0x3E8u)
	{
		total_time_count = time_count;
		return;
	}
	total_time = BOOKKEEPING_TOTAL_TIME_ADDRESS;
	if (BOOKKEEPING_TOTAL_TIME % 0x49u != 0u)
	{
		total_time_count = time_count - 0x3E8u;
		if (BOOKKEEPING_COUNT32(total_time)->value != 0xFFFFFFFFu)
		{
			BOOKKEEPING_COUNT32(total_time)->value++;
			bookkeeping_update_checksum(time_count);
			BOOKKEEPING_COUNT32(total_time)->backup++;
		}
		return;
	}
	if (time_count < 0x7D0u)
	{
		total_time_count = time_count;
		return;
	}
	time_count -= 0x3E8u;
	total_time_count = time_count - 0x3E8u;
	if (BOOKKEEPING_COUNT32(total_time)->value != 0xFFFFFFFFu)
	{
		BOOKKEEPING_COUNT32(total_time)->value++;
		bookkeeping_update_checksum(time_count);
		BOOKKEEPING_COUNT32(total_time)->backup++;
	}
}

/* In the vertical blank, the cabinet's lamps: blinks each seat's CHECK POINT lamp (output port D bit 1:
   checkpoint_lamp, checkpoint_lamp_2) and RACE LEADER lamp (bit 3: leader_lamp, leader_lamp_2), each
   with a blinks, a period and a timer byte after it: while blinks is not 0 the timer counts the vertical
   blanks and at 0 toggles the lamp, starts again from period and takes one off blinks; writes output
   port D of each I/O chip from port_d_outputs or port_d_outputs[1] with those lamps and, when the seat
   is not in the attract mode (screen demo_flags bit 8) and the STEERING WHEEL REACTION setting
   (steering_wheel_reaction) is on, the wheel motor's bits (wheel_motor by seat); then the music and
   arrows lamps (update_button_lamps). */
void update_lamps(void)
{
	uint8_t port_d = 0;
	uint32_t port_g = 0, screen_block = 0;
	uint8_t checkpoint_timer = 0, leader_timer = 0, checkpoint_2_timer = 0, leader_2_timer = 0;

	if (checkpoint_lamp_blinks != 0u)
	{
		checkpoint_timer = checkpoint_lamp_timer;
		checkpoint_lamp_timer = checkpoint_timer - 1u;
		if (checkpoint_timer == 1u)
		{
			checkpoint_lamp_timer = checkpoint_lamp_period;
			checkpoint_lamp[0] ^= 2u;
			checkpoint_lamp_blinks--;
		}
	}
	if (leader_lamp_blinks != 0u)
	{
		leader_timer = leader_lamp_timer;
		leader_lamp_timer = leader_timer - 1u;
		if (leader_timer == 1u)
		{
			leader_lamp_timer = leader_lamp_period;
			leader_lamp[0] ^= 8u;
			leader_lamp_blinks--;
		}
	}
	port_d = port_d_outputs[0] | checkpoint_lamp[0] | leader_lamp[0];
	if ((SCREEN(SCREEN_1)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		port_d |= wheel_motor[0];
	io_write8(IO_1_PORT_D, port_d);
	if (checkpoint_lamp_2_blinks != 0u)
	{
		checkpoint_2_timer = checkpoint_lamp_2_timer;
		checkpoint_lamp_2_timer = checkpoint_2_timer - 1u;
		if (checkpoint_2_timer == 1u)
		{
			checkpoint_lamp_2_timer = checkpoint_lamp_2_period;
			checkpoint_lamp_2 ^= 2u;
			checkpoint_lamp_2_blinks--;
		}
	}
	if (leader_lamp_2_blinks != 0u)
	{
		leader_2_timer = leader_lamp_2_timer;
		leader_lamp_2_timer = leader_2_timer - 1u;
		if (leader_2_timer == 1u)
		{
			leader_lamp_2_timer = leader_lamp_2_period;
			leader_lamp_2 ^= 8u;
			leader_lamp_2_blinks--;
		}
	}
	port_d = port_d_outputs[1] | checkpoint_lamp_2 | leader_lamp_2;
	if ((SCREEN(SCREEN_2)->demo_flags & 0x100u) == 0u && steering_wheel_reaction != 0u)
		port_d |= wheel_motor[1];
	io_write8(IO_2_PORT_D, port_d);
	screen_block = SCREEN_1;
	port_g = port_g_outputs_ADDRESS;
	update_button_lamps(screen_block, port_g);
	screen_block = SCREEN_2;
	port_g = port_g_outputs_ADDRESS + 0x1u;
	update_button_lamps(screen_block, port_g);
}

/* A screen's two button lamps, in its port G image (R11: port_g_outputs or port_g_outputs[1], which the
   LCD's output sends): the DJ/MUSIC lamp (bit 0) by the low byte of button_lamps and the << >> lamp (bit
   1) by its high byte: 0 off, negative on, n blinking every n vertical blanks (music_lamp_timer,
   arrows_lamp_timer), each mode taken when it changes (music_lamp_mode, arrows_lamp_mode). */
void update_button_lamps(uint32_t screen_block, uint32_t port_g)
{
	uint8_t arrows_mode = 0, button_lamps = 0, lamps = 0;
	uint8_t music_lamp_timer = 0, arrows_lamp_timer = 0;

	lamps = PORT_OUTPUT_BITS(port_g)[0];
	button_lamps = (uint8_t)SCREEN(screen_block)->button_lamps;
	if (button_lamps != SCREEN(screen_block)->music_lamp_mode)
	{
		SCREEN(screen_block)->music_lamp_mode = button_lamps;
		if ((int8_t)button_lamps > 0)
		{
			lamps ^= 1u;
			SCREEN(screen_block)->music_lamp_timer = button_lamps;
		}
		else if (button_lamps == 0u)
			lamps &= ~1u;
		else
			lamps |= 1u;
	}
	else if ((int8_t)button_lamps > 0)
	{
		music_lamp_timer = SCREEN(screen_block)->music_lamp_timer;
		SCREEN(screen_block)->music_lamp_timer = music_lamp_timer - 1u;
		if (music_lamp_timer == 1u)
		{
			lamps ^= 1u;
			SCREEN(screen_block)->music_lamp_timer = button_lamps;
		}
	}
	arrows_mode = (uint8_t)(SCREEN(screen_block)->button_lamps >> 8u);
	if (arrows_mode == SCREEN(screen_block)->arrows_lamp_mode)
	{
		if ((int8_t)arrows_mode > 0)
		{
			arrows_lamp_timer = SCREEN(screen_block)->arrows_lamp_timer;
			SCREEN(screen_block)->arrows_lamp_timer = arrows_lamp_timer - 1u;
			if (arrows_lamp_timer == 1u)
			{
				lamps ^= 2u;
				SCREEN(screen_block)->arrows_lamp_timer = arrows_mode;
			}
		}
		PORT_OUTPUT_BITS(port_g)[0] = lamps;
		return;
	}
	SCREEN(screen_block)->arrows_lamp_mode = arrows_mode;
	if ((int8_t)arrows_mode > 0)
	{
		lamps ^= 2u;
		SCREEN(screen_block)->arrows_lamp_timer = arrows_mode;
		PORT_OUTPUT_BITS(port_g)[0] = lamps;
		return;
	}
	if (arrows_mode == 0u)
	{
		lamps &= ~2u;
		PORT_OUTPUT_BITS(port_g)[0] = lamps;
		return;
	}
	lamps |= 2u;
	PORT_OUTPUT_BITS(port_g)[0] = lamps;
}

/**
 *    Name:        system/inputs.c
 *    Description: the controls: the switches and the analog inputs (wheel, pedals), each seat's inputs and the screens' new presses
 *    Version:     0.1.0
 *    Created:     12-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    OutRunners (arcade, Sega System Multi 32): the game's V70 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Reads the inputs in the VBLANK (int_vector_40; twice at start-up): the switches of both I/O chips
   (ports E and F: bits 0-1 the seats' service switches, 2-5 the coin switches, 7 a test switch) into
   switches, with previous_switches, switches_pressed and switches_released; then, on the turn of the
   screen whose task runs, half the analog inputs through the A/D converter (C00050 + 4 * index): screen
   1's turn seat 1's wheel, gas and brake and seat 2's wheel, screen 2's seat 2's gas and brake (the
   converter's second bank), each raw into analog_raw on and scaled to 0-FF between its calibration (a
   low and a high byte from 20E638, in the settings) into analog_inputs on; then the converter's bank for
   the other turn (C00060). */
void read_switches_and_analog(void)
{
	uint8_t chip_1_port_f_on = 0, chip_2_port_f_on = 0, chip_2_switches = 0, new_switches = 0, old_switches = 0, test_switch = 0;
	uint16_t calibration_high = 0, calibration_low = 0, reading = 0;
	uint32_t calibration = 0, converter = 0, inputs_left = 0, raw_place = 0, scaled_place = 0, screen_turn = 0;
	uint8_t chip_1_port_e = 0, chip_1_port_f = 0, chip_2_port_e = 0, chip_2_port_f = 0;
	uint16_t capped_reading = 0;

	chip_1_port_e = io_read8(IO_1_PORT_E);
	chip_1_port_f = io_read8(IO_1_PORT_F);
	chip_1_port_f_on = (uint8_t)~chip_1_port_f;
	new_switches = (uint8_t)(~chip_1_port_e | ((((uint32_t)chip_1_port_f_on << 4) | (chip_1_port_f_on >> 4)) & 3u));
	chip_2_port_e = io_read8(IO_2_PORT_E);
	chip_2_port_f = io_read8(IO_2_PORT_F);
	chip_2_port_f_on = (uint8_t)~chip_2_port_f;
	chip_2_switches = (uint8_t)(~chip_2_port_e | ((((uint32_t)chip_2_port_f_on << 4) | (chip_2_port_f_on >> 4)) & 3u));
	test_switch = (new_switches | chip_2_switches) & 2u;
	new_switches = (uint8_t)((new_switches & 0xDu) | ((((chip_2_switches & 0xCu) * 2u) | (chip_2_switches & 1u)) * 2u) | ((uint32_t)test_switch << 6) | (test_switch >> 2));
	old_switches = switches;
	switches = new_switches;
	previous_switches = old_switches;
	switches_pressed = ~old_switches & new_switches;
	switches_released = old_switches & ~new_switches;
	screen_turn = SCREEN(screen)->index;
	screen_turn = screen_turn & 1u;
	converter = ANALOG_INPUTS + screen_turn * 4u;
	raw_place = analog_raw_ADDRESS + screen_turn * 4u;
	scaled_place = analog_inputs_ADDRESS + screen_turn * 4u;
	inputs_left = analog_input_counts[screen_turn];
	calibration = analog_calibration_addresses[screen_turn];
	do
	{
		io_write8(converter, 0xFFu);
		reading = (uint16_t)(io_read8(converter) * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = (uint16_t)(reading * 2u);
		reading = (uint16_t)((reading & ~0xFFu) | io_read8(converter));
		reading = reading >> 7;
		ANALOG_VALUES(raw_place)[0] = (uint8_t)reading;
		calibration_low = ANALOG_CALIBRATION_BYTES(calibration)[0];
		calibration++;
		calibration_high = ANALOG_CALIBRATION_BYTES(calibration)[0];
		raw_place++;
		calibration++;
		if (calibration_high < reading)
			reading = (uint16_t)calibration_high;
		capped_reading = reading;
		reading = (uint16_t)(capped_reading - calibration_low);
		if ((int16_t)capped_reading < (int16_t)calibration_low)
			reading = 0u;
		ANALOG_VALUES(scaled_place)[0] = divide_u16((uint16_t)(reading * 0xFFu), (uint16_t)(calibration_high - calibration_low));
		inputs_left--;
		scaled_place++;
		converter += 2u;
	} while (inputs_left != 0u);
	io_write8(ANALOG_BANK, screen_turn ^ 1u);
}

/* Packs both seats' inputs for the coming frame (the VBLANK, on screen 1's turn) into their records: the
   screen's player + 1, the buttons (port A's high bits into 0-3, its low two into 4-5, the shift; port E
   bit 4 into bit 6, START), the wheel (analog_inputs inverted), the gas and the brake: seat_1_player to
   seat_1_brake and seat_2_player to seat_2_brake. On ISDN (isdn_mode) they go to the link board instead
   (link_send_seat_inputs). */
void pack_seat_inputs(void)
{
	uint8_t other_seat_buttons = 0, port_a_on = 0, seat_buttons = 0;
	uint32_t analog_place = 0, link_event_place = 0, link_place = 0;
	uint8_t chip_1_port_e = 0, chip_1_port_a = 0, chip_2_port_e = 0, chip_2_port_a = 0;

	chip_1_port_e = io_read8(IO_1_PORT_E);
	chip_1_port_a = io_read8(0xC00000u);
	port_a_on = (uint8_t)~chip_1_port_a;
	seat_buttons = ((~chip_1_port_e * 2u * 2u) & 0x40u) | ((((uint32_t)port_a_on << 4) | (port_a_on >> 4)) & 0x3Fu);
	chip_2_port_e = io_read8(IO_2_PORT_E);
	chip_2_port_a = io_read8(0xC80000u);
	port_a_on = (uint8_t)~chip_2_port_a;
	other_seat_buttons = ((~chip_2_port_e * 2u * 2u) & 0x40u) | ((((uint32_t)port_a_on << 4) | (port_a_on >> 4)) & 0x3Fu);
	analog_place = analog_inputs_ADDRESS;
	if (isdn_mode != 0u)
	{
		link_place = 0x800020u;
		link_event_place = 0x208040u;
		link_send_seat_inputs(seat_buttons, link_event_place, &analog_place, &link_place);
		link_event_place = 0x218040u;
		seat_buttons = (uint8_t)other_seat_buttons;
		link_send_seat_inputs(seat_buttons, link_event_place, &analog_place, &link_place);
		return;
	}
	seat_1_player = SCREEN(SCREEN_1)->player + 1u;
	seat_1_buttons = seat_buttons;
	seat_1_wheel = ~analog_inputs;
	analog_place++;
	seat_1_gas = analog_gas_1;
	analog_place++;
	seat_1_brake = analog_brake_1;
	seat_2_player = SCREEN(SCREEN_2)->player + 1u;
	seat_2_buttons = other_seat_buttons;
	analog_place++;
	seat_2_wheel = ~analog_wheel_2;
	analog_place++;
	seat_2_gas = analog_gas_2;
	seat_2_brake = analog_brake_2;
}

/* Never runs in the scenarios (ISDN only): writes a seat's five bytes as halfwords on the link board at
   R11 (800020 on): 0, the buttons (R0), the wheel (inverted), gas and brake from analog_inputs at R10;
   or, when its screen has a link_event (R2: screen + 40), the event and the four bytes after it, which
   it then clears (without moving R10 on, so the next seat would read this one's analog inputs). */
void link_send_seat_inputs(uint8_t seat_buttons, uint32_t link_event_place, uint32_t *p_analog_place, uint32_t *p_link_place)
{
	uint32_t analog_place = *p_analog_place;
	uint32_t link_place = *p_link_place;
	uint8_t wheel_input = 0, gas_input = 0, brake_input = 0;

	if (SCREEN(link_event_place - 0x40u)->link_event != 0u)
	{
		io_write16(link_place, SCREEN(link_event_place - 0x40u)->link_event);
		link_place += 2u;
		link_event_place++;
		io_write16(link_place, SCREEN(link_event_place - 0x41u)->link_event_data_1);
		link_place += 2u;
		link_event_place++;
		io_write16(link_place, SCREEN(link_event_place - 0x42u)->link_event_data_2);
		link_place += 2u;
		link_event_place++;
		io_write16(link_place, SCREEN(link_event_place - 0x43u)->link_event_data_3);
		link_place += 2u;
		link_event_place++;
		io_write16(link_place, SCREEN(link_event_place - 0x44u)->link_event_data_4);
		link_place += 2u;
		link_event_place = link_event_place + 1u - 4u;
		SCREEN(link_event_place - 0x41u)->link_event_data_1 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_2 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_3 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event_data_4 = 0u;
		SCREEN(link_event_place - 0x41u)->link_event = 0u;
	}
	else
	{
		io_write16(link_place, 0u);
		link_place += 2u;
		io_write16(link_place, seat_buttons);
		wheel_input = ANALOG_VALUES(analog_place)[0];
		analog_place++;
		link_place += 2u;
		io_write16(link_place, (uint8_t)~wheel_input);
		gas_input = ANALOG_VALUES(analog_place)[0];
		analog_place++;
		link_place += 2u;
		io_write16(link_place, gas_input);
		brake_input = ANALOG_VALUES(analog_place)[0];
		analog_place++;
		link_place += 2u;
		io_write16(link_place, brake_input);
		link_place += 2u;
	}
	*p_analog_place = analog_place;
	*p_link_place = link_place;
}

/* queue_screen_inputs for screen 1 (seat_1_buttons, the first I/O chip) and screen 2 (seat_2_buttons,
   the second): the VBLANK on screen 2's turn. */
void queue_seat_inputs(void)
{
	uint32_t seat_record = 0, seat_screen = 0;

	seat_screen = SCREEN_1;
	seat_record = seat_1_buttons_ADDRESS;
	r12 = 0xC00000u;
	queue_screen_inputs(seat_screen, seat_record);
	seat_screen = SCREEN_2;
	seat_record = seat_2_buttons_ADDRESS;
	r12 = 0xC80000u;
	queue_screen_inputs(seat_screen, seat_record);
}

/* Puts a screen's inputs into its ring of eight (screen + 130, at input_write_place): the seat's buttons
   (R11) with ~port B of its I/O chip (R12 + 2) in bits 8-15; and the record's wheel, gas and brake into
   the screen's wheel, gas and brake. On ISDN the record is the screen's race_copy's +2C (the inputs the
   link brought). */
void queue_screen_inputs(uint32_t seat_screen, uint32_t seat_record)
{
	uint32_t input_write_place = 0, inputs = 0, seat_inputs = 0;

	if (isdn_mode != 0u)
		seat_record = SCREEN(seat_screen)->race_copy + 0x2Cu;
	inputs = (uint8_t)~io_read8(r12 + 2u);
	seat_inputs = (SEAT_INPUTS(seat_record)->buttons | ((uint32_t)SEAT_INPUTS(seat_record)->wheel << 8u) | ((uint32_t)SEAT_INPUTS(seat_record)->gas << 16u) | ((uint32_t)SEAT_INPUTS(seat_record)->brake << 24u));
	input_write_place = SCREEN(seat_screen)->input_write_place;
	SCREEN(seat_screen)->input_ring[(int32_t)input_write_place] = (uint16_t)(inputs << 8) | (uint8_t)seat_inputs;
	SCREEN(seat_screen)->input_write_place = (input_write_place + 1u) & 7u;
	AS32(SCREEN(seat_screen)->wheel) = seat_inputs >> 8;
}

/* Takes the screen's inputs for the frame from its ring (input_read_place; both frame loops, before
   check_game_start): buttons, with previous_buttons, buttons_pressed and buttons_released (bit 6 START,
   4-5 the shift up and down, 0-3 the others, 8-15 port B). On ISDN also the link's status (race_copy
   link_status) into link_status, with link_status_set, link_status_cleared and link_status_changed, and,
   unless in it already, game mode 8 (game_mode_isdn_connection) while it says the line connects (bits
   2-3). */
uint32_t take_screen_inputs(void)
{
	uint8_t new_link_status = 0, old_link_status = 0, status_cleared = 0;
	uint32_t buttons = 0, buttons_place = 0, input_changes = 0, input_read_place = 0, inputs = 0;
	uint8_t new_status = 0;

	input_read_place = SCREEN(screen)->input_read_place;
	inputs = SCREEN(screen)->input_ring[(int32_t)input_read_place];
	SCREEN(screen)->input_read_place = (input_read_place + 1u) & 7u;
	buttons_place = screen + 0x110u;
	buttons = SCREEN(buttons_place - 0x110u)->buttons;
	SCREEN(buttons_place - 0x110u)->buttons = inputs;
	buttons_place += 4u;
	SCREEN(buttons_place - 0x114u)->previous_buttons = buttons;
	input_changes = ~buttons & inputs;
	buttons_place += 4u;
	SCREEN(buttons_place - 0x118u)->buttons_pressed = input_changes;
	SCREEN(buttons_place - 0x118u)->buttons_released = buttons & ~inputs;
	if (isdn_mode != 0u)
	{
		new_link_status = RACE(SCREEN(screen)->race_copy)->link_status;
		new_status = new_link_status;
		old_link_status = SCREEN(screen)->link_status;
		SCREEN(screen)->link_status = new_status;
		input_changes = (input_changes & ~0xFFu) | (uint8_t)~old_link_status;
		input_changes = input_changes & (new_link_status | 0xFFFFFF00u);
		SCREEN(screen)->link_status_set = input_changes;
		status_cleared = ~new_link_status & old_link_status;
		SCREEN(screen)->link_status_cleared = status_cleared;
		input_changes = input_changes | status_cleared;
		SCREEN(screen)->link_status_changed = input_changes;
		if (SCREEN(screen)->game_mode != 8u && (new_link_status & 0xCu) != 0u)
			AS32(SCREEN(screen)->game_mode) = 8u;
	}
	return input_changes;
}

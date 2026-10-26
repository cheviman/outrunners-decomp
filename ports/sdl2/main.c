/**
 *    Name:        main.c
 *    Description: the game in a window: SDL2 shows its two screens, plays its sound, reads the controls and
 *                 keeps the pace, around the same game and board the headless runs check
 *    Version:     0.1.0
 *    Created:     21-10-2026
 *    Modified:    25-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: orunners [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]
 *                    [--assets FILE]
 *
 *    The game (src/) and the board (runtime/) run as they are: this file only wraps them. Where the board
 *    has its hooks, the port does its part: when VBLANK starts the video draws both screens and the port
 *    puts them in the window, the left screen beside the right; when the board reads the controls the port
 *    hands it the keyboard and the game controllers; when the game finishes a frame the port queues the
 *    frame's sound and waits for the frame's time (60 a second) to pass. The EEPROM (the game's settings,
 *    bookkeeping) is kept in orunners.eeprom beside the executable.
 *
 *    The game's data is its assets pack (game/data.h): orunners.assets beside the executable, or --assets.
 *    The first run, with no pack there, makes it from the player's ROM set beside the executable (the
 *    folder rom/ or orunners.zip; game/extract.h).
 *
 *    --script plays a control script (runtime/include/s32/script.h) instead of the keyboard, to its end;
 *    --fast runs it without waiting for the frames' time. --hashes, --video-hashes and --sound-hashes
 *    write for each frame the hashes ports/headless and tools/core/system32/reference/s32ref write, so a script run in
 *    the window compares with the reference frame by frame.
 *
 *    Controls (the cabinet's player 1; player 2 on the second game controller):
 *      left, right arrows             the wheel (it comes back to the centre when let go)
 *      up arrow, down arrow           gas, brake
 *      Q, A                           shift up, shift down
 *      space                          DJ (the music)      [ ]    << and >> (the song)
 *      5, 6                           coins 1-2           1, 2   starts 1-2
 *      9                              service             F2     test (the operator's menu)
 *      Escape                         quit                F11 or Alt+Enter   full screen
 *      F3                             smooth or sharp pixels
 *    Game controllers: the left stick is the wheel, the right trigger the gas, the left trigger the brake,
 *    the right and left shoulders shift up and down, Y the DJ, the pad's left and right << and >>, Start
 *    the start, Back a coin.
 */
#include "s32/machine.h"
#include "s32/program.h"
#include "s32/script.h"
#include "s32/video.h"
#include "sound/chips.h"
#include "game/data.h"
#include "game/extract.h"

#include <SDL.h>

#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE               "OutRunners"
#define ASSETS_FILE         "orunners.assets"    /* the game's assets pack (tools/make_pack.py) */
#define PLAYERS             2
#define WINDOW_SCALE        2
#define QUEUE_FRAMES_MAX    6          /* sound queued beyond this many frames is dropped */
#define WHEEL_LEFT          0x10       /* the wheel's travel, as MAME has it */
#define WHEEL_CENTRE        0x80
#define WHEEL_RIGHT         0xf0
#define WHEEL_TURN          6          /* a frame, while a key holds it */
#define WHEEL_RETURN        8          /* a frame, back to the centre */
#define PEDAL_PRESS         0x20       /* a frame, while a key holds it */
#define PEDAL_LIFT          0x30
#define STICK_DEAD_ZONE     4000

#define USAGE "Usage: orunners [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]" \
	" [--assets FILE]"

static jmp_buf finished;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_AudioDeviceID audio;
static SDL_GameController *controllers[PLAYERS];
static bool smooth;
static uint64_t next_frame;           /* performance counter value the next frame waits for */
static char eeprom_path[1024];
static s32_script *script;
static bool fast;
static FILE *hashes;
static FILE *video_hashes;
static FILE *sound_hashes;

/* The wheels and pedals as the keyboard leaves them, frame to frame */
static int wheel[PLAYERS] = { WHEEL_CENTRE, WHEEL_CENTRE };
static int gas[PLAYERS];
static int brake[PLAYERS];

/* ---- files ---- */

static void save_eeprom(void)
{
	FILE *output;

	if (!s32_board.eeprom.dirty)
		return;
	output = fopen(eeprom_path, "wb");
	if (!output)
		return;
	fwrite(s32_board.eeprom.data, 1, sizeof(s32_board.eeprom.data), output);
	fclose(output);
	s32_board.eeprom.dirty = false;
}

static void load_eeprom(void)
{
	FILE *input = fopen(eeprom_path, "rb");
	uint16_t data[S32_EEPROM_WORDS];

	if (!input)
		return;
	if (fread(data, 1, sizeof(data), input) == sizeof(data))
		memcpy(s32_board.eeprom.data, data, sizeof(data));
	fclose(input);
}

/* ---- the picture ---- */

static void on_frame_shown(void)
{
	int widths[2];
	int left = 0;

	s32_video_draw_frame();
	for (int screen = 0; screen < 2; screen++)
	{
		const uint32_t *pixels = s32_video_frame(screen, &widths[screen]);
		SDL_Rect place;

		place.x = screen * S32_VIDEO_MAX_WIDTH;
		place.y = 0;
		place.w = widths[screen];
		place.h = S32_VIDEO_HEIGHT;
		SDL_UpdateTexture(texture, &place, pixels, S32_VIDEO_MAX_WIDTH * (int)sizeof(uint32_t));
	}
	SDL_RenderSetLogicalSize(renderer, widths[0] + widths[1], S32_VIDEO_HEIGHT);
	SDL_RenderClear(renderer);
	for (int screen = 0; screen < 2; screen++)
	{
		SDL_Rect source, target;

		source.x = screen * S32_VIDEO_MAX_WIDTH;
		source.y = 0;
		source.w = widths[screen];
		source.h = S32_VIDEO_HEIGHT;
		target = source;
		target.x = left;
		SDL_RenderCopy(renderer, texture, &source, &target);
		left += widths[screen];
	}
	SDL_RenderPresent(renderer);
}

static void toggle_full_screen(void)
{
	const bool full = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) == 0;

	SDL_SetWindowFullscreen(window, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

static void toggle_smooth(void)
{
	smooth = !smooth;
	SDL_SetTextureScaleMode(texture, smooth ? SDL_ScaleModeLinear : SDL_ScaleModeNearest);
}

/* ---- the controls ---- */

static void read_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				longjmp(finished, 1);
			case SDL_KEYDOWN:
				if (event.key.repeat)
					break;
				if (event.key.keysym.sym == SDLK_ESCAPE)
					longjmp(finished, 1);
				if (event.key.keysym.sym == SDLK_F11
					|| (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT)))
					toggle_full_screen();
				if (event.key.keysym.sym == SDLK_F3)
					toggle_smooth();
				break;
			case SDL_CONTROLLERDEVICEADDED:
				for (int player = 0; player < PLAYERS; player++)
					if (!controllers[player])
					{
						controllers[player] = SDL_GameControllerOpen(event.cdevice.which);
						break;
					}
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				for (int player = 0; player < PLAYERS; player++)
					if (controllers[player]
						&& SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[player])) == event.cdevice.which)
					{
						SDL_GameControllerClose(controllers[player]);
						controllers[player] = NULL;
					}
				break;
			default:
				break;
		}
	}
}

static int clamp(int value, int low, int high)
{
	return value < low ? low : (value > high ? high : value);
}

/* the keyboard's wheel: turned while a key holds it, back to the centre when let go */
static int steer(int position, bool left, bool right)
{
	if (left != right)
		return clamp(position + (right ? WHEEL_TURN : -WHEEL_TURN), WHEEL_LEFT, WHEEL_RIGHT);
	if (position < WHEEL_CENTRE)
		return position + WHEEL_RETURN > WHEEL_CENTRE ? WHEEL_CENTRE : position + WHEEL_RETURN;
	return position - WHEEL_RETURN < WHEEL_CENTRE ? WHEEL_CENTRE : position - WHEEL_RETURN;
}

static int press(int position, bool held)
{
	return clamp(position + (held ? PEDAL_PRESS : -PEDAL_LIFT), 0, 0xff);
}

/* a trigger's travel as a pedal's */
static int trigger_pedal(SDL_GameController *pad, SDL_GameControllerAxis axis)
{
	return SDL_GameControllerGetAxis(pad, axis) * 0xff / 32767;
}

/* the state of every control once a frame, where the board reads them (VBLANK) */
static void on_controls_update(void)
{
	const Uint8 *keys;
	s32_inputs *input = &s32_board.input;
	bool shift_up[PLAYERS] = { false }, shift_down[PLAYERS] = { false };
	bool music[PLAYERS] = { false }, previous[PLAYERS] = { false }, next[PLAYERS] = { false };
	bool coin[PLAYERS] = { false }, start[PLAYERS] = { false };
	int wheels[PLAYERS], gases[PLAYERS], brakes[PLAYERS];

	read_events();
	if (script)
	{
		s32_script_apply(script, s32_board.frame);
		return;
	}
	keys = SDL_GetKeyboardState(NULL);
	wheel[0] = steer(wheel[0], keys[SDL_SCANCODE_LEFT], keys[SDL_SCANCODE_RIGHT]);
	gas[0] = press(gas[0], keys[SDL_SCANCODE_UP]);
	brake[0] = press(brake[0], keys[SDL_SCANCODE_DOWN]);
	shift_up[0] = keys[SDL_SCANCODE_Q] != 0;
	shift_down[0] = keys[SDL_SCANCODE_A] != 0;
	music[0] = keys[SDL_SCANCODE_SPACE] != 0;
	previous[0] = keys[SDL_SCANCODE_LEFTBRACKET] != 0;
	next[0] = keys[SDL_SCANCODE_RIGHTBRACKET] != 0;
	for (int player = 0; player < PLAYERS; player++)
	{
		SDL_GameController *pad = controllers[player];

		coin[player] = keys[SDL_SCANCODE_5 + player] != 0;
		start[player] = keys[SDL_SCANCODE_1 + player] != 0;
		wheels[player] = wheel[player];
		gases[player] = gas[player];
		brakes[player] = brake[player];
		if (!pad)
			continue;
		/* the controller's stick and triggers, where they are moved, over the keyboard's */
		{
			const int stick = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_LEFTX);
			const int pad_gas = trigger_pedal(pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
			const int pad_brake = trigger_pedal(pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

			if (stick > STICK_DEAD_ZONE || stick < -STICK_DEAD_ZONE)
				wheels[player] = clamp(WHEEL_CENTRE + stick * (WHEEL_RIGHT - WHEEL_CENTRE) / 32767, WHEEL_LEFT, WHEEL_RIGHT);
			if (pad_gas > gases[player])
				gases[player] = pad_gas;
			if (pad_brake > brakes[player])
				brakes[player] = pad_brake;
		}
		shift_up[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) != 0;
		shift_down[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) != 0;
		music[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_Y) != 0;
		previous[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT) != 0;
		next[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) != 0;
		start[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_START) != 0;
		coin[player] |= SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_BACK) != 0;
	}

	/* each player's 315-5296 (chip 0 player 1, chip 1 player 2); its ports are active low: a cleared bit
	   is a pressed button */
	for (int player = 0; player < PLAYERS; player++)
	{
		input->port_a[player] = (uint8_t)(0xff & ~(shift_up[player] ? 0x01 : 0) & ~(shift_down[player] ? 0x02 : 0));
		input->port_b[player] = (uint8_t)(0xff & ~(music[player] ? 0x01 : 0) & ~(previous[player] ? 0x02 : 0)
			& ~(next[player] ? 0x04 : 0));
		input->port_e[player] = (uint8_t)(0xff & ~(coin[player] ? 0x04 : 0) & ~(start[player] ? 0x10 : 0));
	}
	if (keys[SDL_SCANCODE_9])
		input->port_e[0] &= (uint8_t)~0x01;
	if (keys[SDL_SCANCODE_F2])
		input->port_e[0] &= (uint8_t)~0x02;
	input->analog[0] = (uint8_t)wheels[0];
	input->analog[1] = (uint8_t)gases[0];
	input->analog[2] = (uint8_t)brakes[0];
	input->analog[3] = (uint8_t)wheels[1];
	input->analog[6] = (uint8_t)gases[1];
	input->analog[7] = (uint8_t)brakes[1];
}

/* ---- the frame ---- */

static void wait_for_frame(void)
{
	const uint64_t frequency = SDL_GetPerformanceFrequency();
	uint64_t now = SDL_GetPerformanceCounter();

	next_frame += frequency / S32_FRAME_RATE;
	if (now > next_frame + frequency / 4)
		next_frame = now;    /* far behind (the window was moved, the machine was busy): start over */
	while ((now = SDL_GetPerformanceCounter()) < next_frame)
	{
		const uint64_t left = (next_frame - now) * 1000 / frequency;
		if (left > 2)
			SDL_Delay((Uint32)(left - 1));
	}
}

static void on_frame_done(void)
{
	const uint64_t frame = program_frames() - 1;
	size_t count;
	const int16_t *mix;

	/* (in the headless runs' order: the board's hashes, then the sound) */
	if (hashes)
		fprintf(hashes, "%" PRIu64 " %016" PRIx64 "\n", frame, s32_state_hash());
	if (video_hashes)
		fprintf(video_hashes, "%" PRIu64 " %016" PRIx64 "\n", frame, s32_video_frame_hash());

	/* the sound board up to the frame's end, then the samples that start before it */
	if (s32_board.time_passes)
		s32_board.time_passes(s32_board.cycles);
	sound_chips_end_frame(s32_board.cycles);
	mix = sound_chips_frame_mix(&count);
	if (audio && !fast && SDL_GetQueuedAudioSize(audio) < sound_chips_mix_rate() / S32_FRAME_RATE * 4 * QUEUE_FRAMES_MAX)
		SDL_QueueAudio(audio, mix, (Uint32)(count * 2 * sizeof(int16_t)));
	if (sound_hashes)
		fprintf(sound_hashes, "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", frame, sound_chips_frame_hash(),
			sound_board_state_hash());
	if (script && s32_script_frames(script) && program_frames() >= s32_script_frames(script))
		program_stop();

	if (program_frames() % 600 == 0)
		save_eeprom();
	if (!fast)
		wait_for_frame();
}

static void on_irq_changed(bool asserted)
{
	program_irq_line(asserted);
}

static void on_log(const char *message)
{
	(void)message;
}

/* A window with what went wrong: the port has no console */
static int fail(const char *what, const char *detail)
{
	char message[1200];

	snprintf(message, sizeof(message), "%s\n%s", what, detail);
	fprintf(stderr, "%s\n", message);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITLE, message, NULL);
	return 1;
}

static FILE *open_hashes(const char *path)
{
	return path ? fopen(path, "w") : NULL;
}

int main(int argc, char **argv)
{
	SDL_AudioSpec wanted;
	const char *script_path = NULL;
	const char *hashes_path = NULL, *video_hashes_path = NULL, *sound_hashes_path = NULL;
	const char *assets_path = NULL;
	char beside[1024];
	char *base;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
		return fail("SDL could not start.", SDL_GetError());
	for (int index = 1; index < argc; index++)
	{
		if (strcmp(argv[index], "--script") == 0 && index + 1 < argc)
			script_path = argv[++index];
		else if (strcmp(argv[index], "--hashes") == 0 && index + 1 < argc)
			hashes_path = argv[++index];
		else if (strcmp(argv[index], "--video-hashes") == 0 && index + 1 < argc)
			video_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--sound-hashes") == 0 && index + 1 < argc)
			sound_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--fast") == 0)
			fast = true;
		else if (strcmp(argv[index], "--assets") == 0 && index + 1 < argc)
			assets_path = argv[++index];
		else
			return fail("Unknown option.", USAGE);
	}
	if (script_path)
	{
		char error[256];

		script = s32_script_load(script_path, error, sizeof(error));
		if (!script)
			return fail("The control script cannot be read.", error);
	}
	fast = fast && script;
	hashes = open_hashes(hashes_path);
	video_hashes = open_hashes(video_hashes_path);
	sound_hashes = open_hashes(sound_hashes_path);
	if ((hashes_path && !hashes) || (video_hashes_path && !video_hashes) || (sound_hashes_path && !sound_hashes))
		return fail("A hash file cannot be written.", hashes_path && !hashes ? hashes_path
			: (video_hashes_path && !video_hashes ? video_hashes_path : sound_hashes_path));

	base = SDL_GetBasePath();
	snprintf(eeprom_path, sizeof(eeprom_path), "%sorunners.eeprom", base ? base : "");
	if (!assets_path)
	{
		snprintf(beside, sizeof(beside), "%s" ASSETS_FILE, base ? base : "");
		assets_path = beside;
	}
	if (game_data_load(assets_path) == GAME_DATA_MISSING)
	{
		/* (the first run: the pack from the player's ROM set beside the executable) */
		static const char *const sources[] = { "rom", "orunners.zip" };
		const char *problem = NULL;
		char source[1024];

		for (size_t index = 0; index < sizeof(sources) / sizeof(sources[0]); index++)
		{
			snprintf(source, sizeof(source), "%s%s", base ? base : "", sources[index]);
			problem = game_data_extract(source, assets_path);
			if (!problem)
				break;
		}
		if (problem)
			return fail("The game's assets are missing, and no ROM set to make them from was found.", problem);
	}
	SDL_free(base);
	switch (game_data_load(assets_path))
	{
	case GAME_DATA_LOADED:
		break;
	case GAME_DATA_MISSING:
		return fail("The game's assets are missing.", assets_path);
	default:
		return fail("The game's assets cannot be read.", assets_path);
	}

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		2 * 320 * WINDOW_SCALE, S32_VIDEO_HEIGHT * WINDOW_SCALE, SDL_WINDOW_RESIZABLE);
	renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) : NULL;
	if (window && !renderer)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	texture = renderer ? SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		2 * S32_VIDEO_MAX_WIDTH, S32_VIDEO_HEIGHT) : NULL;
	if (!texture)
		return fail("The window could not be opened.", SDL_GetError());
	SDL_RenderSetIntegerScale(renderer, SDL_FALSE);
	SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);

	/* the mix at the YM3438's rate; SDL takes it to the device's */
	SDL_zero(wanted);
	wanted.freq = (int)sound_chips_mix_rate();
	wanted.format = AUDIO_S16SYS;
	wanted.channels = 2;
	wanted.samples = 1024;
	audio = SDL_OpenAudioDevice(NULL, 0, &wanted, NULL, 0);
	if (audio)
		SDL_PauseAudioDevice(audio, 0);

	/* the board, the video, the sound board and the game, as the headless runs set them up */
	s32_board.irq_changed = on_irq_changed;
	s32_board.controls_update = on_controls_update;
	s32_board.log = on_log;
	s32_board.frame_timing = true;
	s32_init(game_program);
	s32_video_init(game_tiles, sizeof(game_tiles), game_sprites, sizeof(game_sprites));
	s32_board.frame_shown = on_frame_shown;
	s32_board.sound_rom = game_sound;
	s32_board.pcm_rom = game_pcm;
	sound_chips_attach();
	sound_cpu_attach();
	if (!script)
		load_eeprom();
	program_reset();
	program_frame_done = on_frame_done;
	next_frame = SDL_GetPerformanceCounter();

	if (!setjmp(finished))
		program_run();

	if (!script)
		save_eeprom();
	if (hashes)
		fclose(hashes);
	if (video_hashes)
		fclose(video_hashes);
	if (sound_hashes)
		fclose(sound_hashes);
	if (audio)
		SDL_CloseAudioDevice(audio);
	SDL_Quit();
	return 0;
}

#include "sim_screen.h"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_keycode.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <stdint.h>
#include <wchar.h>

static SDL_Window   *g_win;
static SDL_Renderer *g_ren;
static SDL_Texture  *g_tex;
static uint32_t      g_buf[OLED_X_LENGTH * OLED_Y_LENGTH];

int
Sim_init (const char *title, int scale)
{
	if (SDL_Init (SDL_INIT_VIDEO))
		return -1;

	g_win = SDL_CreateWindow (title, SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          OLED_X_LENGTH * scale, OLED_Y_LENGTH * scale,
	                          (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
	if (!g_win)
		return -1;

	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_SetHint (SDL_HINT_RENDER_DRIVER, "software");
	g_ren = SDL_CreateRenderer (g_win, -1, SDL_RENDERER_SOFTWARE);
	if (!g_ren)
		return -1;

	SDL_RenderSetLogicalSize (g_ren, OLED_X_LENGTH, OLED_Y_LENGTH);

	g_tex = SDL_CreateTexture (g_ren, SDL_PIXELFORMAT_ABGR8888,
	                           SDL_TEXTUREACCESS_STREAMING, OLED_X_LENGTH,
	                           OLED_Y_LENGTH);
	if (!g_tex)
		return -1;

	return 0;
}

void
Sim_present (const uint8_t *buf)
{
	const uint32_t off_color = 0xFF000000;

	for (int page = 0; page < OLED_PAGES; ++page)
		{
			for (int x = 0; x < OLED_X_LENGTH; ++x)
				{
					uint8_t bits
					    = buf[page * OLED_RAWS + x];

					int y0 = page * 8;
					for (int b = 0; b < 8; ++b)
						{
							int y = y0 + b;
							g_buf[y * OLED_RAWS
							      + x]
							    = (bits & (1U << b)
							           ? 0xFFFFFFFF
							           : off_color);
						}
				}
		}

	SDL_UpdateTexture (g_tex, NULL, g_buf, OLED_RAWS * sizeof (uint32_t));
	SDL_RenderClear (g_ren);
	SDL_RenderCopy (g_ren, g_tex, NULL, NULL);
	SDL_RenderPresent (g_ren);
}

int
Sim_pump (void)
{
	SDL_Event e;
	while (SDL_PollEvent (&e))
		{
			if (e.type == SDL_QUIT)
				return 0;
			if (e.type == SDL_KEYDOWN
			    && e.key.keysym.sym == SDLK_ESCAPE)
				return 0;
		}

	return 1;
}

void
Sim_delay (uint32_t ms)
{
	SDL_Delay (ms);
}

void
Sim_shutdown ()
{
	if (g_tex)
		SDL_DestroyTexture (g_tex);
	if (g_ren)
		SDL_DestroyRenderer (g_ren);
	if (g_win)
		SDL_DestroyWindow (g_win);
	SDL_Quit ();
}

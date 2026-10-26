#include "OLED_Cdriver.h"
#include "SDL_timer.h"
#include "sim_screen.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int OLED_send (void *buf, size_t size);

int
main ()
{
	if (Sim_init ("SSD1306 128x64 Simulator", 2))
		exit (EXIT_FAILURE);

	uint32_t       x        = 0;
	const uint32_t frame_ms = 16;
	struct OLED    oled;
	uint8_t        oled_buf[OLED_RAWS * OLED_PAGES];

	OLED_init (&oled, NULL, 0, 128, 64, oled_buf, OLED_send);

	Sim_present (oled_buf);

	OLED_puts (&oled, "Hellow", 0, 0, 8);
	OLED_flush_all (&oled);
	Sim_delay (2000);

	while (Sim_pump ())
		{
			uint32_t t0 = SDL_GetTicks ();
			OLED_clear_all (&oled);
			OLED_puts (&oled,
			           "12345678900\nAaBbCcDdEe\n<>?|\\~!@#$%"
			           "^&*:()+",
			           0, 0, 8);
			OLED_fill_rect (&oled, 0, 56, x, 63, OLED_COLOR_MAIN);
			OLED_flush_all (&oled);
			x              = (x + 1) % OLED_RAWS;
			uint32_t spent = SDL_GetTicks () - t0;
			if (spent < frame_ms)
				Sim_delay (frame_ms - spent);
		}

	Sim_shutdown ();
	exit (EXIT_SUCCESS);
}

int
OLED_send (void *buf, size_t size)
{
	Sim_present (buf);

	return 0;
}

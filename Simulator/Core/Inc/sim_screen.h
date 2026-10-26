#ifndef SIM_SCREEN_H
#define SIM_SCREEN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define OLED_X_LENGTH 128U
#define OLED_Y_LENGTH 64U

#define OLED_RAWS  OLED_X_LENGTH
#define OLED_PAGES (OLED_Y_LENGTH / 8)

int  Sim_init (const char *title, int scale);
void Sim_present (const uint8_t *buf);
int  Sim_pump (void);
void Sim_delay (uint32_t ms);
void Sim_shutdown (void);

#ifdef __cplusplus
}
#endif

#endif

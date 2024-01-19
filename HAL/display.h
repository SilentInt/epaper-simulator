#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <SDL2/SDL.h>

void DISPLAY_Init(int width, int height, int rotate);
void DISPLAY_Loop(void (*main_entry)(void));
void renderScreen_bwr(uint8_t *screen_bw, uint8_t *screen_r);
void renderScreen_bw(uint8_t *screen_bw);
#endif

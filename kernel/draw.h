#ifndef __DRAW__H__
#define __DRAW__H__

#define COLOR_CYAN 0xB
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_WHITE 0xF
#define COLOR_BLACK 0X0


void drawBox(int x1, int x2, int y1, int y2, int z, int color);
void drawBootLogo(); //draw boot logo
void drawGraphic();
void delay(int a, int b);
void setupBoot();

int TEXT_HEIGHT = 0;

#endif
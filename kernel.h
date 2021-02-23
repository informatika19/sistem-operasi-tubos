#ifndef __KERNEL__H__
#define __KERNEL__H__

#define VID_MEMORY 0xB000
#define COLOR_CYAN 0xB
#define COLOR_DARK_GRAY 0x8

/* fungsi kernel yang akan diimplementasikan*/
void drawBootLogo(); //draw boot logo
void drawString(char *string); //draw string in memory 0xB000
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char* buffer, int length);

int TEXT_HEIGHT = 0;

#endif
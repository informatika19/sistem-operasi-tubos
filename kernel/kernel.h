#ifndef __KERNEL__H__
#define __KERNEL__H__

#define EMPTY 0x00
#define ENTER 0xD
#define ENDSTRING 0X0
#define BACKSPACE 0X8
#define VID_MEMORY 0xB000
#define COLOR_CYAN 0xB
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_WHITE 0xF
#define COLOR_BLACK 0X0
#define TRUE 1
#define FALSE 0
#define SECTOR_SIZE 512

/* fungsi kernel yang akan diimplementasikan*/
//MILESTONE 1
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char* buffer, int length);

//MILESTONE 2 TODO
void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
void readFile(char *buffer, char *path, int *result, char parentIndex);
void writeSector(char *buffer, int sector);
void readSector(char *buffer, int sector);

//ADDITIONAL FUNCTION
void drawString(char *string); //draw string in memory 0xB000
void drawBox(int x1, int x2, int y1, int y2, int z, int color);
void drawBootLogo(); //draw boot logo
void drawGraphic();
void delay(int a, int b);
void setupBoot();




#endif
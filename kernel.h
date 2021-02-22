#ifndef __KERNEL__H__
#define __KERNEL__H__

#define VID_MEMORY 0xB000

/* fungsi kernel yang akan diimplementasikan*/
void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char* string);
void readString(char* string);
void clear(char* buffer, int length);

#endif
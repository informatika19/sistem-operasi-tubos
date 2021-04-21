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

// Punya Tanur
// Macro for filesystem syscalls
// Filesytem location
#define NULL 0x00
#define MAP_SECTOR 0x100 // Map size -> 1 sector / 512 bytes
#define FILES_SECTOR 0x101 // Files size -> 2 sectors / 1024 bytes
#define SECTORS_SECTOR 0x103 // Sectors size -> 1 sector / 512 bytes

#define FILES_ENTRY_SIZE 0x10 // 16 bytes for 1 entry in files filesystem
#define SECTORS_ENTRY_SIZE 0x10 // 16 bytes for 1 entry in sectors filesystem
#define FILE_SIZE_MAXIMUM (SECTOR_SIZE*SECTORS_ENTRY_SIZE) // 16 sectors (8192 bytes) for 1 file entry
#define FILES_SECTOR_SIZE 2
#define FILES_ENTRY_COUNT (SECTOR_SIZE*FILES_SECTOR_SIZE/FILES_ENTRY_SIZE)
#define SECTORS_ENTRY_COUNT (SECTOR_SIZE/SECTORS_ENTRY_SIZE)
#define MAXIMUM_SECTOR_MAPPED 0x100 // Due 1 bytes representation in sectors filesystem

// "buffer" parameter in writeFile()
#define FOLDER NULL

// Predefined values in map filesystem
#define EMPTY_MAP_ENTRY 0x00 // For empty entry
#define FILLED_MAP_ENTRY 0xFF // If sector are filled

// Flags in files filesystem
#define ROOT_PARENT_FOLDER 0xFF // Flag for "P" byte
#define BIN_PARENT_FOLDER 0x00 // Flag for "P" byte
#define EMPTY_FILES_ENTRY 0xDE // Flag for "S" byte
#define FOLDER_ENTRY 0xFF // Flag for "S" byte
#define PARENT_BYTE_OFFSET 0x0 // "P" byte, parent folder index
#define ENTRY_BYTE_OFFSET 0x1 // "S" byte, entry index at sectors filesystem
#define PATHNAME_BYTE_OFFSET 0x2 // 14 bytes, filled with pathnames

// Predefined values in sectors filesystem
// Due sector 0x0-KERNEL_SECTOR_SIZE is filled with kernel and bootloader, we can use as flag
#define EMPTY_SECTORS_ENTRY 0x00 // For empty entry (ex. entry "00 00 00 00 ..." is empty-entry) // TODO : Extra, rename to _byte
#define FILLED_EMPTY_SECTORS_BYTE 0x01 // For empty bytes in non-empty entry (ex. entry "24 2F 22 01 01 01 01 01 ..." is non-empty entry)

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
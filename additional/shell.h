#define MAX_DIR 64
#define SECTOR_SIZE 512
#define MAX_FILENAME 14
#define ENTRY 0x10
#define SOFFSET 0x1
#define NAME_OFFSET 0x3
//Tabel Sectors
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

#define ROOT 0xFF
#define EMPTY_FILES 0xDE

void print(char *buff);
void readInput(char *input);
void ls(char *dir, char currentDir);
void getDir(char *dir);
// void cd(char *dir, char currentDir);
void cat(char *name, char currentDir);
void getCurrentDir(char *dir, char currentDir);
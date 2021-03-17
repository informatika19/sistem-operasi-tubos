#define MAX_DIR 64
#define SECTOR_SIZE 512
#define MAX_FILENAME 14
#define ENTRY 0x10
#define SOFFSET 0x1
#define NAME_OFFSET 0x2
//Tabel Sectors
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

#define ROOT 0xFF
#define EMPTY_FILES 0xDE

void print(char *buff);
void readInput(char *input, char *table, char currentDir, int *cursor);
void ls(char *dir, char currentDir);
void getDir(char *dir);
char cd(char *dir, char currentDir, char *argument);
void cat(char *name, char currentDir);
void getCurrentDir(char *dir, char currentDir);
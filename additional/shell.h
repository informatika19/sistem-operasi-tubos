#define MAX_DIR 64
#define SECTOR_SIZE 512
#define MAX_FILENAME 14
#define ENTRY 0x10
//Tabel Sectors
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

#define ROOT 0xFF
#define 

void print(char *buff);
void readInput(char *input);
void ls();
void getCurrDir(char *currdir);
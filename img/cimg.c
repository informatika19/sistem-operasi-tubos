#include <stdio.h>
#include <stdlib.h>

#define FILES_ENTRY "\xFF\xDE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
#define FILESYSTEM_SIZE 4
#define FILLED_MAP_ENTRY "\xFF"
#define EMPTY_MAP_ENTRY "\x00"
#define EMPTY_SECTORS_ENTRY "\x00"
#define SECTOR_SIZE 512

int main() {
    FILE *map = fopen("map.img", "wb");
    FILE *file = fopen("files.img", "wb");

    for (int i = 0; i < SECTOR_SIZE; i++) {
        if (i < 17 || (i >= 0x100 && i < 0x100 + FILESYSTEM_SIZE))
            fwrite(FILLED_MAP_ENTRY, 1, 1, map);
        else
            fwrite(EMPTY_MAP_ENTRY, 1, 1, map);
    }

    for (int i = 0; i < (SECTOR_SIZE/0x10 * 2); i++)
        fwrite(FILES_ENTRY, 0x10, 1, file);

    fclose(map);
    fclose(file);
    return 0;
}
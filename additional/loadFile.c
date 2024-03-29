#include <stdio.h>

#define SECTOR_SIZE 512
#define MAP_SECTOR 0x100
#define FILES_SECTOR1 0x101
#define FILES_SECTOR2 0x102
#define SECTORS_SECTOR 0x103

// Fungsi membaca sector file
void readSectorFile(char* buffer, FILE* file, int sector) {
    int i;
    fseek(file, sector * SECTOR_SIZE, SEEK_SET);
    for (i = 0; i < SECTOR_SIZE; ++i) {
        buffer[i] = fgetc(file);
    }
}

// Mengembalikan indeks entri yang free
int findFreeEntry(char* entry) {
    int i;

    for (i = 0; i < 32; ++i) {
        if (entry[i * 16 + 2] == '\0') {
            return i;
        }
    }

    return -1;
}

// Cari sector di map image yang kosong
int findFreeSector(char* map) {
    int i;

    for (i = 0; i < 512; ++i) { //TODO check edge case
        if (map[i] == 0x00) {
            return i;
        }
    }

    return -1;
}

int findS(char* sector) {
    int i;

    for (i = 0; i < 512; i+=16) {
        if (sector[i] == 0x00) {
            return i/16 - 1;
        }
    }

    return -1;
}

// Mengcopy isi source file ke system.img
void copySourceFile(FILE *source, int srcSector, FILE *destination, int destSector) {
    int i;
    fseek(source, srcSector * SECTOR_SIZE, SEEK_SET);
    fseek(destination, destSector * SECTOR_SIZE, SEEK_SET);
    
    for (i = 0; i < SECTOR_SIZE; ++i) {
        if (feof(source)) {
            fputc(0x00, destination);
        } else {
            fputc(fgetc(source), destination);
        }
    }
}

// Tulis dari buffer ke file
void writeSector (char *buffer, FILE *file, int sector) {
    int i;
    fseek(file, sector * SECTOR_SIZE, SEEK_SET);

    for (i = 0; i < SECTOR_SIZE; ++i) {
        fputc(buffer[i], file);
    }
}

// Tulis nama file
void writeName (char *entries, int index, char *name) {
    int i;

    for (i = 0; name[i] != '\0'; ++i) {
        entries[index * 16 + 2 + i] = name[i];
    }

    for (; i < 14; ++i) {
        entries[index * 16 + 2 + i] = '\0';
    }
}

int main(int argc, char* argv[]) {
    /*
    Alur Program loadFile:
    1. Membuka source file
    2. Membuka system image
    3. Load map sector, file sector, sectors sector
    4. Cari unused entry di directory
    5. Cari free sector dan tambahkan ke file
    6. Tulis map dan directory kembali ke system image 
    */

    if (argc > 2) {
        printf("Masukkan nama file yang akan di-load\n");
        return -1;
    }

    // Buka Source File
    FILE* source_file;
    source_file = fopen(argv[1], "r");
    if (source_file == 0) {
        printf("Tidak ada file yang ditemukan\n");
        return -1;
    }

    // Read and update system image
    FILE* system_img;
    system_img = fopen("system.img", "r+"); 
    if (system_img == 0) {
        printf("File system.img tidak ditemukan\n");
        return -1;
    }

    // Load map sector
    char map[SECTOR_SIZE];
    readSectorFile(map, system_img, MAP_SECTOR);

    // Load file sector 1
    char files1[SECTOR_SIZE];
    readSectorFile(files1, system_img, FILES_SECTOR1);

    // Load file sector 2
    char files2[SECTOR_SIZE];
    readSectorFile(files2, system_img, FILES_SECTOR2);

    // Load sectors sector
    char sector[SECTOR_SIZE];
    readSectorFile(sector, system_img, SECTORS_SECTOR);

    // Cari free entry di files
    int idx = findFreeEntry(files1);
    int fileSecNum = 1;

    // Jika tidak ada di files1, cek files2
    if (idx == -1) {
        idx = findFreeEntry(files2);
        fileSecNum = 2;
    }

    if (idx != -1) {
        int sectorCount = 0;
        int s;

        while (!feof(source_file)) {
            int sectorIdx = findFreeSector(map);
            if (sectorIdx != -1) {
                copySourceFile(source_file, sectorCount, system_img, sectorIdx);
                printf("%s telah di-copy ke sector %d\n", argv[1], sectorIdx);
                map[sectorIdx] = 0xFF;
                sector[idx * 16 + sectorCount] = sectorIdx;
                ++sectorCount;
            } else {
                printf("Tidak bisa load file lagi\n");
                return -1;
            }
        }

        s = findS(sector);

        if (fileSecNum == 1 && s != -1) {
            files1[idx * 16] = 0xFF;
            files1[idx * 16 + 1] = s;
            writeName(files1, idx, argv[1]);
            writeSector(files1, system_img, FILES_SECTOR1);
        
        } else if (fileSecNum == 2 && s != -1) {
            files2[idx * 16] = 0xFF;
            files2[idx * 16 + 1] = s;
            writeName(files2, idx, argv[1]);
            writeSector(files2, system_img, FILES_SECTOR2);
        }

        writeSector(map, system_img, MAP_SECTOR);
        writeSector(sector, system_img, SECTORS_SECTOR);
    } 

    fclose(source_file);
    fclose(system_img);
}
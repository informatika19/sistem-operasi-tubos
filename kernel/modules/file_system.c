#include "../kernel.h"


//MILESTONE 2
void copySegmentSectorSectors(char *dest, char *src, int n) {
    int i = 0;
    while (i < n) {
        dest[i] = src[i];
        i++;
    }
}

void readSector(char *buffer, int sector){
    interrupt(0x13,0x02*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13,0x03*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void readFile(char *buffer, char *path, int *result, char parentIndex){
	char sectorFiles[SECTOR_SIZE*2], sectorSectors[SECTOR_SIZE], segmentSectorSectors[SECTOR_SIZE], fileNameBuffer[14], idxEntrySectors, idxSegmentTarget;

	int i, j, fileFound;
	fileFound = 0;//belum ditemukan

	//read sector
	readSector(sectorFiles, 0x101);
	readSector(sectorFiles + SECTOR_SIZE, 0x102);

	//cek panjang filename valid atau tidak
	if (strlen(path) <= 14)
	{
		//cari filename pada sector files
		for (i = 0; i < (SECTOR_SIZE*2); i+=16)
		{
			if (sectorFiles[i] == parentIndex)//cari file yang index 0(P) == parentIndex
			{
				clear(fileNameBuffer,14);//clear buffer filename
				strncpy(fileNameBuffer,sectorFiles+i+2,14); //copy se
				if (strcmp(fileNameBuffer,path) == 0)//true
				{
					fileFound = 1;//file ditemukan
					idxEntrySectors = sectorFiles[i+1];//index S
					break;
				}
			}	
		}

		//jika file ditemukan copy dari sector sectors ke buffer
		if (fileFound == 1)
		{
			readSector(sectorSectors, 0x103);
			idxSegmentTarget = sectorSectors[idxEntrySectors*16];//idx 1 segment pada sector Sectors
			for (j = 0; j < 16 && sectorSectors[idxEntrySectors*16 + j] != 0x00; j++)
			{
				clear(segmentSectorSectors,SECTOR_SIZE);
				readSector(segmentSectorSectors, idxSegmentTarget);
            	copySegmentSectorSectors((buffer+j*SECTOR_SIZE), segmentSectorSectors, SECTOR_SIZE);//copy segment tersebut ke buffer
			    idxSegmentTarget = sectorSectors[idxEntrySectors*16 + j];//idx 1 segment pada sector Sectors
			}
			*result = 1;//File ditemukan (readFile)
		}

		//file tidak ditemukan
		else
		{
			*result = -1;//File tidak ditemukan (readFile)
		}
	}else{//filename tidak valid(lebih dari 14 byte)
		*result = -1;//File tidak ditemukan (readFile)
	}
}


void writeFile(char *buffer, char *path, int *sectors, char parentIndex){
    char map[512];
    char files[512*2];
    char sector[512];
    char fileNameBuffer[14];
    char currentParent;
    int iterfiles,itermap1,itermap2, itersector, iterpath,iterpath1;
    int fileFound,i,j,foundPath;

    // baca semua yuk baca
    readSector(map, 0x100);
    readSector(files, 0x101);
    readSector(files+512, 0x102);
    readSector(sector, 0x103);

    //cek dir kosonk
    iterfiles = 0;
    while (files[iterfiles*16+1] != '\0' && iterfiles < 64){
        iterfiles++;
    }

    if (iterfiles == 64){
        printString("There's no empty directory");
        *sectors = -2;
        return;
    }
    else{
    //cek jumlah sektor di map
    itermap1 = 0;
    while (itermap1 < 256 && map[itermap1] != 0x00){
        itermap1++;
    }
    if (itermap1 == 255){
        printString("no, no empty sector");
        *sectors = -3;
        return;
    }
    else{
        // cari path untuk file
        foundPath =0; iterpath =0; currentParent = parentIndex;
        while (!foundPath && iterpath <=  64){
            if (files[iterpath*16] == currentParent){
                foundPath = 1;
            }
            else{++iterpath;}
        }
        if (!foundPath) {*sectors = -4;
        return;}else{
        //cari filename pada sector files
            fileFound =0;
            for (i = 0; i < (512*2); i+=16)
            {
                if (files[i] == parentIndex)//cari file yang index 0(P) == parentIndex
                {
                    clear(fileNameBuffer,14);//clear buffer filename
                    strncpy(fileNameBuffer,files[i+2],14); //copy se
                    if (strcmp(fileNameBuffer,path) == 0)//true
                    {
                        fileFound = 1;//file ditemukan
                        break;
                    }
                }	
            }
            if (fileFound == 1){
                *sectors = -1;
                return;
            }
            else{ 
            //isi sektor dengan nama file
	        j = 0;
	        while (path[j] != '\0') {
		        files[iterfiles * 16 + 2 + j] = path[j];
                ++j;
	        }
            }
        while (buffer[itersector * 512] != '\0') { // cek semua buffer
        //cek yg kosong di map
        itermap2 = 0;
        while (itermap2 < 256 && map[itermap2] != 0x00){
            itermap2++;
        }
        if (itermap2 == 255){
            printString("no, no empty sector");
            *sectors = -3;
            return;
        }
        //bersihin sector
        clear(buffer+itersector*512, 512);
        // isi sektor terus tandain
		writeSector(buffer + itersector * 512, itermap2);
        map[itermap2] = 0xFF;
		sectors[itermap1 * 16 + itersector] = itermap2;
		++itersector;
	    }
         //write semua
        writeSector(map, 0x100);
        writeSector(files, 0x101);
        writeSector(files+512, 0x102);
        writeSector(sector, 0x103);
        *sectors = 0;
        }
    }
    }
}

void executeProgram(char *filename, int segment, int *success, char parentIndex) {
    // Buat buffer
    int isSuccess;
    char fileBuffer[512 * 16];
    // Buka file dengan readFile
    readFile(&fileBuffer, filename, &isSuccess, parentIndex);
    // If success, salin dengan putInMemory
    if (isSuccess == 1) {
        int i = 0;
        // launchProgram
        for (i = 0; i < 512*16; i++) {
        putInMemory(segment, i, fileBuffer[i]);}
        launchProgram(segment);
    } else {
        interrupt(0x21, 0, "File not found!", 0,0);
    }
}

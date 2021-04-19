/*
Made by TubOS
1. Pratama Andiko           13519112
2. Fabian Savero Diaz P.    13519140
3. Fadel Ananda D.          13519146
4. Alvin Rizqi A.           13519126
*/

#include "kernel.h"
int main()
{
    int y = 1;
    char string[1024];
    setupBoot(); // menuliskan logo ke layar (bonus)
    interrupt(0x10, 0x0003, 0, 0, 0);//for debug purpose

    makeInterrupt21();

    printString("Masukan Command:\n");
    while (1)
    {
        // int isSame;
        interrupt(0x10, 0x0200, 0, 0, 0x100 * y | 0X0);
        interrupt(0x21, 1, string, 0, 0);
        y++;
        if(strcmp(string, "shell") == 0){
            printString("hello");
        } else if(strcmp(string, "restart") == 0){
            return;
        }
        // interrupt(0x10, 0x0200, 0, 0, 0x100 * y | 0X0);
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    char AL, AH;
    AL = (char) (AX);
    AH = (char) (AX>>8);
    switch (AL)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x01:
        readString(BX);
        break;
    case 0x02:
        readSector(BX, CX);
        break;
    case 0x03:
        writeSector(BX, CX);
        break;
    case 0x04:
        readFile(BX, CX, DX, AH);
        break;
    case 0x05:
        writeFile(BX, CX, DX, AH);
        break;
    default:
        printString("Invalid interrupt");
    }
}






//MILESTONE 2 TODO



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




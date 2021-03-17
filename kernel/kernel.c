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
    //setupBoot(); // menuliskan logo ke layar (bonus)
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
            initShell();
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
    // case 0x03:
    //     writeSector(BX, CX);
    //     break;
    // case 0x04:
    //     readFile(BX, CX, DX, AH);
    //     break;
    // case 0x05:
    //     writeFile(BX, CX, DX, AH);
    //     break;
    default:
        printString("Invalid interrupt");
    }
}


//MILESTONE 1
void drawString(char *string) //menggambar di address memori VGA
{
    int i = 0;
    int TEXT_LENGTH = 0;
    while (string[i] != '\0')
    {
        putInMemory(VID_MEMORY, 0x8000 + (80 * TEXT_HEIGHT + TEXT_LENGTH) * 2, string[i]);
        putInMemory(VID_MEMORY, 0x8001 + (80 * TEXT_HEIGHT + TEXT_LENGTH) * 2, COLOR_CYAN);
        i++;
        TEXT_LENGTH++;
    }
    TEXT_HEIGHT++;
}

void printString(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        int CH = string[i];
        interrupt(0x10, 0xe * 256 + CH, 0, 0, 0); //interrupt untuk menampilkan char ke layar
        i++;
    }
}

void readString(char *string)
{
    int count = 0, input;
    while (1)
    {
        input = interrupt(0x16, 0, 0, 0, 0);
        if (input == 0xD) // 13 adalah simbol ascii tombol enter
        {
            string[count] = '\0';
            return; //keluar dari loop
        }
        else if (input == 8) // 8 adalah simbol ascii tombol backspace
        {
            //agar cursor mundur ke kiri
            interrupt(0x10, 0xe * 256 + 8, 0, 0, 0); //backspace
            interrupt(0x10, 0xe * 256 + 0, 0, 0, 0); //menghapus input saat itu(diganti null)
            interrupt(0x10, 0xe * 256 + 8, 0, 0, 0); //kembali ke backspace
            count--;
        }
        else
        {
            string[count] = input;                       //input dari keyboard
            interrupt(0x10, 0xe * 256 + input, 0, 0, 0); // menampilkan ke layar hasil inputan
            count++;
        }
    }
}

void clear(char *buffer, int length)
{
    int i = 0;

    /* Mengganti isi buffer 
    dengan empty yang sama dengan 0 */
    while (i < length)
    {
        buffer[i] = EMPTY;
        i++;
    }
}

//MILESTONE 2 TODO

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
				strncpy(fileNameBuffer,sectorFiles+2,14); //copy se
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
			*result = 1;//File tidak ditemukan (readFile)
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

//ADDITIONAL FUNCTION
void setupBoot()
{
    drawBootLogo(); //draw ASCII art
    interrupt(0x10, 0x0013, 0, 0, 0); //reset screen
    drawGraphic();
    interrupt(0x10, 0x0003, 0, 0, 0);
}

void drawBootLogo() //Bonus ASCII art
{
    interrupt(0x10, 0x0003, 0, 0, 0); //reset screen
    interrupt(0x10, 0x0100, 0, 0x2607, 0); //invisible cursor
    drawString("Now Loading...");
    drawString(" ");
    drawString("            $$$$$$$$\\        $$\\        $$$$$$\\   $$$$$$\\  ");
    drawString("            \\__$$  __|       $$ |      $$  __$$\\ $$  __$$\\ ");
    drawString("               $$ |$$\\   $$\\ $$$$$$$\\  $$ /  $$ |$$ /  \\__|");
    drawString("               $$ |$$ |  $$ |$$  __$$\\ $$ |  $$ |\\$$$$$$\\  ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ | \\____$$\\ ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$\\   $$ |");
    drawString("               $$ |\\$$$$$$  |$$$$$$$  | $$$$$$  |\\$$$$$$  |");
    drawString("               \\__| \\______/ \\_______/  \\______/  \\______/ ");
    delay(5000, 5000); //delay
    TEXT_HEIGHT = 0; //mengeset text height jadi 0
}

void drawGraphic() //BONUS draw pixel
{
    drawBox(40, 279, 25, 174, 40, COLOR_DARK_GRAY);
    delay(2500, 2500);
    drawBox(80, 239, 50, 149, 80, COLOR_LIGHT_GRAY);
    delay(2500, 2500);
    drawBox(120, 199, 75, 124, 120, COLOR_WHITE);    
    delay(2500, 2500);
    drawBox(130, 155, 80, 97, 130, COLOR_BLACK);
    delay(2000, 2000);
    drawBox(130, 155, 102, 119, 130, COLOR_BLACK);  
    delay(2000, 2000);
    drawBox(164, 189, 102, 119, 164, COLOR_BLACK);     
    delay(2000, 2000);
    drawBox(164, 189, 80, 97, 164, COLOR_BLACK);         
    delay(2500, 2500);                                
}

void drawBox(int x1, int x2, int y1, int y2, int z, int color){
    while(y1<y2){
        while(x1<x2){
            interrupt(0x10, 0x0C00|color, 0, x1, y1);
            x1++;
        }
        x1=z;
        y1++;
    }
}

void delay(int a, int b){
    int i=0;
    int j=0;
    while(i<a){
        j=0;
        while(j<b){
            j++;
        }
        i++;
    }
}


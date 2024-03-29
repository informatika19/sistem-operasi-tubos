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
    int return1;
    setupBoot(); // menuliskan logo ke layar (bonus)
    interrupt(0x10, 0x0003, 0, 0, 0);//for debug purpose

    makeInterrupt21();

    printString("Masukan Command:\n");
    executeProgram("tes", 0x2000, &return1, 0xFF);
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
    case 0x06:
        executeProgram(BX, CX, DX, AH);
        break;
    default:
        printString("Invalid interrupt");
    }
}





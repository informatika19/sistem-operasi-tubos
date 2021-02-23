/*
Made by TubOS
1. Pratama Andiko           13519112
2. Fabian Savero Diaz P.    13519140
3. Fadel Ananda D.          13519146
*/

#include "kernel.h"

int main()
{
    interrupt(0x10, 0x0003, 0, 0, 0);
    drawBootLogo();
    delay();
    interrupt(0x10, 0x0003, 0, 0, 0);

    makeInterrupt21();

    while (1)
        interrupt(0x21, 1, "shell\0", 0x2000, 0);
        ;
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    switch (AX)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

void drawString(char *string)
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
        interrupt(0x10, 0xe * 256 + CH, 0, 0, 0);
        i++;
    }
}

void readString(char *string)
{
    int count = 0;
    while (1) //1/true agar loop terus
    {
        int input = interrupt(0x16, 0, 0, 0, 0);
        if (input == 13) // 13 adalah simbol ascii tombol enter
        {
            string[count] = 0x0;
            string[count + 1] = 10;
            string[count + 2] = 13;
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

void drawBootLogo()
{
    drawString("            $$$$$$$$\\        $$\\        $$$$$$\\   $$$$$$\\  ");
    drawString("            \\__$$  __|       $$ |      $$  __$$\\ $$  __$$\\ ");
    drawString("               $$ |$$\\   $$\\ $$$$$$$\\  $$ /  $$ |$$ /  \\__|");
    drawString("               $$ |$$ |  $$ |$$  __$$\\ $$ |  $$ |\\$$$$$$\\  ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ | \\____$$\\ ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$\\   $$ |");
    drawString("               $$ |\\$$$$$$  |$$$$$$$  | $$$$$$  |\\$$$$$$  |");
    drawString("               \\__| \\______/ \\_______/  \\______/  \\______/ ");
};

void delay(){
    int i=32767;
    int j=32767;
    int k=32767;
    int l=2048;
    while(i>0){
        if (mod(j,4)==0){
            if(mod(k,6)==0){
                if(mod(l,6)==0){
                    i--;
                }
                l--;        
            }
            k--;
        }
        j--;
    } 
}

int mod(int x, int y){
    return(x-y*(x/y));
}
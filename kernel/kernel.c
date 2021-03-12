/*
Made by TubOS
1. Pratama Andiko           13519112
2. Fabian Savero Diaz P.    13519140
3. Fadel Ananda D.          13519146
*/

#include "kernel.h"

int main()
{
    int y = 1;
    char *string;
    setupBoot(); // menuliskan logo ke layar (bonus)

    makeInterrupt21();

    printString("Masukan Command:\n");
    while (1)
    {
        interrupt(0x10, 0x0200, 0, 0, 0x100 * y | 0X0);
        interrupt(0x21, 1, string, 0, 0);
        y++;
        interrupt(0x10, 0x0200, 0, 0, 0x100 * y | 0X0);
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
    case 0x1:
        readString(BX);
        break;
    //TODO
    // case 0x2:
    //     readSector(BX, CX);
    //     break;
    // case 0x03:
    //     writeSector(BX, CX);
    //     break;
    // case 0x04:
    //     readFile(BX, CX, DX, AH);
    //     break;
    // case 0x5:
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

//MILESTONE 2 TODO

void readSector(char *buffer, int sector){
    interrupt(0x13,0x02*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13,0x03*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void readFile(char *buffer, char *path, int *result, char parentIndex){
    // /* Read in the directory sector */
    // readSector(buffer, 2);  
}


void writeFile(char *buffer, char *path, int *sectors, char parentIndex){

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

int mod(int x, int y)
{
    return (x - y * (x / y));
}

int div(int x, int y)
{
    return x/y;
}
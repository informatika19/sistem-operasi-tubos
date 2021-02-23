/*
Made by TubOS
1. Pratama Andiko           13519112
2. Fabian Savero Diaz P.    13519140
3. Fadel Ananda D.          13519146
*/

#include "kernel.h"

int main(){
    drawBootLogo();

    makeInterrupt21();

    while (1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
    drawString("haha");
    switch(AX) {
        case 0x0:
            printString(BX);
            break;
        // case 0x1:
        //     readString(BX);
        //     break;
        default:
            printString("Invalid interrupt");
    }
}

void drawString(char *string){
    int i=0;
    int TEXT_LENGTH=0;
    while (string[i]!='\0'){
        putInMemory(VID_MEMORY, 0x8000+ (80*TEXT_HEIGHT+TEXT_LENGTH)*2, string[i]);
        putInMemory(VID_MEMORY, 0x8001+ (80*TEXT_HEIGHT+TEXT_LENGTH)*2, COLOR_CYAN);
        i++;
        TEXT_LENGTH++;
    }
    TEXT_HEIGHT++;
}

void printString(char *string){
    int i=0;
    while(string[i] != '\0'){
        int CH=string[i];
        interrupt(0x10, 0xe*256+CH, 0, 0, 0);
        i++;
    }
}

// void readString(char *string){

// }

// void clear(char* buffer, int length){
    
// }

void drawBootLogo(){
    drawString("            $$$$$$$$\\        $$\\        $$$$$$\\   $$$$$$\\  ");
    drawString("            \\__$$  __|       $$ |      $$  __$$\\ $$  __$$\\ ");
    drawString("               $$ |$$\\   $$\\ $$$$$$$\\  $$ /  $$ |$$ /  \\__|");
    drawString("               $$ |$$ |  $$ |$$  __$$\\ $$ |  $$ |\\$$$$$$\\  ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ | \\____$$\\ ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$\\   $$ |");
    drawString("               $$ |\\$$$$$$  |$$$$$$$  | $$$$$$  |\\$$$$$$  |");
    drawString("               \\__| \\______/ \\_______/  \\______/  \\______/ ");
};
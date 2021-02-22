/*
Made by TubOS
1. Pratama Andiko           13519112
2. Fabian Savero Diaz P.    13519140
3. Fadel Ananda D.          13519146
*/

void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char* string);
void readString(char* string);
void clear(char* buffer, int length);

int main(){
    putInMemory(0xB000, 0x8000, 'H');
    putInMemory(0xB000, 0x8001, 0xD);
    putInMemory(0XB000, 0X8002, 'A');
    putInMemory(0XB000, 0X8003, 0xD);

    while (1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
    switch(AX) {
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

void printString(char* string){

}

void readString(char* string){

}

void clear(char* buffer, int length){
    
}
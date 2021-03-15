#include "shell.h"

int cursorRow = 1;
void initShell(){
    char input[128];
    // int y = 1;
    interrupt(0x10, 0x0003, 0, 0, 0);
    print("\r[simple shell]\n");
    // print("\r>");
    while(1){
        interrupt(0x10, 0x0200, 0, 0, 0x100 * cursorRow | 0X0);
        interrupt(0x21, 1, input, 0, 0);
        readInput(input);
        cursorRow++;
    }
}

void print(char *buff){
    interrupt(0x21, 0, buff, 0, 0);
}

void readInput(char *input){
    // print(input, &cursorRow);
    //TODO cd, cat, ls, ln
    // int i;
    // strCompare(input, "ls", &i);
    
    if(strcmp(input, "ls") == 0){
        ls();
    } else if (strcmp(input, "cd") == 0){

    } else if (strcmp(input, "cat") == 0){

    } else if (strcmp(input, "ln") == 0){
        
    }
}

void ls(){
    
}

void getDir(char *dir){
    interrupt(0x21, 0x0002, dir, FILES_SECTOR, 0);
    interrupt(0x21, 0x0002, dir+SECTOR_SIZE, FILES_SECTOR+1, 0);
}
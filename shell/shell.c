#include "shell.h"

void initShell(){
    char input[1024];
    int y = 1;
    interrupt(0x10, 0x0003, 0, 0, 0);
    print("\r[simple shell]\n");
    // print("\r>");
    while(1){
        interrupt(0x10, 0x0200, 0, 0, 0x100 * y | 0X0);
        interrupt(0x21, 1, input, 0, 0);
        readInput(input);
        y++;
    }
}

void print(char *buff){
    interrupt(0x21, 0, buff, 0, 0);
}

void readInput(char *input){
    //TODO cd, cat, ls, ln
    int i;
    strCompare(input, "ls", &i);
    if(i == 1){
        
    }
}
#include "shell.h"

int cursorRow = 2;
void initShell(){
    char filesSector[2][SECTOR_SIZE]; 
    char currentDir = ROOT;
    
    char input[128];

    getDir(filesSector);
    // int y = 1;
    interrupt(0x10, 0x0003, 0, 0, 0);
    print("\r[Shell]\n");
    // print("\r>");
    while(1){
        getCurrentDir(filesSector, currentDir);
        interrupt(0x10, 0x0200, 0, 0, 0x100 * cursorRow | 0X0);
        interrupt(0x21, 1, input, 0, 0);
        readInput(input);
        print("\n");
        cursorRow+=3;
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
    char command[16];
    clear(command, 16);
    strncopy(input, command, ' ');
    // print("\n"); debug
    // print(command);
    if(strcmp(command, "ls") == 0){
        //ls(filesSector, currentDir);
        print("\r\nls");
    } else if (strcmp(command, "cd") == 0){
        //cd(filesSector, &currentDir)
        print("\r\ncd");
    } else if (strcmp(command, "cat") == 0){
        print("\r\ncat");
    } else if (strcmp(command, "ln") == 0){
        print("\r\nln");
    } else{
        print("\r\nInvalid Command");
    }
}

void ls(char *dir, char currentDir){
    int i;

    char name[MAX_FILENAME];
    char p, s;
    
    for(i=0; i<MAX_DIR; i++){
        p = dir[ENTRY*i];
        s = dir[ENTRY*i + SOFFSET];
        if(p == currentDir && s!=EMPTY_FILES){
            strncpy(name, dir+ENTRY*i+NAME_OFFSET, MAX_FILENAME);
            print(name);
            print(" | ");
        }
    }
    print("\n");
}

// void cd(char *dir, char *currentDir, char *arg){
//     int i;
// }

void cat(char *name, char currentDir){
    char content[8192];
    int returnRead;
    readFile(content, name, &returnRead, currentDir);
    if (returnRead == -1){
        print("file not found");
    } else{
        print(content);
    }
}

void getDir(char *dir){
    interrupt(0x21, 0x0002, dir, FILES_SECTOR, 0);
    interrupt(0x21, 0x0002, dir+SECTOR_SIZE, FILES_SECTOR+1, 0);
}

void getCurrentDir(char *dir, char currentDir){
    print("\r");
    if(currentDir == ROOT)
        print("root");
    else
        print(dir+currentDir*16+NAME_OFFSET);
}
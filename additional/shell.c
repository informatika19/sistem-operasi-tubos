#include "shell.h"

int cursorRow = 2;
void initShell(){ //Shell Init
    char filesSector[2][SECTOR_SIZE]; //contains filesystem files
    char currentDir = ROOT; //current direcory (default root)
    
    char input[128]; //input command

    getDir(filesSector); //readsector then assign it to filesSector
    // int y = 1;
    interrupt(0x10, 0x0003, 0, 0, 0); //clearscreen
    print("\r[Shell]\n");
    // print("\r>");
    while(1){ //shell main program
        //getCurrentDir(filesSector, currentDir); //get current directory
        interrupt(0x10, 0x0200, 0, 0, 0x100 * cursorRow | 0X0);
        interrupt(0x21, 1, input, 0, 0);
        readInput(input, filesSector, currentDir, &cursorRow); //read user input then parse
        print("\n");
        // cursorRow+=3;
    }
}

void print(char *buff){
    interrupt(0x21, 0, buff, 0, 0);
}

void readInput(char *input, char *table, char currentDir, int *cursor){
    // print(input, &cursorRow);
    //TODO cd, cat, ls, ln
    // int i;
    // strCompare(input, "ls", &i);
    char command[16];
    clear(command, 16);
    strncopy(input, command, ' '); //split string and copy string before space
    // print("\n"); debug
    // print(command);
    if(strcmp(command, "ls") == 0){ //if user input ls
        ls(table, ROOT);
        //print("\r\nfolder");
        *cursor = *cursor + 10;
    } else if (strcmp(command, "cd") == 0){//if user input cd
        //cd(filesSector, &currentDir)
        print("\r\ncd");
    } else if (strcmp(command, "cat") == 0){//if user input cat
        print("\r\ncat");
    } else if (strcmp(command, "ln") == 0){//if user input ln
        print("\r\nln");
    } else{
        print("\r\nInvalid Command");
    }
}

void ls(char *dir, char currentDir){ //ls function
    int i;

    char name[MAX_FILENAME]; //name variable
    char p, s;//parent and sector variable
    print("dir :\n");
    print(dir);
    print("\r\nsebelum for loop");
    print("\n");
    for(i=0; i<MAX_DIR; i++){//looping through sectors
        p = dir[ENTRY*i];
        s = dir[ENTRY*i + SOFFSET];
        if(p == currentDir){
            if (i==0)
                print("\r");
            strncpy(name, dir+ENTRY*i+NAME_OFFSET, MAX_FILENAME);
            print(name);
            print(" | ");
        }
    }
    print("\r\nsetelah for loop");
}

// void cd(char *dir, char *currentDir, char *arg){
//     int i;
// }

void cat(char *name, char currentDir){//read file
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
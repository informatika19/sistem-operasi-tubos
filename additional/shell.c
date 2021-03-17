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
        getCurrentDir(filesSector, currentDir); //get current directory
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
    //TODO cd, ln
    char command[16];
    char arg[256];
    clear(command, 16);
    clear(arg, 256);
    splitstring(input, command, arg, ' ');
    
    if(strcmp(command, "ls") == 0){ //if user input ls
        ls(table, currentDir);
        //print("\r\nfolder");
        *cursor = *cursor + 4;
    } else if (strcmp(command, "cd") == 0){//if user input cd
        currentDir = cd(table, currentDir, arg);
        print("\r\ncd");
        *cursor = *cursor + 6;
    } else if (strcmp(command, "cat") == 0){//if user input cat
        cat("tes.txt", currentDir);
        //cat("tes4.txt", currentDir);
        print("\r\n");
        print(arg);
        //print("\r\ncat");
        *cursor = *cursor + 3;
    } else if (strcmp(command, "ln") == 0){//if user input ln
        print("\r\nln");
        *cursor = *cursor + 3;
    } else{
        print("\r\nInvalid Command");
        *cursor = *cursor + 3;
    }
}

void ls(char *dir, char currentDir){ //ls function
    int i;

    char name[MAX_FILENAME]; //name variable
    char p, s; //parent and sector variable
    print("\r\n* | ");
    for(i=0; i<MAX_DIR; i++){ //looping through sectors
        p = dir[ENTRY*i];
        s = dir[ENTRY*i + SOFFSET];
        if(p == currentDir && s!=EMPTY_FILES){
            strncpy(name, dir+ENTRY*i+NAME_OFFSET, MAX_FILENAME);
            print(name);
            print(" | ");
        }
    }
    print("*");
}

char cd(char *dir, char currentDir, char *argument){
    int i,j,k, dirCount;
    char successCode;
    char newDir;
    char parentByte;
    char dir_name[8][32];
    char filename[16];
    char isFolder;
    char isValidArg = 1;

    // String parsing
    i = 0;
    j = 0;
    k = 0;

    while (argument[i] != 0) {
        if (argument[i] == 0x2F && j < 8) {
            k = 0;
            j++;
        }
        else {
            dir_name[j][k] = argument[i];
            k++;
        }
        i++;
    }
    dirCount = j + 1; 

    i = 0;
    while (i < dirCount && isValidArg) {
        // Jika argumen diawali .
        if (strcmp(dir_name[i], ".") == 0) {
            newDir = currentDir;
        } else if (strcmp(dir_name[i], "..") == 0){ // Jika diawali ..
            if (currentDir >= 0 && currentDir < 512 ) {
                newDir = dir[currentDir * 16 + 0]; // Parent directory
            }
        } else { // Jika menemukan foldername
            j = 0;
            isFolder = 0;

            while (j < 512 && !isFolder) {
                clear(filename, 16);
                strcpybounded(filename, dir[j * 16 + 2], 14);

                parentByte = dir[j * 16 + 0];

                if (strcmp(dir_name[i], filename) == 0 && parentByte == currentDir) {
                    isFolder = 1;
                    newDir = j;
                }
                j++;
            }

            if (!isFolder) {
                isValidArg = 0;
            }
        }
        i++;
    }

    if (!isValidArg) {
        successCode = 0;
    } else {
        successCode = 1;
    }

    if (successCode) {
        return newDir;
    } else {
        return currentDir;
    }
}

void cat(char *fname, char currentDir){//read file
    char input[1024];
    char content[8192];
    int returnRead;
    
    if(strcmp(fname, " ")==0){
        do{
            print("\r\n");
            interrupt(0x21, 1, input, 0, 0);
            print("\r\n");
            print(input);
        } while(strcmp(input, "exit")!=0);
        interrupt(0x10, 0x0003, 0, 0, 0);
    } else{
        readFile(content, fname, &returnRead, currentDir);
        if(returnRead == -1){
            print("\r\nfile not found");
        } else{
            print("\r\n");
            print(content);
        }
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

// void ln(char *dir, char currentDir, char *name){
//     char fname[MAX_FILENAME];
//     char content[8192];
//     int success = 0;
//     readFile(content, name, &success, currentDir);
//     if(success == -1){

//     }
// }
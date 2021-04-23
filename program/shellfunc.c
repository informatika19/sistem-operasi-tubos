#define MAX_DIR 64
#define SECTOR_SIZE 512
#define MAX_FILENAME 14
#define ENTRY 0x10
#define SOFFSET 0x1
#define NAME_OFFSET 0x2
//Tabel Sectors
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

#define ROOT 0xFF
#define EMPTY_FILES 0xDE

void ls(char *dir, char currentDir){ //ls function
    int i;

    char name[MAX_FILENAME]; //name variable
    char p, s; //parent and sector variable
    printString("\r\n* | ");
    for(i=0; i<MAX_DIR; i++){ //looping through sectors
        p = dir[ENTRY*i];
        s = dir[ENTRY*i + SOFFSET];
        if(p == currentDir && s!=EMPTY_FILES){
            strncpy(name, dir+ENTRY*i+NAME_OFFSET, MAX_FILENAME);
            printString(name);
            printString(" | ");
        }
    }
    printString("*");
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
                strcpybounded(filename, dir + j * 16 + 2, 14);

                parentByte = dir[j * 16 + 0];

                if (strcmp(dir_name[i], filename) == 0 && parentByte == currentDir) {
                    isFolder = 1;
                    newDir = j;
                }
                j++;
            }

            if (!isFolder) {
                printString("\r\n\r");
                printString("Bukan sebuah directory!");
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
    
    if(strcmp(fname, "")==0){
        do{
            printString("\r\n");
            interrupt(0x21, 1, input, 0, 0);
            printString("\r\n");
            printString(input);
        } while(strcmp(input, "exit")!=0);
        interrupt(0x10, 0x0003, 0, 0, 0);
    }
    else{
        readFile(content, fname, &returnRead, currentDir);
        if(returnRead == -1){
            printString("\r\nfile not found");
        } else{
            printString("\r\n");
            printString(content);
        }
    }
}

void getCurrentDir(char *dir, char currentDir){
    printString("\r");
    if(currentDir == ROOT)
        printString("root\n\r");
    else
        printString(dir+currentDir*16+NAME_OFFSET);
}

extern int interrupt (int number, int AX, int BX, int CX, int DX);


int main(){
    int return2;
    char buffer[512];
    char input[1024];
    char arg1[512];
    char arg2[512];
    char filesSector[2][512]; //contains filesystem files
    char currentDir = 0xFF; //current direcory (default root)

    clear(); //clear screen
    printString("Shell\n\r");
    while(1){
        getDir(filesSector); //readsector then assign it to filesSector
        clear(buffer, 512);
        clear(input, 128);
        getCurrentDir(filesSector, currentDir); //get current directory
        readString(input);
        splitstring(input, arg1, arg2, ' ');
        if(strcmp(input, "ls") == 0){
            ls(filesSector, currentDir);
        } if(strcmp(arg1, "cat") == 0){
            cat(arg2, currentDir);
        } if(strcmp(arg1, "rm") == 0){
            executeProgram("rm", 0x3000, &return2, 0xFF);
        } if(strcmp(arg1, "mv") == 0){
            executeProgram("mv", 0x3000, &return2, 0xFF);
        } if(strcmp(arg1, "debug") == 0){
            strcopy("hello", buffer);
            writeFile(buffer, "tes2.txt", &return2, 0xFF);
        }
        printString("\r\n\r");
    }
    
}


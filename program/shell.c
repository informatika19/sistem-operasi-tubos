extern int interrupt (int number, int AX, int BX, int CX, int DX);


int main(){
    int return2;
    char buffer[512];
    char input[1024];
    char arg1[512];
    char arg2[512];
    char dir[512];
    char filesSector[2][512]; //contains filesystem files
    char currentDir = 0xFF; //current direcory (default root)

    clear(); //clear screen
    printString("TubOShell\n\r");
    while(1){
        getDir(filesSector); //readsector then assign it to filesSector
        clear(buffer, 512);
        clear(arg2, 512);
        clear(input, 128);
        clear(dir, 512);
        dir[0] = currentDir;
        getCurrentDir(filesSector, currentDir); //get current directory
        readString(input);
        splitstring(input, arg1, arg2, ' ');
        writeFile(arg2, "arg.temp", &return2, 0xFF);
        writeFile(dir, "dir.temp", &return2, 0xFF);

        if(strcmp(input, "ls") == 0){
            ls(filesSector, currentDir);
        } if(strcmp(arg1, "cd") == 0){
            /*Todo:
                Entah kenapa input cd gabisa masuk branch if yang ini.
            */
            currentDir = cd(filesSector, currentDir, arg2);
        } if(strcmp(arg1, "cat") == 0){ //cat created
            executeProgram("cat", 0x3000, &return2, 0xFF); //exec cat as external program
        } if(strcmp(arg1, "rm") == 0){ //rm created
            executeProgram("rm", 0x3000, &return2, 0xFF); //exec rm as external program
        } if(strcmp(arg1, "mv") == 0){ //mv not yet
            executeProgram("mv", 0x3000, &return2, 0xFF); //exec mv as external program
        } if(strcmp(arg1, "cp") == 0){ //cp created
            executeProgram("cp", 0x3000, &return2, 0xFF); //exec cp as external program
        } if(strcmp(arg1, "mkdir") == 0){
            executeProgram("mkdir", 0x3000, &return2, 0xFF); //exec mkdir as external program
        } 
        printString("\r\n\r"); //move cursor
        removeFile("arg.temp", &return2, 0xFF); //remove temp file
        removeFile("dir.temp", &return2, 0xFF); //remove temp file
    }
    
}


extern int interrupt (int number, int AX, int BX, int CX, int DX);


int main(){
    int return2;
    char buffer[512];
    char input[1024];
    char arg1[512];
    char arg2[512];
    char debug[8092];
    char filesSector[2][512]; //contains filesystem files
    char currentDir = 0xFF; //current direcory (default root)

    clear(); //clear screen
    printString("TubOShell\n\r");
    while(1){
        getDir(filesSector); //readsector then assign it to filesSector
        clear(buffer, 512);
        clear(arg2, 512);
        clear(input, 128);
        getCurrentDir(filesSector, currentDir); //get current directory
        readString(input);
        splitstring(input, arg1, arg2, ' ');
        printString(arg1);
        printString(arg2);
        writeFile(arg2, "arg.temp", &return2, 0xFF);

        if(strcmp(input, "ls") == 0){
            printString("ls");
            ls(filesSector, currentDir);
            removeFile("arg.temp", &return2, 0xFF);
        } if(strcmp(arg1, "cd") == 0){
            /*Todo:
                Entah kenapa input cd gabisa masuk branch if yang ini.
            */
            printString("\r\n\r");
            printString("cd");
            // currentDir = cd(filesSector, currentDir, arg2);
            // removeFile("arg.temp", &return2, 0xFF);
        } if(strcmp(arg1, "cat") == 0){ //cat created
            executeProgram("cat", 0x5000, &return2, 0xFF);
        } if(strcmp(arg1, "rm") == 0){ //rm created
            executeProgram("rm", 0x3000, &return2, 0xFF);
        } if(strcmp(arg1, "mv") == 0){ //mv not yet
            //executeProgram("mv", 0x3000, &return2, 0xFF);
        } if(strcmp(arg1, "cp") == 0){ //cp created
            executeProgram("cp", 0x3000, &return2, 0xFF);
        } if(strcmp(arg1, "debug") == 0){
            strcopy("hello", buffer);
            writeFile(buffer, "tes2.txt", &return2, 0xFF);
        } if(strcmp(arg1, "remov") == 0){
            //deleteFile("tes2.txt", &return2, 0xFF);
            removeFile("tes2.txt", &return2, 0xFF);
        } if(strcmp(arg1, "mkdir") == 0){
            executeProgram("mkdir", 0x5000, &return2, 0xFF);
        }
        printString("\r\n\r");
    }
    
}


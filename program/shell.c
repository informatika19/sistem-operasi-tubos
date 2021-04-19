extern int interrupt (int number, int AX, int BX, int CX, int DX);


int main(){
    char input[1024];
    char filesSector[2][512]; //contains filesystem files
    char currentDir = 0xFF; //current direcory (default root)

    getDir(filesSector); //readsector then assign it to filesSector
    clear(); //clear screen
    printString("Shell\n\r");
    while(1){
        clear(input, 128);
        getCurrentDir(filesSector, currentDir); //get current directory
        readString(input);
        if(strcmp(input, "ls") == 0){
            ls(filesSector, currentDir);
            printString("\n\r");
        }
        printString("\r\n\r");
    }
    
}


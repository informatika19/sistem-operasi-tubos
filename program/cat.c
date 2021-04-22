void cat(char *fname, char currentDir);

int main(){
    char arg2[8192];
    int return2;
    clear(arg2, 8192);
    readFile(arg2, "arg.temp", &return2, 0xFF);
    cat(arg2, 0xFF);
    printString("\r");
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
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
            printString("\r\n\nfile not found\n");
        } else{
            printString("\r\n");
            printString(content);
        }
    }
}
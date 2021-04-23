int main(){
    char arg2[8192];
    char argdir[8192];
    char dir;
    int return2;
    clear(argdir, 8192);
    clear(arg2, 8192);
    readFile(arg2, "arg.temp", &return2, 0xFF);
    readFile(argdir, "dir.temp", &return2, 0xFF);
    dir = argdir[0];
    removeFile(arg2, &return2, dir);
    //TODO remove relative pathing
    if(return2 == 0 || return2 == 1){
        printString("\r\nRemove Successfull\n\r");
    }
    else
        printString("\r\nRemove Failed\n\r");
    removeFile("dir.temp", &return2, 0xFF);
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
}
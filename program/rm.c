int main(){
    char arg2[8192];
    int return2;
    clear(arg2, 8192);
    readFile(arg2, "arg.temp", &return2, 0xFF);
    removeFile(arg2, &return2, 0xFF);
    //TODO remove relative pathing
    if(return2 == 0 || return2 == 1)
        printString("\r\nRemove Successfull\n\r");
    else
        printString("\r\nRemove Failed\n\r");
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
}
int main(){
    // Fungsi write file yang diberi argumen pertama 0x00 akan membuat sebuah
    // folder dengan nama yang dimasukkan pada argumen kedua
    char foldername[8192];
    char argdir[8192];
    int returncode;
    char currentDir;
    clear(foldername, 8192);
    clear(argdir, 8192);
    readFile(foldername, "arg.temp", &returncode, 0xFF);
    readFile(argdir, "dir.temp", &returncode, 0xFF);
    currentDir = argdir[0];
    writeFile(0x00, foldername, &returncode, currentDir);
    printString("\n\r");
    if(returncode == 0){
        printString("Folder ");
        printString(foldername);
        printString(" created!");
    } if(returncode == -1){
        printString("Folder name ");
        printString(foldername);
        printString(" already exist!");
    }
    printString("\n\r");
    removeFile("arg.temp", &returncode, 0xFF);
    removeFile("dir.temp", &returncode, 0xFF);
    executeProgram("tes", 0x2000, &returncode, 0xFF);
}
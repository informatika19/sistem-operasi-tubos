int main(){
    // Fungsi write file yang diberi argumen pertama 0x00 akan membuat sebuah
    // folder dengan nama yang dimasukkan pada argumen kedua
    char foldername[8192];
    int returncode;
    char currentDir;
    readFile(foldername, "arg.temp", &returncode, 0xFF);
    writeFile(0x00, foldername, &returncode, 0xFF);
    removeFile("arg.temp", &returncode, 0xFF);
    printString("\n\r");
    printString("Folder ");
    printString(foldername);
    printString(" terbuat!");
    printString("\n\r");
    executeProgram("tes", 0x2000, &returncode, 0xFF);
}
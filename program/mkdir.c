int main(){
    // Fungsi write file yang diberi argumen pertama 0x00 akan membuat sebuah
    // folder dengan nama yang dimasukkan pada argumen kedua
    char argc[8192];
    char argdir[8192];
    char split1[512];
    char split2[512];
    char filesSector[2][512];
    int returncode;
    char currentDir;

    clear(argc, 8192);
    clear(argdir, 8192);
    clear(split1, 512);
    clear(split2, 512);

    getDir(filesSector);
    readFile(argc, "arg.temp", &returncode, 0xFF);
    readFile(argdir, "dir.temp", &returncode, 0xFF);
    splitstring(argc, split1, split2, ' ');
    currentDir = argdir[0];
    currentDir = directoryEvaluator(filesSector, split2, &returncode, currentDir);
    
    if(!strcmp(split2, ".")){
        writeFile(0x00, split1, &returncode, currentDir);
        printString("\n\r");
        if(returncode == 0){
            printString("Folder ");
            printString(split1);
            printString(" created!");
        } if(returncode == -1){
            printString("Folder name ");
            printString(split1);
            printString(" already exist!");
        }
        printString("\n\r");
    } else{
        writeFile(0x00, split1, &returncode, currentDir);
        printString("\n\r");
        if(returncode == 0){
            printString("Folder ");
            printString(split1);
            printString(" created!");
        } if(returncode == -1){
            printString("Folder name ");
            printString(split1);
            printString(" already exist!");
        }
        printString("\n\r");
    }
    removeFile("arg.temp", &returncode, 0xFF);
    removeFile("dir.temp", &returncode, 0xFF);
    executeProgram("tes", 0x2000, &returncode, 0xFF);
}
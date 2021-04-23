int main(){
    char argc[8192];
    char argdir[8192];
    char split1[512];
    char split2[512];
    char filesSector[2][512];
    int returncode;

    char srcDir;
    char targetDir;

    clear(argc, 8192);
    clear(argdir, 8192);
    clear(split1, 512);
    clear(split2, 512);

    getDir(filesSector);
    readFile(argc, "arg.temp", &returncode, 0xFF);
    readFile(argdir, "dir.temp", &returncode, 0xFF);
    splitstring(argc, split1, split2, ' ');
    srcDir = argdir[0];
    targetDir = directoryEvaluator(filesSector, split2, &returncode, srcDir);
    
    if(!strcmp(split2, ".")){
        printString("\n\r");
        printString("Moved to the same folder");
        printString("\n\r");
    } else{
        moveFile(split1, &returncode, srcDir, targetDir);
        printString("\n\r");
        if(returncode == 0){
            printString(split1);
            printString(" moved!");
        } if(returncode == -1){
            printString(" error");
        }
        printString("\n\r");
    }
    removeFile("arg.temp", &returncode, 0xFF);
    removeFile("dir.temp", &returncode, 0xFF);
    executeProgram("tes", 0x2000, &returncode, 0xFF);
}
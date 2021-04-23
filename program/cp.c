void cp(char *filenamesrc, char *filenamedst, char currentidx);
int main(){
    char arg2[8192];
    char buff[8192];
    char split1[512];
    char split2[512];
    int return2;
    int debug;
    clear(buff, 8192);
    //read argc
    readFile(arg2, "arg.temp", &return2, 0xFF);
    //split arg 2
    splitstring(arg2, split1, split2, ' ');
    //read isi text
    readFile(buff, split1, &return2, 0xFF);
    cp(split1,split2,0xFF);
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
}

void cp(char *filenamesrc, char *filenamedst, char currentidx) {
    if (!strcmp(filenamedst,"."))
    {
        printString("\r\nSame file already exist\n\r");
    }else
    {
        char file_buffer[8192];
        int returncode;
        int destIndex;
        char buff1[1024];
        readSector(buff1,0x101);
        readSector(buff1 + 512,0x102);
        readFile(file_buffer, filenamesrc, &returncode, currentidx);
        destIndex = directoryEvaluator(buff1, filenamedst, &returncode,currentidx);
        writeFile(file_buffer, filenamesrc, &returncode, destIndex);
        printString("\r\ncp success\n\r");
    }
}
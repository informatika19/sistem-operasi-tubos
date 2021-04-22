int main(){
    char arg2[8192];
    char buff[8192];
    char split1[512];
    char split2[512];
    char *newfname;
    int return2;
    clear(buff, 8192);
    clear(newfname, 8192);
    //read argc
    readFile(arg2, "arg.temp", &return2, 0xFF);
    //split arg 2
    splitstring(arg2, split1, split2, ' ');
    //read isi text
    readFile(buff, split1, &return2, 0xFF);
    //copy ke folder yang sama
    newfname = strcat(split1, "1");
    printString(newfname);
    if(!strcmp(split2, ".")){
        writeFile(buff, newfname, &return2, 0xFF);
    }
    //TODO cp to relative pathing
    //{

    //}
    printString("\r\ncp success\n\r");
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
}
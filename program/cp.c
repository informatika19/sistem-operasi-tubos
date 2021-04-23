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
    debug = 1;
    //copy ke folder yang sama
    if(!strcmp(split2, ".")){
        //TODO string name butuh convert int ke char
        strapp(split1, "copy1"); //append name
        writeFile(buff, split1, &return2, 0xFF);
    }
    //TODO cp to relative pathing
    //{

    //}
    printString("\r\ncp success\n\r");
    removeFile("arg.temp", &return2, 0xFF);
    executeProgram("tes", 0x2000, &return2, 0xFF);
}
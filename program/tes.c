extern int interrupt (int number, int AX, int BX, int CX, int DX);
int main(){
    char input[1024];
    interrupt(0x10, 0x0003, 0, 0, 0);
    interrupt(0x21, 0, "\rShell\n\r", 0, 0);
    while(1){
        interrupt(0x21, 1, input, 0, 0);
        // if(strcmp(input, "shell") == 0){
        //     interrupt(0x21, 0, "\rShell\n\r", 0, 0);
        // }
        interrupt(0x21, 0, "\r\n\r", 0, 0);
    }
}


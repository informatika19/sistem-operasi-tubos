extern int interrupt (int number, int AX, int BX, int CX, int DX);
int main(){
    interrupt(0x21, 0, "\rcokkkkk\n", 0, 0);
    interrupt(0x21, 0, "\rcokkkkk2\n", 0, 0);
    while(1);
}


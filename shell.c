void printString(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        int CH = string[i];
        interrupt(0x10, 0xe * 256 + CH, 0, 0, 0); //interrupt untuk menampilkan char ke layar
        i++;
    }
}

int main(){
    printString("\r\nhello gan")
    while(true);
}
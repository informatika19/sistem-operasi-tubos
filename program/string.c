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

void readString(char *string)
{
    int count = 0, input;
    while (1)
    {
        input = interrupt(0x16, 0, 0, 0, 0);
        if (input == 0xD) // 13 adalah simbol ascii tombol enter
        {
            string[count] = '\0';
            return; //keluar dari loop
        }
        else if (input == 8) // 8 adalah simbol ascii tombol backspace
        {
            //agar cursor mundur ke kiri
            interrupt(0x10, 0xe * 256 + 8, 0, 0, 0); //backspace
            interrupt(0x10, 0xe * 256 + 0, 0, 0, 0); //menghapus input saat itu(diganti null)
            interrupt(0x10, 0xe * 256 + 8, 0, 0, 0); //kembali ke backspace
            count--;
        }
        else
        {
            string[count] = input;                       //input dari keyboard
            interrupt(0x10, 0xe * 256 + input, 0, 0, 0); // menampilkan ke layar hasil inputan
            count++;
        }
    }
}

void clear(char *buffer, int length)
{
    int i = 0;

    /* Mengganti isi buffer 
    dengan empty yang sama dengan 0 */
    while (i < length)
    {
        buffer[i] = 0x00;
        i++;
    }
}

int strcmp(char *str1, char*str2){
    while (*str1 == *str2) {
          if (*str1 == '\0' || *str2 == '\0')
            break;

          str1++;
          str2++;
       }


    if (*str1 == '\0' && *str2 == '\0')
        return 0;
    else
        return -1;
}
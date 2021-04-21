#include "../kernel.h"

//MILESTONE 1

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
        buffer[i] = EMPTY;
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

int strlen(char* string){
    int count = 0;
    while (string[count] != '\0')
    {
        count++;
    }
    return count;
}

void strcopy(char *src, char *dest) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void rawstrcpy(char *dest, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
}

void strcpybounded(char *dest, char *src, int n) {
    int i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strncpy(char *destination, char *source, int n){

    while(*source && n--){
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
}

void splitstring(char *src, char* part1, char *part2, char x) {
    int i = 0;
    int j = 0;
    while (src[i] != '\0' && src[i] != x) {
        part1[i] = src[i];
        i++;
    }
    part1[i] = '\0';
    i++;
    while (src[i] != '\0') {
        part2[j] = src[i];
        i++;
        j++;
    }
    part2[j] = '\0';
}
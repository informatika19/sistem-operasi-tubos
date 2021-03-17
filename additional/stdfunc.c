#include "stdfunc.h"

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

int mod(int x, int y)
{
    return (x - y * (x / y));
}

int div(int x, int y)
{
    return x/y;
}

int strlen(char* string){
    int count = 0;
    while (string[count] != '\0')
    {
        count++;
    }
    return count;
}

//copy str1 ke str2
// void strcopy(char* str1, char* str2){

//     while (*str1 != '\0')
//     {
//         *str2 = *str1;
//         str1++;
//         str2++;
//     }
//     *str2 = '\0';
// }

// void strncopy(char* src, char* dest, char x){

//     int i = 0;
//     while (src[i] != '\0' && src[i] != x) {
//         dest[i] = src[i];
//         i++;
//     }
//     dest[i] = '\0';
// }

void strcopy(char *src, char *dest) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
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

void splitstring(char *src, char* part1, char* part2, char x) {
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
//a[3][5]
//  11111 22222 33333
//               a[2][1] = *(a+5i+j)
// input [128][16];

void copySegmentSectorSectors(char *dest, char *src, int n) {
    int i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }
}

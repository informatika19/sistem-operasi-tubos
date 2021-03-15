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
void strcopy(char* str1, char* str2){

    while (*str1 != '\0')
    {
        *str2 = *str1;
        str1++;
        str2++;
    }
}

void strncpy(char *destination, char *source, int n){

    while(*source && n--){
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
}
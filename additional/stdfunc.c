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
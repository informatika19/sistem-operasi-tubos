#ifndef __STRING__H__
#define __STRING__H__

int strcmp(char *str1, char*str2);
int strlen(char* string);
void splitstring(char *src, char* part1, char* part2, char x);
void strcpybounded(char *dest, char *src, int n);
void strncpy(char *destination, char *source, int n);
void strcopy(char *src, char *dest);

#endif
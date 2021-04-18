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
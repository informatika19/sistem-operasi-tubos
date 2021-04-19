void copySegmentSectorSectors(char *dest, char *src, int n) {
    int i = 0;
    while (src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }
}

void readSector(char *buffer, int sector){
    interrupt(0x13,0x02*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13,0x03*0x100+0x01,buffer,div(sector,36)*0x100+mod(sector,18) + 1,mod(div(sector,18),2)*0x100 + 0);
}
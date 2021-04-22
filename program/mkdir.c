# define MAX_NAME_LENGTH 14
// // Fungsi mendaftarkan folder pada image
// void createFolder(char* filename, char idxParent) {
//     int index;
//     int row;
//     char files[1024];

//     // Baca sektor files di 0x101 dan 0x102
//     readSector(files, 0x101);
//     readSector(files+512, 0x102);

//     // Cek filename (ada 64 row)
//     for (row = 0; row < 64; ++row) {
//         if (files[row * 16 + 0] == idxParent) {
//             if (strcmp(filename, files + row*16+2) == 0) {
//                 printString("Sudah ada folder.");
//                 return;
//             }
//         }
//     }

//     printString("Folder telah dibuat.");
// }

int main(){
    char foldername[MAX_NAME_LENGTH];
    int returncode;
    char currentDir;
    writeFile(0x00, foldername, &returncode, currentDir);
}
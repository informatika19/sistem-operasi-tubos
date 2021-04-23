# MILESTONE 3 IF2230 Sistem Operasi
> Dibuat oleh TubOS

## Anggota Kelompok
1. Pratama Andiko           (13519112)  
2. Alvin Rizqi Alfisyahrin  (13519126)
3. Fabian Savero Diaz P.    (13519140)  
4. Fadel Ananda Dotty       (13519146)  

## How to run
> Masukkan "./runall.sh" tanpa tanda kutip, maka semua program external akan dimasukkan ke filesystem dan program akan berjalan

## Modul
1. Modul teks (string.c)
2. Modul fileIO (file_system.c)
3. Modul matematika (math.c)

## Program External
1. cat: mencetak isi file, memanggil di shell dengan perintah "cat {namafile}" pada directory tempat file berada
2. cp: menduplikasi file, memanggil di shell dengan perintah "cp {namafile} {directory}"
3. mkdir: membuat directory pada filesystem, memanggil di shell dengan perintah "mkdir {namadirectory} {directorytujuan}"
4. rm: menghapus file pada filesystem, memanggil di shell dengan perintah "rm {namafile}" pada directory tempat file berada
5. mv: memindahkan file dari suatu directory ke directory lain, memanggil dengan perintah "mv {namafile} {directorytujuan}"
6. ln: tidak membuat

## Kode Kelompok Lain
> Beberapa fungsi memakai fungsi pada tag rilis 2.0 kelompok Mangga (K-04)
# MILESTONE 1 IF2230 Sistem Operasi
##### Dibuat oleh TubOS
###### Anggota Kelompok
1. Pratama Andiko           (13519112)  
2. Fabian Savero Diaz P.    (13519140)  
3. Fadel Ananda Dotty       (13519146)  

- Cara kerja *interrupt* pada Sistem Operasi:  
Pada sebuah sistem operasi, sebuah kejadian biasanya dimulai karena ada *interrupt*
dari *hardware* atau *software*. Hardware dapat melakukan *interrupt* dengan mengirim sinyal ke CPU
melalui bus sistem. *Software* juga dapat melakukan *interrupt* dengan mengeksekusi sebuah system call.\

Ketika sebuah CPU itu di-*interrupt*, CPU akan menghentikan operasi saat ini dan langsung memindah eksekusi 
ke sebuah address dimana *service routine* untuk *interrupt* terletak. Setelah *interrupt* service routine selesai dieksekusi, CPU akan kembali melanjutkan operasi yang sebelumnya terhenti.

- Cara kerja kode di kernel.asm:
1. putInMemory
2. interrupt
3. makeInterrupt21
4. handleInterrupt21
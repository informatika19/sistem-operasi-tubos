# MILESTONE 1 IF2230 Sistem Operasi
> Dibuat oleh TubOS
## Anggota Kelompok
1. Pratama Andiko           (13519112)  
2. Fabian Savero Diaz P.    (13519140)  
3. Fadel Ananda Dotty       (13519146)  

## Cara kerja *interrupt* pada Sistem Operasi:  
Pada sebuah sistem operasi, sebuah kejadian biasanya dimulai karena ada *interrupt*
dari *hardware* atau *software*. Hardware dapat melakukan *interrupt* dengan mengirim sinyal ke CPU
melalui bus sistem. *Software* juga dapat melakukan *interrupt* dengan mengeksekusi sebuah system call.

Ketika sebuah CPU itu di-*interrupt*, CPU akan menghentikan operasi saat ini dan langsung memindah eksekusi 
ke sebuah address dimana *service routine* untuk *interrupt* terletak. Setelah *interrupt* service routine selesai dieksekusi, CPU akan kembali melanjutkan operasi yang sebelumnya terhenti.

## Cara kerja kode di kernel.asm:
1. putInMemory
Fungsi putInMemory menerima 3 parameter yaitu *segment*, *address*, dan *char*. Fungsi ini akan menuliskan parameter *character* ke dalam *address* memori yang dirujuk. Dalam hal ini misalnya kita memanggil fungsi putInMemory(0xB000, 0x8000, 'H'), maka fungsi tersebut akan menuliskan H pada koordinat x=0 dan y=0. 0xB000 merupakan segmen *video memory* dan 0x8000 merupakan offset memori yang mendefinisikan karakter pada x=0 dan y=0.

2. interrupt
Fungsi *interrupt* menerima 5 parameter yaitu *number*, AX, BX, CX, dan DX. Parameter *number* menunjukkan angka *bios interrupt* yang ingin dipanggil. Misalnya kita memanggil fungsi interrupt(0x10, 0x0003, 0, 0, 0). Fungsi ini akan memanggil *interrupt* pada *bios* dengan kode 10. 0x0003 merupakan parameter AX yang terdiri dari AH dan AL. Terdapat tabel-tabel yang berisi macam-macam fungsi dan parameternya yang dapat dipanggil dari sebuah nomor interrupt. Misalnya saja dapat dilihat pada laman https://en.wikipedia.org/wiki/INT_10H.

3. makeInterrupt21
Fungsi makeInterrupt berfungsi untuk melakukan setup agar apabila interrupt 21 dipanggil, maka akan memanggil fungsi handleInterrupt21.

4. handleInterrupt21
Fungsi handleInterrupt21 menerima 4 parameter yaitu AX, BX, CX, dan DX. Fungsi ini akan melakukan fungsi-fungsi lain tergantung dari interrupt yang dipanggil dan parameter AX yang diberikan. Misalnya interrupt(0x21, 0x0, 0, 0, 0) akan memanggil fungsi printString.
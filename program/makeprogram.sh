bcc -ansi -c -o tes.o $1 && ld86 -o tes -d string.o tes.o interrupt.o
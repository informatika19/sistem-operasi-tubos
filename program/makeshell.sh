bcc -ansi -c -o out/math.o ../kernel/modules/math.c &&
bcc -ansi -c -o out/string.o ../kernel/modules/string.c &&
bcc -ansi -c -o out/tes1.o ../kernel/modules/file_system.c &&
bcc -ansi -c -o out/tes.o $1 && 
bcc -ansi -c -o out/func.o $2 && 
nasm -f as86 lib.asm -o out/lib.o &&
ld86 -o tes -d out/tes.o out/func.o out/lib.o out/math.o out/string.o out/tes1.o
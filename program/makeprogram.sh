bcc -ansi -c -o out/math.o ../kernel/modules/math.c &&
bcc -ansi -c -o out/string.o ../kernel/modules/string.c &&
bcc -ansi -c -o out/tes1.o ../kernel/modules/file_system.c &&
bcc -ansi -c -o out/$1.o $1.c &&  
nasm -f as86 lib.asm -o out/lib.o &&
ld86 -o $1 -d out/$1.o out/lib.o out/math.o out/string.o out/tes1.o
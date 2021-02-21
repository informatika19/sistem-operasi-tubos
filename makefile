#Makefile
all: clean image boot compile

image:
	dd if=/dev/zero of=system.img bs=512 count=2880

boot:
	nasm bootloader.asm -o bootloader
	dd if=bootloader of=system.img bs=512 count=1 conv=notrunc

compile:
	bcc -ansi -c -o kernel.o kernel.c
	nasm -f as86 kernel.asm -o kernel_asm.o
	ld86 -o kernel -d kernel.o kernel_asm.o
	dd if=kernel of=system.img bs=512 conv=notrunc seek=1

clean:
	rm *.img


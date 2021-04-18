#Makefile
all: compileBootLoader enterToDisk compileKernel

createImage:
#dd if=/dev/zero of=system.img bs=512 count=2880
# dd if=/dev/zero of=map.img bs=512 count=1
# dd if=/dev/zero of=files.img bs=512 count=2
# dd if=/dev/zero of=sectors.img bs=512 count=1

compileBootLoader:
	nasm asm/bootloader.asm -o compiledResult/bootloader

enterToDisk:
	dd if=compiledResult/bootloader of=system.img bs=512 count=1 conv=notrunc

compileKernel:
	bcc -ansi -c -o compiledResult/kernel.o kernel/kernel.c
	# bcc -ansi -c -o compiledResult/shell.o additional/shell.c
	# bcc -ansi -c -o compiledResult/stdfunc.o additional/stdfunc.c
	nasm -f as86 asm/kernel.asm -o compiledResult/kernel_asm.o
	ld86 -o compiledResult/kernel -d compiledResult/*.o
	dd if=compiledResult/kernel of=system.img bs=512 conv=notrunc seek=1

cleanImg:
	rm *.img

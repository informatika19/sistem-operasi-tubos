#create image
gcc -Wall -Wextra cimg.c -o cimg;
./cimg;
dd if=/dev/zero of=sectors.img bs=512 count=1;

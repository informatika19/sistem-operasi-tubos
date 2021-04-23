cd program/&&
./makeshell.sh shell.c shellfunc.c&&
./makeprogram.sh cat&&
./makeprogram.sh cp&&
./makeprogram.sh rm&&
cd ..&&
cp -a program/cat .&&
cp -a program/cp .&&
cp -a program/rm .&&
./loadshell.sh&&
./loadfile cat&&
./loadfile cp&&
./loadfile rm&&
./loadfile tes2.txt&&
./buildrun.sh
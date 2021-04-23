cd img/&&
./create.sh&&
cd ..&&
cp -a img/map.img .&&
cp -a img/sectors.img .&&
cp -a img/files.img .&&
cd program/&&
./makeshell.sh shell.c shellfunc.c&& #buat program external shell
./makeprogram.sh cat&& #buat program external cat
./makeprogram.sh cp&& #buat program external cp
./makeprogram.sh rm&& #buat program external rm
./makeprogram.sh mv&& #buat program external mv
./makeprogram.sh mkdir&& #buat program external mkdir
./makeprogram.sh ln&& #buat program external ln
cd ..&&
cp -a program/cat .&& #copy cat bin file to root
cp -a program/cp .&& #copy cp bin file to root
cp -a program/rm .&& #copy rm bin file to root
cp -a program/ln .&& #copy ln bin file to root
cp -a program/mkdir .&& #copy mkdir bin file to root
cp -a program/mv .&& #copy bin mv file to root
./loadfile.sh&& #load shell
./loadshell.sh&& #load shell
./loadfile cat&& #load cat
./loadfile cp&& #load cp
./loadfile rm&& #load rm
./loadfile mkdir&& #load mkdir
./loadfile mv&& #load mv
./loadfile ln&& #load ln
./loadfile tes2.txt&& #load testfile
./buildrun.sh
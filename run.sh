#!/bin/sh

mkdir -p obj/unzip
mkdir -p obj/zlib
mkdir -p bin/Debug
make Debug

./bin/Debug/unzipRom -idprog /bin/ls -lrt /home/pi/programacion/Alex.zip

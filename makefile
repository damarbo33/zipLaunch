# A simple makefile generator by KiSoft, 2010. mailto: kisoft@rambler.ru
# version: 0.3.12.5

# Project Variables start
CPP=g++
CC=gcc
LD=g++
LIB=ar
#WINDRES=
# Project Variables end

# Target: Debug

OBJS_DEBUG=obj/dirutil.o obj/excepcion.o obj/fileattr.o obj/fileio.o obj/listaSimple.o obj/main.o obj/tcstring.o obj/traza.o obj/unzip/ioapi.o obj/unzip/unzip.o obj/unzip/unziptool.o obj/zlib/adler32.o obj/zlib/compress.o obj/zlib/crc32.o obj/zlib/deflate.o obj/zlib/gzio.o obj/zlib/infback.o obj/zlib/inffast.o obj/zlib/inflate.o obj/zlib/inftrees.o obj/zlib/minigzip.o obj/zlib/trees.o obj/zlib/uncompr.o obj/zlib/zutil.o

Debug: bin/Debug/unzipRom

bin/Debug/unzipRom: $(OBJS_DEBUG)
	@echo Building console executable bin/Debug/unzipRom
	@g++   -o bin/Debug/unzipRom $(OBJS_DEBUG)  -lz  

obj/dirutil.o: src/dirutil.cpp
	@echo Compiling: src/dirutil.cpp
	@g++ -Wall  -g -DUNIX    -c src/dirutil.cpp -o obj/dirutil.o

obj/excepcion.o: src/excepcion.cpp
	@echo Compiling: src/excepcion.cpp
	@g++ -Wall  -g -DUNIX    -c src/excepcion.cpp -o obj/excepcion.o

obj/fileattr.o: src/fileattr.cpp
	@echo Compiling: src/fileattr.cpp
	@g++ -Wall  -g -DUNIX    -c src/fileattr.cpp -o obj/fileattr.o

obj/fileio.o: src/fileio.cpp
	@echo Compiling: src/fileio.cpp
	@g++ -Wall  -g -DUNIX    -c src/fileio.cpp -o obj/fileio.o

obj/listaSimple.o: src/listaSimple.cpp
	@echo Compiling: src/listaSimple.cpp
	@g++ -Wall  -g -DUNIX    -c src/listaSimple.cpp -o obj/listaSimple.o

obj/main.o: src/main.cpp
	@echo Compiling: src/main.cpp
	@g++ -Wall  -g -DUNIX    -c src/main.cpp -o obj/main.o

obj/tcstring.o: src/tcstring.cpp
	@echo Compiling: src/tcstring.cpp
	@g++ -Wall  -g -DUNIX    -c src/tcstring.cpp -o obj/tcstring.o

obj/traza.o: src/traza.cpp
	@echo Compiling: src/traza.cpp
	@g++ -Wall  -g -DUNIX    -c src/traza.cpp -o obj/traza.o

obj/unzip/ioapi.o: src/unzip/ioapi.c
	@echo Compiling: src/unzip/ioapi.c
	@gcc -Wall  -g -DUNIX    -c src/unzip/ioapi.c -o obj/unzip/ioapi.o

obj/unzip/unzip.o: src/unzip/unzip.c
	@echo Compiling: src/unzip/unzip.c
	@gcc -Wall  -g -DUNIX    -c src/unzip/unzip.c -o obj/unzip/unzip.o

obj/unzip/unziptool.o: src/unzip/unziptool.cpp
	@echo Compiling: src/unzip/unziptool.cpp
	@g++ -Wall  -g -DUNIX    -c src/unzip/unziptool.cpp -o obj/unzip/unziptool.o

obj/zlib/adler32.o: src/zlib/adler32.c
	@echo Compiling: src/zlib/adler32.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/adler32.c -o obj/zlib/adler32.o

obj/zlib/compress.o: src/zlib/compress.c
	@echo Compiling: src/zlib/compress.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/compress.c -o obj/zlib/compress.o

obj/zlib/crc32.o: src/zlib/crc32.c
	@echo Compiling: src/zlib/crc32.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/crc32.c -o obj/zlib/crc32.o

obj/zlib/deflate.o: src/zlib/deflate.c
	@echo Compiling: src/zlib/deflate.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/deflate.c -o obj/zlib/deflate.o

obj/zlib/gzio.o: src/zlib/gzio.c
	@echo Compiling: src/zlib/gzio.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/gzio.c -o obj/zlib/gzio.o

obj/zlib/infback.o: src/zlib/infback.c
	@echo Compiling: src/zlib/infback.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/infback.c -o obj/zlib/infback.o

obj/zlib/inffast.o: src/zlib/inffast.c
	@echo Compiling: src/zlib/inffast.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/inffast.c -o obj/zlib/inffast.o

obj/zlib/inflate.o: src/zlib/inflate.c
	@echo Compiling: src/zlib/inflate.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/inflate.c -o obj/zlib/inflate.o

obj/zlib/inftrees.o: src/zlib/inftrees.c
	@echo Compiling: src/zlib/inftrees.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/inftrees.c -o obj/zlib/inftrees.o

obj/zlib/minigzip.o: src/zlib/minigzip.c
	@echo Compiling: src/zlib/minigzip.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/minigzip.c -o obj/zlib/minigzip.o

obj/zlib/trees.o: src/zlib/trees.c
	@echo Compiling: src/zlib/trees.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/trees.c -o obj/zlib/trees.o

obj/zlib/uncompr.o: src/zlib/uncompr.c
	@echo Compiling: src/zlib/uncompr.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/uncompr.c -o obj/zlib/uncompr.o

obj/zlib/zutil.o: src/zlib/zutil.c
	@echo Compiling: src/zlib/zutil.c
	@gcc -Wall  -g -DUNIX    -c src/zlib/zutil.c -o obj/zlib/zutil.o

main.cpp: main.h

main.h: unzip/unziptool.h dirutil.h traza.h

unzip/unziptool.h: ../zlib/zlib.h unzip.h crypt.h ../fileio.h ../dirutil.h

zlib/zlib.h: zconf.h

fileio.h: excepcion.h constant.h traza.h

dirutil.h: fileattr.h constant.h traza.h listaSimple.h

unzip/ioapi.c: ../zlib/zlib.h ioapi.h

unzip/unzip.c: ../zlib/zlib.h unzip.h crypt.h

traza.h: constant.h tcstring.h

tcstring.h: traza.h

dirutil.cpp: dirutil.h

excepcion.cpp: excepcion.h

fileattr.cpp: fileattr.h

fileio.cpp: fileio.h

tcstring.cpp: tcstring.h

traza.cpp: traza.h

unzip/unziptool.cpp: unziptool.h

.PHONY: clean

clean: 
	@echo rm -f $(OBJS_DEBUG) bin/Debug/unzipRom
	-@rm -f $(OBJS_DEBUG) bin/Debug/unzipRom




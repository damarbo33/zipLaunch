CC=cc
CFLAGS=-O 

UNZ_OBJS = unzip.o ioapi.o 

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

all: unzip_lib

unzip.a: $(UNZ_OBJS)
	ar -rv unzip.a *.o
	
clean:
	/bin/rm -f *.o *.a

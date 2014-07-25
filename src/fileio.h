#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include "excepcion.h"
#include "constant.h"
#include "traza.h"

class Fileio{
    public :
        Fileio();
        ~Fileio();
        void loadFromFile(const char *);
        void loadFromFile(const char *, ifstream::pos_type , ifstream::pos_type);
        void writeToFile(const char *, char * ,ifstream::pos_type, bool append);
        void writeToFileBlock(const char *uri, char * memblocktowrite, ifstream::pos_type tam ,ifstream::pos_type blockSize, bool append);
        void clearFile();
        char * getFile() {return memblock;}
        ifstream::pos_type getFileSize(const char *);
        ifstream::pos_type getFileSize() {return size;}
        unsigned int contarLineas(const char *);

    private:
        ifstream::pos_type size;
        char * memblock;

};

#endif // FILEIO_H_INCLUDED

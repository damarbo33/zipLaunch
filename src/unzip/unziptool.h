#ifndef UNZIPTOOL_H
#define UNZIPTOOL_H

#include <unistd.h>
#include "../zlib/zlib.h"
#include "unzip.h"
#include "crypt.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../fileio.h"
#include "../dirutil.h"

using namespace std;

struct unzippedFileInfo{
    int errorCode;
    string rutaEscritura;
    unsigned long romsize;
};

class UnzipTool
{
    public:
        UnzipTool();
        virtual ~UnzipTool();
        unzippedFileInfo descomprimirZip(const char *);
    protected:
    private:
        unzippedFileInfo extraerFichero(unzFile myZip, const char *ruta);
};

#endif // UNZIPTOOL_H

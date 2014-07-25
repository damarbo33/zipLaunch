#ifndef TCString_H
#define TCString_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

class TCString
{
    private:
        char *string;

    public:
        TCString();
        TCString(const char *);
        ~TCString();
        void add(const char *);
        char * get(int);
        void clear();
        char * get();
        void trim(char *);
        void trim();
        void toLowerCase (char *);
        void toLowerCase ();
        void filtrarLetras (char *varDato);
        void filtrarLetras ();
        int size(){return strlen(string);}
};

#endif // TCString_H

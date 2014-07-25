#ifndef Excepcion_H
#define Excepcion_H

    #include <iostream>
    #include <cmath>
    #include "traza.h"

    using namespace std;

    //static const int ERANGO=100;
    //static const int EESPACIO=101;
    //static const int ENULL=102;
    //static const int EFIO=103;

    #define ERANGO 1000
    #define EESPACIO 1001
    #define ENULL 1002
    #define EFIO 1003


    class Excepcion{
    public:
        Excepcion();
        Excepcion(int);
        Excepcion(int, string);
        const char* getMessage();
        const int getCode();
        //Errores de listas


    private:
        int code;
        string sqlMsg;
    };

#endif // Excepcion_H

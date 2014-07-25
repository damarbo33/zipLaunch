#include "fileio.h"
#include <algorithm>    // std::count
#include <vector>       // std::vector
#include <iterator>     // istream_iterator

/**
*/
Fileio::Fileio(){
    memblock = NULL;
    size = 0;
}

/**
*/
Fileio::~Fileio(){
    clearFile();
}

/**
* Retorna en bytes el tamanyo de un fichero
*/
ifstream::pos_type Fileio::getFileSize(const char *uri){

    ifstream::pos_type begin,end;
    ifstream::pos_type total = 0;
    ifstream myfile (uri);
    if (myfile.is_open()){
        begin = myfile.tellg();
        myfile.seekg (0, ios::end);
        end = myfile.tellg();
        myfile.close();
        total = end-begin;
        return total;
    } else {
        return 0;
    }
}

/**
*/
void Fileio::clearFile(){
    if (memblock != NULL){
        size = 0;
        delete [] memblock;
        memblock = NULL;
    }
}

unsigned int Fileio::contarLineas(const char *filename){

    ifstream archivo(filename, ios::in);
    string str;
    unsigned int cont=0;

    if (archivo.is_open()){
        for(cont=0;!archivo.eof() && getline(archivo,str);cont++);
        archivo.close();
    }
    Traza::print("contarLineas:",cont, DEBUG);
    return cont;
}

/**
* Lee un fichero en formato binario y lo almacena en memoria
*/
void Fileio::loadFromFile(const char *uri){
    loadFromFile(uri, 0, getFileSize(uri));
}

/**
* Lee un fichero en formato binario y lo almacena en memoria
*/
void Fileio::loadFromFile(const char *uri, ifstream::pos_type ini, ifstream::pos_type tam){
    clearFile();
    //Usamos el flag ios::ate flag para poder obtener
    //directamente el tam del fichero una vez abierto
    ifstream file (uri, ios::in | ios::binary | ios::ate);
    Traza::print("loadfromfile:",uri, DEBUG);

    if (file.is_open()){
        try{
            size = file.tellg();//Obtenemos el tamanyo del fichero
            if (tam + ini <= size){
                memblock = new char [tam];//Reservamos memoria
                file.seekg (ini, ios::beg);//Volvemos al principio del fichero
                file.read (memblock, tam);//Leemos el fichero
                file.close();//Cerramos el fichero
                size = tam;
                Traza::print("the complete file content is in memory: size:",size, DEBUG);
            } else {
                Traza::print("loadFromFile: El tam. del fichero es menor que el tam. definido", ERROR);
                Traza::print("uri:",uri, DEBUG);
                Traza::print("size:",size, DEBUG);
                Traza::print("tam + ini:",tam + ini, DEBUG);
                file.close();//Cerramos el fichero
                throw(Excepcion(ERANGO));
            }

        } catch(bad_alloc&) {
            Traza::print("loadFromFile: Memoria insuficiente al reservar:",uri, ERROR);
            file.close();//Cerramos el fichero
            throw(Excepcion(EESPACIO));
        }
    } else {
        Traza::print("loadFromFile: Unable to open file:",uri, ERROR);
        file.close();//Cerramos el fichero
        throw(Excepcion(EFIO));
    }
}


/**
* operaciones de escritura
*
* ios::in	Open for input operations.
* ios::out	Open for output operations.
* ios::binary	Open in binary mode.
* ios::ate	Set the initial position at the end of the file. If this flag is not set to any value, the initial position is the beginning of the file.
* ios::app	All output operations are performed at the end of the file, appending the content to the current content of the file. This flag can only be used in streams open for output-only operations.
* ios::trunc	If the file opened for output operations already existed before, its previous content is deleted and replaced by the new one.
*/
void Fileio::writeToFile(const char *uri, char * memblocktowrite, ifstream::pos_type tam, bool append){

    Traza::print("writeToFile:",uri, DEBUG);
    ofstream file;

    if (append){
        file.open(uri,ios::out | ios::binary | ios::app);
    } else {
        file.open(uri,ios::out | ios::binary | ios::trunc);
    }

    if (file.is_open()){
        file.write(memblocktowrite, tam);
        file.close();
        Traza::print("writeToFile::Fichero escrito correctamente", DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", ERROR);
        file.close();//Cerramos el fichero
        throw(Excepcion(EFIO));
    }

    #ifdef GP2X
        sync();
    #endif


}

/**
*
*/
void Fileio::writeToFileBlock(const char *uri, char * memblocktowrite, ifstream::pos_type tam, ifstream::pos_type blockSize, bool append){

    Traza::print("writeToFile:",uri, DEBUG);
    ofstream file;

    if (tam > 0 and blockSize > 0){

        if (blockSize > tam) {
            Traza::print("writeToFile:", "El bloque era mayor que el tamanyo del fichero", DEBUG);
            blockSize = tam;
        }

        char *reservaMemoria = new char[blockSize];

        if (append){
            file.open(uri,ios::out | ios::binary | ios::app);
        } else {
            file.open(uri,ios::out | ios::binary | ios::trunc);
        }

        if (file.is_open()){
            do{
                memcpy(reservaMemoria, memblocktowrite, blockSize);
                file.write(memblocktowrite, blockSize);
                tam -= blockSize;

                if (tam > 0)
                {
                    if (tam < blockSize ){
                        blockSize = tam;
                    }
                    memblocktowrite += blockSize;
                }

            } while (tam > 0);

            file.close();
            Traza::print("writeToFile::Fichero escrito correctamente", DEBUG);

        } else {
            Traza::print("writeToFile::Unable to write file", ERROR);
            file.close();//Cerramos el fichero
            throw(Excepcion(EFIO));
        }
        #ifdef GP2X
            sync();
        #endif
    } else {
        Traza::print("writeToFile::El tamanyo de bloque o del fichero son incorrectos", ERROR);
    }


}


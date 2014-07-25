#ifndef Dirutil_H
#define Dirutil_H

#include "fileattr.h"
#include "constant.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "traza.h"
#include "listaSimple.h"

    //#define PATH_MAX 4096 /* # chars in a path name including nul */

    class Dirutil{
        public :
            Dirutil(); //Constructor
            ~Dirutil(); //Destructor

            char * getDirActual(); //Recupera el directorio actual de navegacion
            bool changeDirFromActualDir(const char *);
            bool changeToDir(const char *);
            int countDir(DIR *dp);
            void dircat(char *, const char *);
            string getFolder(string file);
            void listarDirRecursivo(string , listaSimple<FileProps> * );
            void listarDir(const char *, listaSimple<FileProps> * );
            string getFile(string );
            string getRutaInicial();
            bool existe(string ruta);
            string cambiarUnidadARuta(string );
            bool borrar(string );
            bool borrarArchivo(string );
            bool isDir(string);

        private :
            char rutaActual[PATH_MAX]; //Ruta actual que se esta navegando
            void comprobarDirPadre(char *);
            char * getDir(char *); //Obtiene el directorio actual y retorna la ruta


    };

#endif //Dirutil_H

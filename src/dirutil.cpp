#include "dirutil.h"

/**
*
*/
Dirutil::Dirutil(){
}

/**
*
*/
Dirutil::~Dirutil(){
}

string Dirutil::getRutaInicial(){
    static string ruta;

    if (ruta.length() <= 0){
        ruta = string(getDirActual());
    }
    return ruta;
}

/**
*
*/
void Dirutil::listarDirRecursivo(string dir, listaSimple<FileProps> * filelist){

    listaSimple<FileProps> * tempfilelist = new listaSimple<FileProps>();

    try{
        listarDir(dir.c_str(), tempfilelist); //Clase que almacena los directorios y ficheros listados
        string tempDir = "";

        if (tempfilelist != NULL){
            for (unsigned int i=0; i < tempfilelist->getSize(); i++){
                tempDir = dir + FILE_SEPARATOR + tempfilelist->get(i).filename;
                if (strcmp(tempfilelist->get(i).filename.c_str(),"..") != 0 ){
                    if (tempfilelist->get(i).filetype[0] == 'D') {
                        listarDirRecursivo(tempDir, filelist);
                    } else {
                        FileProps propFile;
                        propFile.dir = dir;
                        propFile.filename = tempfilelist->get(i).filename;
                        propFile.filetype = "F";
                        filelist->add(propFile);
                        Traza::print("Recursivo Fichero",tempDir.c_str(), PARANOIC);
                    }
                }
            }
            //clear(listadotemp);
            delete tempfilelist;
        }
    } catch(Excepcion &e) {
        Traza::print("Error Dirutil::listarDirRecursivo - ",  e.getMessage(), ERROR);
//        clear(listadotemp);
    delete tempfilelist;
    }
}

/**
* El directorio deberia venir siempre con una / al final
*/
void Dirutil::listarDir(const char *strdir, listaSimple<FileProps> * filelist){
    DIR *dp;
    struct dirent *dirp;
    struct stat info;
    char * concatDir = NULL;
    const char  tempFileSep[2] = {FILE_SEPARATOR,'\0'};

    FileProps propFile;


    try{
        //Miramos a ver si el directorio a explorar tiene una / al final
        if (strdir != NULL){

            int dirlen = sizeof(strdir);
            bool sepDir = false;
            if (dirlen > 0){
                if (strdir[dirlen-1] != FILE_SEPARATOR){
                    dirlen++;
                    sepDir = true;
                }
            }

            if((dp  = opendir(strdir)) == NULL) {
                Traza::print("Error al listar el directorio: ", strdir, ERROR);
                throw(Excepcion(EFIO));
            } else {
                Traza::print("Antes de reservar", PARANOIC);

                while ((dirp = readdir(dp)) != NULL) {
                    concatDir = new char[dirlen+sizeof(dirp->d_name)+1];//Contamos +1 con el fin de cadena
                    strcpy(concatDir,strdir);
                    if (sepDir){
                        strcat(concatDir,tempFileSep);
                    }
                    strcat(concatDir,dirp->d_name);
                    stat(concatDir, &info);

                    if (strcmp(dirp->d_name,".") != 0){
                        propFile.dir = strdir;
                        propFile.filename = dirp->d_name;


                        if(S_ISDIR(info.st_mode)){
                            propFile.filetype = "D";
                        } else {
                            propFile.filetype = "F";
                        }

                        filelist->add(propFile);
                    }
                    delete concatDir;
                }

                Traza::print("Ficheros anyadidos", PARANOIC);
                closedir(dp);
            }
        }

    } catch(Excepcion &e) {
        Traza::print("Error Dirutil::listarDir - ",  e.getMessage(), ERROR);
        throw(e);
    }
}

/**
* Cambiamos la ruta por una ruta que exista en la unidad desde la que se lanza el emulador
*/
string Dirutil::cambiarUnidadARuta(string ruta){
    string tempRuta = ruta;
    //Comprobamos que existan todos los directorios. Tanto la rom como el ejecutable
    if (!existe(ruta)){
        ruta[0] = getRutaInicial()[0];
        if (!existe(ruta)){
            //La ruta tampoco existia aun cambiando la unidad actual. Volvemos a la ruta que teniamos
            ruta = tempRuta;
        }
    }
    return ruta;
}

/**
*
*/
bool Dirutil::borrar(string ruta){
    if (remove(ruta.c_str()) != 0){
        return false;
    } else {
        return true;
    }
}

/**
*
*/
bool Dirutil::borrarArchivo(string ruta){

    if (isDir(ruta))
        return false;
    else {
        if (existe(ruta))
            return (remove(ruta.c_str()) != 0) ? false : true;
        else
            return false;
    }
}

/**
*
*/
int Dirutil::countDir(DIR *dp){
    int nFiles = 0;
    while (readdir(dp) != NULL) {
        nFiles++;
    }
    rewinddir(dp);
    Traza::print("countDir. nFiles: ",nFiles, DEBUG);
    return nFiles;
}

/**
*Con este metodo comprobamos que el directorio al que tengo que ir es el adecuado
*Convertiremos C:\\Archivos de programa\\.. en
* C:\\
*
*/
void Dirutil::comprobarDirPadre(char * dirDestino)
{
    if (dirDestino != NULL)
    {
        const char tempSep[2] = {FILE_SEPARATOR,'\0'};
        char file[FILENAME_MAX];
        strcpy(file,dirDestino);


        int pos = Constant::contiene(dirDestino,FILE_PARENT);


        if (pos > 0){
            int i = pos;
            char letra = 0;
            bool encontrado = false;

            do{
                i--;
                letra = dirDestino[i];
                if (letra == FILE_SEPARATOR) encontrado = true;

            }while (i > 0 && !encontrado);

            if (encontrado){
                memset(file, '\0',FILENAME_MAX-1);

            }
            for (int j=0;j<i;j++){
                file[j] = dirDestino[j];
            }

            //Este es el caso en el que llegamos a la raiz de la unidad
            //Si no hay ningun separador de unidad lo incluimos
            if (Constant::contiene(file,FILE_SEPARATOR) < 0){
                strcat(file,tempSep);
            }
        }
        memset(dirDestino, '\0',FILENAME_MAX-1);
        strcpy(dirDestino,file);
    }
}

/**
* Devuelve true si se ha hecho el cambio al directorio.
* False si no se ha podido hacer el cambio. P.ejm: Cambio por un fichero
*/
bool Dirutil::changeDirFromActualDir(const char *str){
    char ruta[PATH_MAX];
    struct stat info;

    strcpy(ruta,this->getDirActual());
    dircat(ruta,str);
    comprobarDirPadre(ruta);

    //Comprobamos si es un directorio
    stat(ruta, &info);
    if(S_ISDIR(info.st_mode)){
        chdir(ruta);
        return true;
    } else {
        return false;
    }
}


/**
* Devuelve true si se ha hecho el cambio al directorio.
* False si no se ha podido hacer el cambio. P.ejm: Cambio por un fichero
*/
bool Dirutil::changeToDir(const char *str){
    struct stat info;
    //Comprobamos si es un directorio
    stat(str, &info);
    if(S_ISDIR(info.st_mode)){
        chdir(str);
        return true;
    } else {
        return false;
    }
}

/**
*/
void Dirutil::dircat(char *dir, const char *file){
    const char tempSep[2] = {FILE_SEPARATOR,'\0'};
    int dirlen = sizeof(dir);
    if (dirlen > 0){
        if (dir[dirlen-1] != FILE_SEPARATOR){
            strcat(dir,tempSep);
        }
        strcat(dir,file);
    }
}

/**
* Obtiene el directorio de un fichero
*/
string Dirutil::getFolder(string file){
    struct stat info;

    //Comprobamos si es un directorio
    stat(file.c_str(), &info);
    if(S_ISDIR(info.st_mode)){
        return file;
    } else {
        size_t found;
        found = file.find_last_of(FILE_SEPARATOR);
        if (found > 0){
            return file.substr(0,found);
        } else {
            return file;
        }
    }
}

/**
* Comprueba si existe el directorio o fichero pasado por parámetro
*/
bool Dirutil::existe(string ruta){
    struct stat info;

    stat(ruta.c_str(), &info);

    if(S_ISDIR(info.st_mode)){
        return true;
    } else {
        FILE *archivo = fopen(ruta.c_str(), "r");
        if (archivo) {
            fclose(archivo);
            return 1; //TRUE
        } else {
            return 0; //FALSE
        }
    }
}

/**
*
*/
bool Dirutil::isDir(string ruta){
    struct stat info;

    stat(ruta.c_str(), &info);

    if(S_ISDIR(info.st_mode))
        return true;
    else
        return false;
}


/**
*
*/
string Dirutil::getFile(string file){
    struct stat info;

    //Comprobamos si es un directorio
    stat(file.c_str(), &info);
    if(S_ISDIR(info.st_mode)){
        return file;
    } else {
        size_t found;
        found = file.find_last_of(FILE_SEPARATOR);
        if (found > 0){
            return file.substr(found+1,file.length()-1);
        } else {
            return file;
        }
    }
}

/**
*
*/
char * Dirutil::getDir(char *buffer){
    memset(buffer, '\0',FILENAME_MAX-1);
    getcwd(buffer, PATH_MAX);
    return buffer;
}

/**
*
*/
char * Dirutil::getDirActual(){
    return getDir(rutaActual);
}

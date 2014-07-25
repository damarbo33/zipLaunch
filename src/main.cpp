#include <iostream>
#include "main.h"

#define LEER		0
#define ESCRIBIR	1
#define MAXTUBERIALEN 2000


int main(int argc, char **argv)
{
    Launcher *launcher = new Launcher();
    if (launcher->parsearArgumentos(argc, argv)){
        launcher->lanzarProgramaUNIXFork();
    }
    delete launcher;
    exit(0);
}

/**
*
*/
Launcher::Launcher(){
    programa = "";
    this->prog = NULL;
    this->vargs = NULL;
    //unzipedRom = "";
    vparm.clear();
    deleteUnzipedRom(RUTA_TRAZA);
    //Redirigiendo la salida estandar
    desc_fich = open (RUTA_TRAZA, O_CREAT|O_APPEND|O_WRONLY, 0);
    dup2 (desc_fich, 1);		/* Redirige la salida normal */
    dup2 (desc_fich, 2);		/* Redirige la salida de error */
}

/**
*
*/
Launcher::~Launcher(){
    log("Launcher::~Launcher");
    if (this->prog != NULL)
        delete this->prog;
    log("prog deleted");
    delete [] vargs;
    log("vargs deleted");
    close (desc_fich);
}

/**
* Busca en todos los parametros el texto -idprog para obtener el ejecutable
* y el resto de parametros que se le deben concatenar
*/
bool Launcher::parsearArgumentos(int argc, char **argv){
    bool found = false;
    programa = "";

    vparm.clear();

    log(string("parsearArgumentos.NumParametros: ") + Constant::TipoToStr(argc));
    if (argc > 1){
        for (int i = 1; i < argc; i+=1){
//            if (strcmp(argv[i], IDPROGRAMALANZAR) == 0 && i + 1 < argc){
//                found = true;
//                programa = argv[i+1];
//                i++;
//            } else {
//                vparm.push_back(argv[i]);
//            }
            if (i == 1){
                programa = argv[i];
                found = true;
            } else {
                vparm.push_back(argv[i]);
            }
            log(argv[i]);
        }
    }

    return found;
}

/**
* Descomprime el fichero .zip pasado por parametro y devuelve el nombre del fichero extraido
*/
string Launcher::descomprimirZIP(string filename){
    string filenameReturn = filename;
    log("descomprimirZIP");
    if (filename.find(".zip") != string::npos || filename.find(".ZIP") != string::npos){
        //Descomprimir el fichero
        UnzipTool *unzipTool = new UnzipTool();
        unzippedFileInfo salida = unzipTool->descomprimirZip(filename.c_str());
        delete unzipTool;
        //unzipedRom = salida.rutaEscritura;
        filenameReturn = salida.rutaEscritura;
//        if (filenameReturn.find(" ") != string::npos){
//            filenameReturn = "'" + filenameReturn + "'";
//        }
    }
    log("descomprimirZIP FIN");
    return filenameReturn;
}

/**
*
*/
void Launcher::lanzarProgramaUNIXPopen(){
    string buffer = "";
    string comando = programa + " ";
    string fileNameDecompressed = "";
    string strDecompresed = "";
    FILE *fpipe;
    char line[2500];

    for (unsigned int i = 0; i < vparm.size(); i++){
        //Si el parametro es un fichero .zip lo descomprimimos y le pasamos el nombre
        //del fichero descomprimido en lugar del .zip
        strDecompresed = descomprimirZIP(vparm[i]);

        if (vparm[i].compare(strDecompresed) != 0){
            //Detectamos que hemos extraido el fichero
            log(RUTA_TRAZA_HIJO, "Hijo: Detectada extraccion de fichero");
            fileNameDecompressed = strDecompresed;
        }

        comando.append(strDecompresed);
        if (i < vparm.size() -1)
            comando.append(" ");
    }

    cout << "comando: " << comando << endl;
    if ( !(fpipe = (FILE*)popen(comando.c_str(),"r")) ){
        perror("Problems with pipe");
        exit(1);
    }

    //while ( fgets( line, sizeof line, fpipe)){
    while ( fgets( line, sizeof line, fpipe)){
        cout << line << endl;
    }
    pclose(fpipe);

    deleteUnzipedRom(fileNameDecompressed);
}

/**
* Realiza un lanzamiento del programa que se encontro al parsearArgumentos
*/
bool Launcher::lanzarProgramaUNIXFork(){

    #ifdef UNIX
        log( "*************Launcher::lanzarProgramaUNIX*************");
        if (this->programa.compare("") != 0){
            int child_status;
            /* Buffer para escribir o leer caracteres de la tuberia */
            string buffer = "";

            /* Se crea el proceso hijo. En algún sitio dentro del fork(), nuestro
             * programa se duplica en dos procesos. Cada proceso obtendrá una salida
             * distinta. */
            pid_t pID = vfork(); //vfork - create a child process and block parent

            /* fork() devuelve 0 al proceso hijo.*/
            if (pID == 0){

                // Code only executed by child process
                log( RUTA_TRAZA_HIJO, "Hijo: Proceso Hijo Inicio");
                //Creamos el array de parametros en vargs y obtenemos el nombre del fichero
                //extraido (si lo hay) en la variable buffer
                buffer = crearArrayParametros();
                //Ejecutamos el programa
                int execReturn = execv(this->prog, this->vargs);
                //Aqui no llegamos a no ser que vaya algo mal en la ejecucion
                if(execReturn == -1){
                    log(RUTA_TRAZA_HIJO, "ERROR al ejecutar execv: " + getErrMsg(errno));
                     _exit(-1);
                }
                _exit(0); //_exit - - terminate the current process
            }
            /* Si fork() devuelve -1, es que hay un error y no se ha podido crear el
             * proceso hijo. */
            else if (pID < 0){            // failed to fork
                cerr << "Failed to fork" << endl;
                log( "lanzarProgramaUNIX Failed to fork");
                exit(1);
                 // Throw exception
            }
            /* fork() devuelve un número positivo al padre. Este número es el id del
             * hijo. */
            else {   // parent

                /* This is run by the parent.  Wait for the child
                to terminate. */
                log("Padre: Esperando a la muerte del hijo");
                waitpid(pID, &child_status, 0);

                /* Comprueba la salida del hijo */
                if (WIFEXITED(child_status) != 0){
                    log("Padre: Proceso Hijo terminado. Recibido: " + buffer);
                    deleteUnzipedRom(buffer);
                } else {
                    log("Padre: ERROR EN LA SALIDA DEL HIJO");
                }
            }
        } else {

             log( "No se ha encontrado ningun programa que lanzar");
        }

    #else
        log( "*************Launcher::lanzarProgramaUNIX - En Windows*************");
        crearArrayParametros();
    #endif
    // executed only by parent
    log("lanzarProgramaUNIX Fin");
    return true;
}


/**
*
*/
string Launcher::crearArrayParametros(){
    //Todos los parametros puestos en un string con motivo informativo.
    //Para mostrar por pantalla lo que se va a lanzar
    string strParms = "";
    //Para comprobar si se ha extraido un fichero y que nombre tiene al extraerse
    string strDecompresed = "";
    //Ruta y nombre del fichero extraido del .zip
    string fileNameDecompressed = "";
    //Ruta y nombre del ejecutable especificado por parametros
    prog = new char[programa.length()+1];
    strcpy(prog, programa.c_str());

    string paramet = "";
    //Vamos rellenando el array vargs con todos los parametros necesarios para ejecutar el programa
    //Que se encuentran almacenados en vparm
    bool foundIdRom = false;
    bool foundZip = false;
    string zipFullName = "";
    vector<string> vPath;

    for (unsigned int i = 0; i < vparm.size(); i++){
        paramet = vparm[i];
        log(RUTA_TRAZA_HIJO, "paramet: " + paramet);

        if (paramet.compare("-idRom") == 0){
            foundIdRom = true;
        } else {
            if (foundIdRom == true){
                zipFullName.append(paramet);
                if (paramet.find(".zip") != string::npos || paramet.find(".ZIP") != string::npos){
                    foundZip = true;
                    log(RUTA_TRAZA_HIJO, "ZIP encontrado");
                } else {
                    zipFullName.append(" ");
                }
            }

            if (foundIdRom && foundZip){
                //Si el parametro es un fichero .zip lo descomprimimos y le pasamos el nombre
                //del fichero descomprimido en lugar del .zip
                log(RUTA_TRAZA_HIJO, "Descomprimiendo la ROM: " + zipFullName);
                strDecompresed = descomprimirZIP(zipFullName);
                log(RUTA_TRAZA_HIJO, "ROM descomprimida: " + strDecompresed);

                if (zipFullName.compare(strDecompresed) != 0){
                    //Detectamos que hemos extraido el fichero
                    log(RUTA_TRAZA_HIJO, "Hijo: Detectada extraccion de fichero");
                    fileNameDecompressed = strDecompresed;
                    paramet = strDecompresed;
                    foundZip = false;
                    foundIdRom = false;
                }
            }
        }

        if ((!foundZip && !foundIdRom) || i == vparm.size() -1){
            //this->vargs[i+1] = new char[paramet.length()+1];
            //strcpy(this->vargs[i+1], paramet.c_str());
            vPath.push_back(paramet);
            strParms.append(paramet);
            strParms.append(" ");
        }
    }


    //Creando un array con: programa a ejecutar, parametros pasados y 0
    this->vargs = new char *[vPath.size() + 2];
    //En la primera posicion del array debe ir el la ruta y el nombre del ejecutable
    this->vargs[0] = new char[programa.length()+1];
    this->vargs[0] = prog;

    for(int unsigned i=0; i < vPath.size(); i++){
            this->vargs[i+1] = new char[vPath[i].length()+1];
            strcpy(this->vargs[i+1], vPath[i].c_str());
    }
    //En la ultima posicion debe ir un 0
    this->vargs[vPath.size() + 1] = new char[2];
    this->vargs[vPath.size() + 1] = (char *) 0;

    log(RUTA_TRAZA_HIJO, "------------------------------------------------------------");
    log(RUTA_TRAZA_HIJO, "Hijo: Lanzando: " +  programa + " " + strParms);
    log(RUTA_TRAZA_HIJO, "------------------------------------------------------------");
    return fileNameDecompressed;
}


/**
*
*/
void Launcher::log(string msg){
    cout << Constant::fecha() << string(" - ") << msg << endl;
}

/**
*
*/
void Launcher::log(string ruta, string msg){
    ofstream myfile (ruta.c_str(), fstream::out | fstream::app);
    if (myfile.is_open()){
        myfile << Constant::fecha() << string(" - ") << msg << VBCRLF;
        myfile.close();
    }
}

/**
*
*/
void Launcher::deleteUnzipedRom(string romfile){
    if (romfile.compare("") != 0 && romfile.find(".zip") == string::npos && romfile.find(".ZIP") == string::npos){
        Dirutil *dirutil=new Dirutil();
        //dirutil->borrar(romfile);
        if (dirutil->borrarArchivo(romfile))
            log( "deleteUnzipedRom Fichero eliminado: " + romfile);
        else
            log( "No se ha encontrado el fichero a eliminar: " + romfile);

        delete dirutil;
    }
}

/**
*
*/
void Launcher::writeToFile(string ruta, string msg){

//    ofstream myfile (ruta.c_str(), std::fstream::out | std::fstream::app);
//
//    if (myfile.is_open()){
//        //myfile << msg);
//        log( msg);
//        myfile.close();
//    } else {
//         myfile.close();
//    }
//    #ifdef UNIX
//        sync();
//    #endif

    FILE *fout = fopen(ruta.c_str(),"at+");
    msg = msg + VBCRLF;

    fwrite(msg.c_str(), sizeof(char) , msg.length(), fout);

    if (fout != NULL){
        fclose(fout);
        fout = NULL;
    }
}

/**
*
*/
std::string Launcher::getErrMsg(int errnum){

        switch ( errnum ) {

    #ifdef EACCES
            case EACCES :
            {
                return "EACCES Permission denied";
            }
    #endif

    #ifdef EPERM
            case EPERM :
            {
                return "EPERM Not super-user";
            }
    #endif

    #ifdef E2BIG
            case E2BIG :
            {
                return "E2BIG Arg list too long";
            }
    #endif

    #ifdef ENOEXEC
            case ENOEXEC :
            {
                return "ENOEXEC Exec format error";
            }
    #endif

    #ifdef EFAULT
            case EFAULT :
            {
                return "EFAULT Bad address";
            }
    #endif

    #ifdef ENAMETOOLONG
            case ENAMETOOLONG :
            {
                return "ENAMETOOLONG path name is too long     ";
            }
    #endif

    #ifdef ENOENT
            case ENOENT :
            {
                return "ENOENT No such file or directory";
            }
    #endif

    #ifdef ENOMEM
            case ENOMEM :
            {
                return "ENOMEM Not enough core";
            }
    #endif

    #ifdef ENOTDIR
            case ENOTDIR :
            {
                return "ENOTDIR Not a directory";
            }
    #endif

    #ifdef ELOOP
            case ELOOP :
            {
                return "ELOOP Too many symbolic links";
            }
    #endif

    #ifdef ETXTBSY
            case ETXTBSY :
            {
                return "ETXTBSY Text file busy";
            }
    #endif

    #ifdef EIO
            case EIO :
            {
                return "EIO I/O error";
            }
    #endif

    #ifdef ENFILE
            case ENFILE :
            {
                return "ENFILE Too many open files in system";
            }
    #endif

    #ifdef EINVAL
            case EINVAL :
            {
                return "EINVAL Invalid argument";
            }
    #endif

    #ifdef EISDIR
            case EISDIR :
            {
                return "EISDIR Is a directory";
            }
    #endif

    #ifdef ELIBBAD
            case ELIBBAD :
            {
                return "ELIBBAD Accessing a corrupted shared lib";
            }
    #endif

            default :
            {
                std::string errorMsg(strerror(errnum));
                if ( errnum ) return errorMsg;
                else return "";
            }
         }
}

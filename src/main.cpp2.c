#include <iostream>

#include "main.h"


int main(int argc, char **argv)
{
    Traza *miTraza = new Traza();
    miTraza->print("main","**** Iniciando lanzador", DEBUG);
    Launcher *launcher = new Launcher();
    if (launcher->parsearArgumentos(argc, argv)){
        launcher->lanzarProgramaUNIX();
    }

    miTraza->print("main","**** exit(0)", DEBUG);
    exit(0);
}

/**
*
*/
Launcher::Launcher(){
    programa = "";
    unzipedRom = "";
    vparm.clear();
    delete miTraza;
}

/**
*
*/
Launcher::~Launcher(){
}

/**
*
*/
bool Launcher::parsearArgumentos(int argc, char **argv){
    bool found = false;
    programa = "";
    unzipedRom = "";

    vparm.clear();

    if (argc > 1){
        for (int i = 1; i < argc; i+=1){
            if (strcmp(argv[i], IDPROGRAMALANZAR) == 0 && i + 1 < argc){
                found = true;
                programa = argv[i+1];
                i++;
            } else {
                vparm.push_back(argv[i]);
            }
        }
    }

    return found;
}

/**
*
*/
string Launcher::descomprimirZIP(string filename){
    string filenameReturn = filename;
    if (filename.find(".zip") != string::npos || filename.find(".ZIP") != string::npos){
//        miTraza->print("descomprimirZIP","Descomprimiendo: " + filename, DEBUG);
        //Descomprimir el fichero
        UnzipTool *unzipTool = new UnzipTool();
        unzippedFileInfo salida = unzipTool->descomprimirZip(filename.c_str());
        delete unzipTool;
//        miTraza->print("descomprimirZIP", "Codigo: " + Constant::TipoToStr(salida.errorCode) + " ruta: " + Constant::TipoToStr(salida.rutaEscritura), DEBUG);
        unzipedRom = salida.rutaEscritura;
        filenameReturn = salida.rutaEscritura;
    }
    return filenameReturn;
}

/**
*
*/
bool Launcher::lanzarProgramaUNIX(){
    miTraza->print("lanzarProgramaUNIX", "Inicio", DEBUG);

    int child_status;
    string strParms = "";
    string strDecompresed = "";

    /* Descriptores para la tuberia. El 0 será de entrada y el 1 de salida */
	int descriptorTuberia[2];

	/* Buffer para escribir o leer caracteres de la tuberia */
	char buffer[FILENAME_MAX+1];
	strcpy(buffer, "");

    /* Antes de llamar al fork(), Se crea una tuberia, con dos descriptores,
	 * uno de entrada y otro de salida */
	if (pipe (descriptorTuberia) == -1){
		perror ("No se puede crear Tuberia");
		miTraza->print("lanzarProgramaUNIX", "No se puede crear Tuberia", ERROR);
		exit (-1);
	}

    /* Se crea el proceso hijo. En algún sitio dentro del fork(), nuestro
	 * programa se duplica en dos procesos. Cada proceso obtendrá una salida
	 * distinta. */
    pid_t pID = fork(); //vfork - create a child process and block parent

    /* fork() devuelve 0 al proceso hijo.*/
    if (pID == 0){
      // child
      // Code only executed by child process

      /* Cierra el descriptor de lectura puesto que no lo va a usar. */
		close (descriptorTuberia[0]);

        char *prog = new char[programa.length()+1];
        strcpy(prog, programa.c_str());
//      char *args[] = {"/bin/ls", "-r", "-t", "-l", (char *) 0 };
//	    execv("/bin/ls", args);

        //Creando un array con: programa a ejecutar, parametros pasados y 0
        char *vargs[vparm.size() + 2];

        vargs[0] = new char[programa.length()+1];
        vargs[0] = prog;
        vargs[vparm.size() + 1] = (char *) 0;

        for (unsigned int i = 0; i < vparm.size(); i++){
            vargs[i+1] = new char[vparm[i].length()+1];
            strDecompresed = descomprimirZIP(vparm[i]);
            strcpy(vargs[i+1], strDecompresed.c_str());

            if (strcmp(vargs[i+1], vparm[i].c_str()) != 0){
                //Detectamos que hemos extraido el fichero
                /* Escribe por el descriptor de escritura */
                strcpy (buffer, strDecompresed.c_str());
                write (descriptorTuberia[1], buffer, strlen(buffer)+1);
            }
            strParms = strParms + vargs[i+1] + " ";
        }
//        miTraza->print("lanzarProgramaUNIX", "lanzando: " + Constant::TipoToStr(prog) + " " + strParms, DEBUG);
        int execReturn = execv(prog, vargs);

        if(execReturn == -1){
//            miTraza->print("lanzarProgramaUNIX", "Failure! execv error code=" + errno, DEBUG);
//            miTraza->print("lanzarProgramaUNIX", getErrMsg(errno), ERROR);
             _exit(-1);
        }

        delete prog;

        for (unsigned int i = 0; i <= vparm.size(); i++){
            delete vargs[i];
        }

//        miTraza->print("lanzarProgramaUNIX", "Terminando execv", DEBUG);
        _exit(0); //_exit - - terminate the current process
    }
    /* Si fork() devuelve -1, es que hay un error y no se ha podido crear el
	 * proceso hijo. */
    else if (pID < 0){            // failed to fork
        cerr << "Failed to fork" << endl;
        miTraza->print("lanzarProgramaUNIX", "Failed to fork", ERROR);
        exit(1);
         // Throw exception
    }
    /* fork() devuelve un número positivo al padre. Este número es el id del
	 * hijo. */
    else {   // parent

        /* This is run by the parent.  Wait for the child
        to terminate. */
        /* Espera un segundo (para dar tiempo al hijo a hacer sus cosas y no
		 * entremezclar salida en la pantalla) y escribe su pid y el de su hijo */
		sleep (1);
		/* Cierra el descriptor de escritura puesto que no lo va a usar. */
		close (descriptorTuberia[1]);
        /* Espera que el hijo muera */
		wait (&child_status);

		/* Comprueba la salida del hijo */
		if (WIFEXITED(child_status) != 0){
			miTraza->print("lanzarProgramaUNIX", "Padre : Mi hijo ha salido", DEBUG);
			/* Lee caracteres del descriptor de lectura, escribe el resultado en
             * pantalla y sale */
            read (descriptorTuberia[0], buffer, FILENAME_MAX);
            miTraza->print("lanzarProgramaUNIX", "Hijo  : Recibido " + Constant::TipoToStr(buffer), DEBUG);
            deleteUnzipedRom(buffer);
		} else {
            miTraza->print("lanzarProgramaUNIX", "Padre : ERROR EN LA SALIDA DEL HIJO", ERROR);
		}

        //pid_t tpid;
//        do {
//            tpid = wait(&child_status);
//            //if(tpid != pID) process_terminated(tpid);
////            if (isDecompresed){
////                Dirutil *dirutil=new Dirutil();
////                dirutil->borrar(strDecompresed);
////                cout << "Fichero eliminado: " << strDecompresed << endl;
////                delete dirutil;
////            } else {
////                cout << "No hay que borrar el fichero"<< endl;
////            }
//            if ( WIFEXITED(child_status) != 0)
//            {
//                printf ("Mi hijo ha hecho exit (%d)\n", WEXITSTAUS (child_status));
//            }
//        } while(tpid != pID);
        // Code only executed by parent process
    }

    // executed only by parent
    miTraza->print("lanzarProgramaUNIX", "Fin", DEBUG);
    return true;
}

/**
*
*/
void Launcher::deleteUnzipedRom(string romfile){
    if (romfile.compare("") != 0 && romfile.find(".zip") == string::npos && romfile.find(".ZIP") == string::npos){
        Dirutil *dirutil=new Dirutil();
        dirutil->borrar(romfile);
//        miTraza->print("deleteUnzipedRom", "Fichero eliminado" + romfile, DEBUG);
        delete dirutil;
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

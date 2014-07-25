#include "excepcion.h"

const char* Excepcion::getMessage(){
    if (code < 1000){
        return sqlMsg.c_str();
    } else {
        switch (code){
            case ERANGO:   return "Error de Rango";break;
            case EESPACIO: return "No se ha reservado suficiente memoria";break;
            case ENULL: return "Se ha intentado acceder a un objeto NULL";break;
            case EFIO: return "Se ha producido un error al acceder a un fichero";break;
            default:       return "Error Desconocido";  //En rigor no debería ocurrir
        }
    }



    return ""; //Evita warnings
}

/**
* Obtenemos el codigo de error lanzado
*/
const int Excepcion::getCode()
{
    return code;
}

/**
* Constructor
*/
Excepcion::Excepcion(int m){
    code=m;
    string dato = "Excepcion: ";
    dato.append(Constant::TipoToStr(getCode()));
    dato.append(";");
    dato.append(getMessage());
    Traza::print(dato.c_str(), ERROR);
}

/**
* Constructor
*/
Excepcion::Excepcion(int motivo, string msg){
    code = motivo;
    sqlMsg = msg;

    Traza::print(string("Excepcion::Excepcion: " + msg).c_str(),code, ERROR);
}

/**
* Constructor
*/
Excepcion::Excepcion(){
}

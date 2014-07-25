#include "tcstring.h" // class's header file

/**
* Constructor
*/
TCString::TCString(){
    string = NULL;
    clear();
}

TCString::TCString(const char * str){
    string = NULL;
    clear();
    add(str);
}

/**
* Destructor
*/
TCString::~TCString(){
    clear();
}

/**
* Borramos el contenido del string
*/

void TCString::clear(){
    if (string != NULL){
        delete string;
    }
    string = new char[2]; //String vacio + fin de cadena
    strcpy(string,"");
}

void TCString::add(const char *dato)
{
    if (dato != NULL)
    {
        char *tempString = new char[strlen(string)+strlen(dato)+1]; //+1 para fin de cadena
        strcpy(tempString, string);
        strcat(tempString, dato);

        delete string;
        string = new char[strlen(tempString)+1]; //+1 para fin de cadena
        strcpy(string, tempString);
    }
}

char * TCString::get ()
{
   return string;
}

void TCString::trim()
{
    trim(string);
}

void TCString::trim(char *dato)
{
    int tam = strlen(dato);
    int i = tam - 1;
    bool salir = false;

    int j=0;

    while (!salir && tam > 1 && j < tam)
    {
        if (dato[0] == ' ')
        {
            //printf("Encontramos un espacio en %d\n",j);
            for (int k=1;k<tam;k++)
            {
                dato[k-1] = dato[k];
            }
            if (tam > 1) dato[tam-1] = ' ';
        }
        else
        {
            salir = true;
        }
        j++;
    }

    if (i > 0)
    {
        //temp[0] = dato[i];
        while (i > 0 && dato[i] == ' ' )
        {
            dato[i] = '\0';
            i--;
        }
    }
}

void TCString::toLowerCase ()
{
    toLowerCase(string);
}

void TCString::filtrarLetras ()
{
    filtrarLetras (string);
}

void TCString::filtrarLetras (char *varDato)
{
    if (varDato != NULL)
    {
        int tam = strlen(varDato);
        unsigned char leido = ' ';


        for (int i=0;i<tam;i++)
        {

            leido = varDato[i];
            //Los caracteres que no pueden ser letras, seran eliminados con un espacio
            if ((int)leido < 65 || ( (int)leido > 90 && (int)leido < 97) || ((int)leido > 122 && (int)leido < 192) )
            {
                varDato[i] = ' ';
            }
        }
    }
}

void TCString::toLowerCase (char *varDato)
{
    if (varDato != NULL)
    {
        int tam = strlen(varDato);
        unsigned char leido = ' ';

        //char *temp = new char[tam+1];
        //strcpy(temp,varDato);

        for (int i=0;i<tam;i++)
        {

            leido = varDato[i];
            //Hacemos que todas las palabras esten en minusculas
            if (((int)leido >= 65 && (int)leido <= 90) || ((int)leido >= 192 && (int)leido <= 221))
            {
                varDato[i] = (int)leido+32;
            }
        }
    }
    //strcpy(varDato,temp);
}




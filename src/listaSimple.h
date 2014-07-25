//Se implementa la clase en el .h para que g++ pueda linkar correctamente
//el template. Sino da errores en compilacion

#ifndef LISTASIMPLE
#define LISTASIMPLE

#include <string.h>
#include <stdio.h>
#include "constant.h"
#include "excepcion.h"
#include "fileio.h"
#include <typeinfo>

using namespace std;


class Data{
    public:
//        Data(){};
//        ~Data(){};

        string getKey(){
            return key;
        };

        void setKey(string str){
            key = str;
        };
        string getValue(){
            return value;
        };

        void setValue(string str){
            value = str;
        };

        void addValue(string str){
            vector<string> x;
            x = Constant::split(str, '=');
                if (x.size() > 1){
                    this->key = x[0];
                    this->value = x[1];
                } else {
                    this->key = str;
                    this->value = "";
                }
        };

    private:
        string key;
        string value;
};

class DataList{
    public:
//        DataList(){valores = new vector<Data>();};
//        ~DataList(){delete valores;};

        void initValores(){
            valores = new vector<Data>();
        };

        void addValue(string str){
             Data *campo = new Data();
             campo->addValue(str);
             if (valores != NULL){
                this->valores->push_back(*campo);
             }
        };

        Data getSubElem(int i){
            return this->valores->at(i);
        };

        string getKey(){
            return key;
        };

        void setKey(string str){
            key = str;
        };

    private:
        string key;
        vector<Data> *valores;
};


/**
* Definicion de las clases para almacenar los valores de la lista
*/
template<class TIPO> class listaSimple;

template<class TIPO>
class nodoSimple {
   public:
    nodoSimple(){};
    ~nodoSimple(){};

    TIPO getValor(){ return valor;}
    void setValor(TIPO v){ valor = v;}
    //int comparar(TIPO);
    //Los metodos de comparar es mejor que los implemente uno a uno por temas de rendimiento.
    //Sino estaría obligado a parsear los propios strings sin hacer falta
    int comparar(int);
    int comparar(string);
    int comparar(DataList);

   protected:
        TIPO valor;

   friend class listaSimple<TIPO>;
};

template<class TIPO> int nodoSimple<TIPO>::comparar(int t){
        if (t > getValor()) return -1;
        else if (t < getValor()) return 1;
        else return 0;
}

template<class TIPO> int nodoSimple<TIPO>::comparar(string t){
        return getValor().compare(t);
}

template<class TIPO> int nodoSimple<TIPO>::comparar(DataList t){
        return getValor().getKey().compare(t.getKey());
}


template<class TIPO>
class listaSimple : public Excepcion, public Fileio{
   public:
    listaSimple();
    listaSimple(unsigned int);//Es mucho mas rapido si creamos de antemano la lista de elementos
    ~listaSimple();
    TIPO get(unsigned int);
    void add(TIPO);
    void del(unsigned int);
    void setIncremento(int);
    void loadTipoFromFile(string);
    unsigned int loadStringsFromFile(string);
    void writeToFile(string);
    unsigned int getSize(){return size;}
    void sort(); //El metodo sort solo funciona con una lista de strings
    int find(string);

    void clear(){
        if (getSize() > 0){
            delElements(plista);
            inicializar(INILIST);
        }
    };


   protected:
    void QuickSort(nodoSimple<TIPO> *, int, int , int);
    void inicializar(unsigned int);
    void reservar(unsigned int);
    void reservar();
    void delElements(nodoSimple<TIPO> *);
    nodoSimple<TIPO> *plista;
    unsigned int size; //Tamanyo de la lista
    unsigned int totalReserva;  //Especifica cuanto espacio hemos reservado
    int incremento; //Define cuanto espacio reservaremos en el momento de anyadir elementos.
    int BuscarBin(nodoSimple<TIPO> *, int , int , string );

};

/**********************************************************
* IMPLEMENTACION
**********************************************************/

/**
* Destructor
*/
template<class TIPO> listaSimple<TIPO>::~listaSimple(){
    delElements(plista);

}

/**
* Constructor
*/
template<class TIPO> listaSimple<TIPO>::listaSimple(){
    inicializar(INILIST);
}

/**
* Constructor
*/
template<class TIPO> listaSimple<TIPO>::listaSimple(unsigned int length){
    inicializar(length);
}

/**
*/
template<class TIPO> void listaSimple<TIPO>::setIncremento(int value){
    incremento = value;
}

/**
*
*/
template<class TIPO> void listaSimple<TIPO>::inicializar(unsigned int length){
    size = 0;         //Iniciamos el tamanyo del vector
    totalReserva = 0; //Creamos la reserva especificada

    if (length > INILIST){
        incremento = length;    //Si sobrepasamos el tamanyo definido del array, vamos creando tantos elementos nuevos
                                //como indique esta variable
    } else {
        incremento = INILIST;
    }

    reservar(length);
}

template<class TIPO> void listaSimple<TIPO>::delElements(nodoSimple<TIPO> *elems){
     if (elems != NULL){
         if (getSize() > 0)
         delete [] elems;

    }
}

/**
* Elimina un elemento de la posición de la lista pasada por parametro
* TODO: Si hay demasiados elementos en reserva, estaria bien liberar memoria
*/
template<class TIPO> void listaSimple<TIPO>::del(unsigned int pos){

    if (size > pos && size > 0){
        for (unsigned int i = pos; i < size; i++){
            if (i < size - 1){
                plista[i] = plista[i+1];
            }
        }
        size--;
    } else {
         throw(Excepcion(ERANGO));
    }
}


template<class TIPO> TIPO listaSimple<TIPO>::get(unsigned int pos){
     if (size > pos){
        return plista[pos].getValor();
     }
     else{
         throw(Excepcion(ERANGO));
     }
     throw(Excepcion(ERANGO));
}

/**
*/
template<class TIPO> void listaSimple<TIPO>::reservar(){
    try{
        reservar(incremento);
    }catch(Excepcion &e){
        Traza::print("Error capturado al reservar memoria", ERROR);
        throw(Excepcion(EESPACIO));
    }
}


/**
* Reserva el espacio de memoria necesario en la lista cuando vemos que sea necesario
*/
template<class TIPO> void listaSimple<TIPO>::reservar(unsigned int length){

    if (size >= totalReserva){
        Traza::print("Se reservan:",totalReserva + length, PARANOIC);
        Traza::print("size:",size, PARANOIC);
        totalReserva += length;
        try{
            nodoSimple<TIPO> *f = new nodoSimple<TIPO>[size]; // Array temporal de ns punteros a char
            if (size > 0){
                // Cada elemento del array es un array de lenTexto chars
                // size sera menor siempre que el numero de elementos reservados
                // Copiamos los elementos en el nuevo array
                for(unsigned int i = 0; i < size; i++){
                    f[i].setValor(plista[i].getValor());
                }

                this->delElements(plista);
                plista = NULL;
            }

                //Creamos los elementos que necesitamos en reserva
                plista = new nodoSimple<TIPO>[totalReserva];

            if (size > 0){
                //copiamos el resto de elementos nuevamente
                for(unsigned int i = 0; i < size; i++){
                    plista[i].setValor(f[i].getValor());
                }
                this->delElements(f);
            }
        } catch(bad_alloc&) {
            Traza::print("listaSimple::Error capturado al reservar memoria", ERROR);
            throw(Excepcion(EESPACIO));
        }
    }



}

/**
* Anyade un elemento al final de la lista
*/
template<class TIPO> void listaSimple<TIPO>::add(TIPO datoEntrada)
{
    //Comprobamos si es necesario reservar más memoria
    try{
        reservar();
    }catch(Excepcion &e){
        Traza::print("listaSimple::Error add capturado al reservar memoria", ERROR);
        throw(Excepcion(EESPACIO));
    }

    //incluimos la cadena
    if (size < totalReserva){
        nodoSimple<TIPO> myNode;
        myNode.setValor(datoEntrada);
        plista[size] = myNode;
        size++;
    } else {
         Traza::print("listaSimple::Error add capturado al asignar datos", ERROR);
         throw(Excepcion(EESPACIO));
    }
}

/**
*
*/
template<class TIPO> void listaSimple<TIPO>::loadTipoFromFile(string ruta){

    string str;
	ifstream infile;

	Fileio *fio = new Fileio();
    int nlines = fio->contarLineas(ruta.c_str());
    inicializar(nlines);

	infile.open (ruta.c_str());

    if (infile.is_open()){
        while(!infile.eof()) {// To get you all the lines.
            getline(infile,str); // Saves the line in STRING.
            add(Constant::strToTipo<TIPO>(str));
        }
        infile.close();
        delete fio;
    } else {
         Traza::print("listaSimple.loadTipoFromFile: Unable to open file:",ruta.c_str(), ERROR);
         infile.close();
         delete fio;
         throw(Excepcion(EFIO));
    }
}

template<class TIPO> unsigned int listaSimple<TIPO>::loadStringsFromFile(string ruta){

    string str;
	ifstream infile;

	Fileio *fio = new Fileio();
    unsigned int nlines = fio->contarLineas(ruta.c_str());
    inicializar(nlines);

	infile.open (ruta.c_str());

    if (infile.is_open()){
        while(!infile.eof()) {// To get you all the lines.
            getline(infile,str); // Saves the line in STRING.
            add(str);
        }
        infile.close();
        delete fio;
        return nlines;
    } else {
         Traza::print("listaSimple.loadStringsFromFile: Unable to open file:",ruta.c_str(), ERROR);
         infile.close();
         delete fio;
         throw(Excepcion(EFIO));
         return 0;
    }
}


/**
*
*/
template<class TIPO> void listaSimple<TIPO>::writeToFile(string ruta){

    ofstream myfile (ruta.c_str());

    if (myfile.is_open()){
        for (unsigned int i = 0; i < size; i++){
            myfile << get(i) << VBCRLF;
        }
        myfile.close();
    } else {
         Traza::print("List.writeToFile: Unable to open file:",ruta.c_str(), ERROR);
         myfile.close();
         throw(Excepcion(EFIO));
    }
    #ifdef GP2X
        sync();
    #endif
}

/**
*
*/
template<class TIPO> void listaSimple<TIPO>::sort(){
    if (getSize() > 0)
    {
        QuickSort(plista, getSize()-1, 0, getSize()-1);
    }
}

/**
*
*/
template<class TIPO> void listaSimple<TIPO>::QuickSort(nodoSimple<TIPO> *A, int AHigh, int iLo, int iHi)
{
  int Lo, Hi;
  TIPO Mid;
  nodoSimple<TIPO> T;
  Lo = iLo;
  Hi = iHi;

  Mid = A[(Lo+Hi)/2].getValor();

  do
  {
    while (A[Lo].comparar(Mid) < 0)
        Lo++;
    while (A[Hi].comparar(Mid) > 0)
        Hi--;
    //free(Mid);
    if (Lo <= Hi)
    {
      T=A[Lo];
      A[Lo] = A[Hi];
      A[Hi]=T;
      Lo++;
      Hi--;
    }
  }
  while (Lo <= Hi);

  if (Hi > iLo)
    QuickSort(A, AHigh, iLo, Hi);
  if (Lo < iHi)
    QuickSort(A, AHigh, Lo, iHi);
}

/**
*
*/
template<class TIPO> int listaSimple<TIPO>::find(string palabra)
{
    return BuscarBin(plista,0, getSize() - 1, palabra);
}

/**
*
*/
template<class TIPO> int listaSimple<TIPO>::BuscarBin(nodoSimple<TIPO> *datos, int izq, int der, string palabra)
{
    bool encontrado=false;
    int centro=0,i=0;


    while ( (!encontrado) && (izq <= der) )
    {
        i++;
        centro=(izq+der)/2;
    //        printf("centro: %d, i: %d, d: %d, %s, total: %d\n",centro,izq,der,datos[centro].palabra,size());

        if (palabra.compare(Constant::TipoToStr(datos[centro].getValor())) == 0)
        {
            encontrado=true;
            //printf("finded: %s %d\n",datos[centro].palabra,centro);

        }
        else
        {
            if (palabra.compare(Constant::TipoToStr(datos[centro].getValor())) > 0)
                izq=centro+1;
            else
                der=centro-1;
        }
    }
    // El algoritmo de búsqueda binaria encuentra el número, pero no
    // asegura que sea el primero si hay repeticiones de números. Por eso
    // retrocedemos hasta el primer elemento
     if (encontrado && centro > 0)
     while(centro > 0 && palabra.compare(Constant::TipoToStr(datos[centro-1].getValor())) == 0)
           centro--;

    if (encontrado) return (centro);
    else return(-1);
}


#endif


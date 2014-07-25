#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED

#include <sstream>
#include <string.h>
#include <vector>
#include <typeinfo>
#include <ctime>


using namespace std;

#define PI 3.14159265359
#define MOUSE_BUTTON_LEFT		1
#define MOUSE_BUTTON_MIDDLE	2
#define MOUSE_BUTTON_RIGHT	3
#define MOUSE_BUTTON_WHEELUP	4
#define MOUSE_BUTTON_WHEELDOWN	5

    struct t_region{
        int selX;
        int selY;
        int selW;
        int selH;
    };

    struct t_color{
        int r;
        int g;
        int b;
    };

    struct tEvento{
        int key;
        int keyMod;
        int joy;
        int mouse;
        int mouse_x;
        int mouse_y;
        int mouse_state;
        t_region region;
        bool isMousedblClick;
        bool resize;
        bool isJoy;
        bool isKey;
        bool isMouse;
        bool isMouseMove;
        bool isRegionSelected;
        bool quit;
    };

    typedef enum {
        FATAL,     //0
        ERROR,     //1
        WARN,      //2
        INFO,      //3
        DEBUG,     //4
        PARANOIC   //5
    } enumTraza;

     typedef enum {
        BOTTOM,     //0
        TOP,     //1
        LEFT,      //2
        RIGHT,      //3
        CLIENT     //4
    } alignment;

    typedef enum {
        INPUTBUTTON,     //0
        INPUTSTRING,     //1
        INPUTTEXT,      //2
        LISTBOX,      //3
    } tipoObj;

    typedef enum {
        page_white_text,
        folder,
        page_white,
        bullet_go,
        door_out,
        bullet_wrench,
        clockIco,
        page_find,
        folder_explore,
        magnifier,
        chart_bar,
        cross,
        tick,
        font,
        font_add,
        color_swatch,
        text_align_justify,
        layout_sidebar,
        clock_delete,
        book_go,
        bullet_black,
        page_white_gear,
        page_white_compressed,
        page_white_acrobat,
        page_white_c,
        page_white_h,
        page_white_paint,
        application_form_edit,
        deleteIco,
        sound,
        emoticon_waii,
        page_white_zip,
        page_white_picture,
        controller,
        add,
        eye,
        drive_edit,
        drive_rename,
        totalIcons
    } enumIco;


    //*********************************************************************************/
    //Cuidado con no sobrepasar el numero de elementos que define la constante MAXMENU
    //*********************************************************************************/
     typedef enum { MENUINICIAL,     //0
           PANTALLAPREGUNTA, //1
           MENUSELEMU, //2
           PANTALLASELROM, //3
           CARGAPANTALLAOPCIONRUTAS, //4
           PANTALLAOPCIONRUTAS, //5
           MENUSELECCIONAREMU, //6
           PANTALLAOPCIONES,
           PANTALLACOMPILAR,
           MENUSELEMURUTAS,
           CARGAPANTALLASELEMU,
           PANTALLASELEMU,
           PANTALLAMASOPCIONESRUTA,
           PANTALLAACTUALIZARROMS,
           MAXMENU
    } enumMenus;

class Constant{
    public:

        static string APPRUTA;

        Constant();
        ~Constant();

        template<class TIPO> static string TipoToStr(TIPO number){
           stringstream ss;//create a stringstream
           ss << number;//add number to the stream
           return ss.str();//return a string with the contents of the stream
        }

        template<class TIPO> static TIPO strToTipo(string str){
                TIPO i;
                stringstream s_str( str );
                s_str >> i;
                return i;
        }

        static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
            std::stringstream ss(s);
            std::string item;
            while(std::getline(ss, item, delim)) {
                elems.push_back(item);
            }
            return elems;
        }

        static string fecha(){
//           char* dS[7] = {"Domingo","Lunes","Martes","Miercoles",
//                          "Jueves","Viernes","Sabado"};
            string theFecha = "";
            time_t tSac = time(NULL);       // instante actual
            struct tm* tmP = localtime(&tSac);
            theFecha = TipoToStr(tmP->tm_mday) + "-" + TipoToStr(tmP->tm_mon+1) + "-" + TipoToStr(tmP->tm_year+1900);
            theFecha = theFecha + " " + TipoToStr(tmP->tm_hour) + ":" + TipoToStr(tmP->tm_min) + ":" + TipoToStr(tmP->tm_sec);



            //           cout << "Dia de semana: " << dS[tmP->tm_wday] << endl;
            //           cout << "Dia del año: " << tmP->tm_yday << endl;
            //           cout << "Horario de verano: " << (tmP->tm_isdst ? "Si" : "No") << endl;
            return theFecha;
        }


        static std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            return split(s, delim, elems);
        }

        static int contiene (const char *s1, const char *s2)
        {
            int len_s1=0, len_s2=0; //Tamanyos de los strings
            int j=0;  //para recorrer s2
            int i=0;  //para recorrer s1

            if (s1 != NULL && s2 != NULL)
            {
                len_s1 = strlen(s1);
                len_s2 = strlen(s2);

                for (i = 0; i < len_s1; i++)
                {
                    if (s1[i] == s2[j])
                    {
                        j++;
                    }
                    else if (j != len_s2)
                    {
                        j = 0;
                        if (s1[i] == s2[j])
                        {
                            j++;
                        }
                    }
                }
            }

            if (j == len_s2 && j != 0)
            {
                return (i-j);
            }
            else
            {
                return -1;
            }
        }

        static int contiene (const char *s1, const char s2)
        {
            int len_s1=0; //Tamanyos de los strings
            int i=0;  //para recorrer s1

            if (s1 != NULL)
            {
                len_s1 = strlen(s1);

                while (s1[i] != s2 && i<len_s1)
                {

                    i++;
                }
            }

            if (i != len_s1)
            {
                return (i);
            }
            else
            {
                return -1;
            }
        }

        /**
        *
        */
        static void stringTrim(char *dato)
        {
            int tam = strlen(dato);
            //int i = tam - 1;
            int i = tam;
            bool salir = false;

            int j=0;

            while (!salir && tam > 1 && j < tam)
            {
                if (dato[0] == ' ')
                {
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
                while (i > 0 && dato[i] == ' ' )
                {
                    i--;
                    dato[i] = '\0';
                }
            }
        }

        /**
*
*/
    static int findIcon(const char *filename){

    char ext[5] = {' ',' ',' ',' ','\0'};
    int len = 0;

    if (filename != NULL){
        len = strlen(filename);
        if ( len > 4){
            ext[3] = filename[len-1];
            ext[2] = filename[len-2];
            ext[1] = filename[len-3];
            ext[0] = filename[len-4];
        }
    }

    if (contiene(ext, "txt") >= 0 || contiene(ext, "TXT") >= 0
         || contiene(ext, ".inf") >= 0 || contiene(ext, ".INF") >= 0){
        return page_white_text;
    } else if (contiene(ext, ".gpu") >= 0 || contiene(ext, ".GPU") >= 0
        || contiene(ext, ".gpe") >= 0 || contiene(ext, ".GPE") >= 0
        || contiene(ext, ".exe") >= 0 || contiene(ext, ".EXE") >= 0
        || contiene(ext, ".bat") >= 0 || contiene(ext, ".BAT") >= 0
        || contiene(ext, ".com") >= 0 || contiene(ext, ".COM") >= 0){
        return page_white_gear;
    } else if (contiene(ext, ".gz") >= 0 || contiene(ext, ".GZ") >= 0
        || contiene(ext, ".z") >= 0 || contiene(ext, ".Z") >= 0
        || contiene(ext, ".tar") >= 0 || contiene(ext, ".TAR") >= 0
        || contiene(ext, ".zip") >= 0 || contiene(ext, ".ZIP") >= 0
        || contiene(ext, ".rar") >= 0 || contiene(ext, ".RAR") >= 0){
	    return page_white_compressed;
	} else if (contiene(ext, ".bmp") >= 0 || contiene(ext, ".BMP") >= 0
        || contiene(ext, ".jpg") >= 0 || contiene(ext, ".JPG") >= 0
        || contiene(ext, ".jpeg") >= 0 || contiene(ext,".JPEG") >= 0
        || contiene(ext, ".png") >= 0 || contiene(ext, ".PNG") >= 0
        || contiene(ext, ".gif") >= 0 || contiene(ext, ".GIF") >= 0){
        return page_white_picture;
    } else if (contiene(ext, ".bin") >= 0 || contiene(ext, ".BIN") >= 0){
        return page_white_zip;
    } else {
        return page_white;
    }
}
};


    //static const int MAXMENU = 13;  //Maximo numero de menus disponibles
    static const int MAXOBJECTS = 10; //Numero maximo de objetos (inputs, checks, etc.) que se pueden mostrar en una pagina
    //*********************************************************************************/
    //Cuidado con no sobrepasar el numero de elementos que define la constante MAXMENU
    //*********************************************************************************/

    static const t_color cNegro = {0,0,0};
    static const t_color cBlanco = {0xFF,0xFF,0xFF};
    static const t_color cRojo  = {0xFF,0,0};
    static const t_color cVerde = {0,0xFF,0};
    static const t_color cAzul  = {0,0,0xFF};
    static const t_color cGris  = {0x80,0x80,0x80};
    static const t_color cGrisClaro  = {0xC6,0xC6,0xC6};
    static const t_color cNaranja  = {0xFA,0x70,0x00};


    static const t_color cBgScreen = {0xB4,0xB4,0xB4};
    static const t_color cBgImageBin = cNegro;
    static const t_color cBgMenu = cBlanco;
    static const t_color cBorde1Button = {0x40,0x40,0x40};
    static const t_color cBorde2Button = cGris;
    static const t_color cButtonContent = {0xD4,0xD0,0xC8};
    static const t_color cInputBorder = {0x78,0x78,0x78};
    static const t_color cInputContent = cBlanco;

    static const int MENUSPACE = 15; //Separacion de cada elemento del menu
    static const unsigned char FONTSIZE = 12;
    static const bool TRAZA_ENABLED = false; //
    static const bool TRAZA_CONSOLE = true;
    static const unsigned int INILIST = 30; //Numero de elementos que se crearan inicialmente para las listas
    static const char FONT_TYPE[] = {"arial.ttf"}; //Fuente de los textos
    static const char WINDOW_TITLE[] = {"LaunchIt"};
    static const char ARRAYLETRAS[] = {" abcdefghijklmnñopqrstuvwxyz0123456789-_.:,;ç[]*'¿?=)(/&%$·!|@#"};
    static const bool SHOWFPS = true;   //Muestra los frames actuales
    static const bool LIMITFPS = true; //Especifica si se realiza una limitacion maxima de frames
    static const unsigned long FRAMELIMIT = 30; //Limite de frames
    static const unsigned long FRAMEPERIOD = 1000; //Periodo en el que se comprobara la velocidad de frames. en ms
    static const unsigned long TIMETOLIMITFRAME = FRAMEPERIOD/FRAMELIMIT; //Tiempo que deberia utilizar cada frame
    static const int COLORMENUSEL = 0xFF9F9F; //El color tiene que ser clarito
    static const unsigned int MAXQUERY = 10; //Maxima longitud de palabra de un input
    static const unsigned int MAXQUERYINT = 10; //Maxima longitud de palabra de un input de tipo int
    static const unsigned long DBLCLICKSPEED = 300; //tiempo en ms para poder hacer un doble click
    static const double ZOOMINCREMENT = 0.4; //Incremento de zoom que se aplicara a la imagen
    static const int MOVEINCREMENT = 15; //Pixels que se desplazara la imagen con una pulsacion
    static const bool VIEWALPHA = false;
    static const int CHECKW = 11; //Ancho de la cajita de seleccion de un input check
    static const int CHECKH = 11; //Alto de la cajita de seleccion de un input check
    static const int BUTTONW = 100;
    static const int BUTTONH = 25;
    static const int MINIBUTTONH = 25;
    static const int MINIBUTTONW = 25;
    static const int BUTTONBORDER = 1; //Borde del boton
    static const int ICOWIDTH = 16;
    //Variables para el formulario de querys
    static const int SPACIOLETRA = 14; //Separacion de cada letra del contenido del input
    static const int INPUTW=200;
    static const int INPUTH=25; //Ancho y alto del input
    static const int INPUTBORDER = 1; //Borde del input
    static const int TITLEBORDER = 10; //Separacion del borde respecto de la izquierda de la pantalla
    static const int TITLESPACE = 10; //Separacion del titulo respecto del borde
    static const int TITLEFONTSIZE = 20; //Tamanyo del titulo
    static const int INPUTCONTENT = 4; //Separacion en y del contenido del input respecto al input
    static const int INPUTDELIMITER = 5; //Separacion en y de los delimitadores del contenido del input respecto al contenido del input
    static const int INPUTDELIMITERSELECTOR = 24; //Separacion en y del selector (el triangulo) del contenido del input respecto al contenido del input
    static const int SELECTORBASE = 10; //Tamanyo de la base del triangulo del selector del input
    static const int SELECTORHEIGHT = 10; //Tamanyo de la altura del triangulo del selector del input
    //Variables para el formulario de query
    static const int MENUCATEGWIDTH = 100;


    //Dependientes del sistema
    #ifdef WIN
        static const int WINDOW_WIDTH = 320;
        static const int WINDOW_HEIGHT = 240;
        static const int SCREEN_BITS = 16;
        static const char FILE_SEPARATOR = 0x5C; //Separador de directorios para win32
        static const char FILE_PARENT[] = {"\\.."}; //Separador de directorios para win32
        static const char RUTA_TRAZA[] = {"C:\\traza2.txt"}; //Directorio de las trazas
        //static const char RUTA_TRAZA[] = {"C:\\traza.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_HIJO[] = {"C:\\trazaHIJO.txt"}; //Directorio de las trazas
        static const char VBCRLF[] = {"\n"}; //Retorno de carro para las trazas o ficheros en windows \n
        static const int CURSORVISIBLE = 1;
        #ifdef _RELEASE_
            static const int TRAZA_LEVEL = FATAL;
        #else
            static const int TRAZA_LEVEL = DEBUG;
        #endif
        static const int THUMBW = 0;    //Si THUMBW != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBX
        static const int THUMBH = 0;    //Si THUMBH != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBY
        static const int MAXTHUMBX = 7; //Si THUMBW == 0 ponemos por defecto este numero de imagenes en una fila
        static const int MAXTHUMBY = 5; //Si THUMBH == 0 ponemos por defecto este numero de imagenes en una colummna
        static const int SEPTHUMB = 2;  //Separacion del cuadro de seleccion de cada thumb
        #define JOY_BUTTON_UP              (0)
        #define JOY_BUTTON_UPLEFT          (1)
        #define JOY_BUTTON_LEFT            (2)
        #define JOY_BUTTON_DOWNLEFT        (3)
        #define JOY_BUTTON_DOWN            (4)
        #define JOY_BUTTON_DOWNRIGHT       (5)
        #define JOY_BUTTON_RIGHT           (6)
        #define JOY_BUTTON_UPRIGHT         (7)
        #define JOY_BUTTON_START           (8)
        #define JOY_BUTTON_SELECT          (9)
        #define JOY_BUTTON_R               (10)
        #define JOY_BUTTON_L               (11)
        #define JOY_BUTTON_A               (12)
        #define JOY_BUTTON_B               (13)
        #define JOY_BUTTON_Y               (14)
        #define JOY_BUTTON_X               (15)
        #define JOY_BUTTON_VOLUP           (16)
        #define JOY_BUTTON_VOLDOWN         (17)
        #define JOY_BUTTON_CLICK           (18)
    #elif UNIX
        static const int WINDOW_WIDTH = 320;
        static const int WINDOW_HEIGHT = 240;
        static const int SCREEN_BITS = 16;
        static const char FILE_SEPARATOR = '/'; //Separador de directorios para unix
        static const char FILE_PARENT[] = {"/.."}; //Separador de directorios para unix
        static const char RUTA_TRAZA[] = {"/home/pi/traza.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_HIJO[] = {"/home/pi/trazaHijo.txt"}; //Directorio de las trazas
        static const char VBCRLF[] = {"\r\n"}; //Retorno de carro para las trazas o ficheros en UNIX \r\n
        static const int CURSORVISIBLE = 0;
        static const int TRAZA_LEVEL = DEBUG;
        static const int THUMBW = 0;    //Si THUMBW != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBX
        static const int THUMBH = 0;    //Si THUMBH != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBY
        static const int MAXTHUMBX = 3; //Si THUMBW == 0 ponemos por defecto este numero de imagenes en una fila
        static const int MAXTHUMBY = 3; //Si THUMBH == 0 ponemos por defecto este numero de imagenes en una colummna
        static const int SEPTHUMB = 2;  //Separacion del cuadro de seleccion de cada thumb
        #define JOY_BUTTON_UP              (0)
        #define JOY_BUTTON_UPLEFT          (1)
        #define JOY_BUTTON_LEFT            (2)
        #define JOY_BUTTON_DOWNLEFT        (3)
        #define JOY_BUTTON_DOWN            (4)
        #define JOY_BUTTON_DOWNRIGHT       (5)
        #define JOY_BUTTON_RIGHT           (6)
        #define JOY_BUTTON_UPRIGHT         (7)
        #define JOY_BUTTON_START           (8)
        #define JOY_BUTTON_SELECT          (9)
        #define JOY_BUTTON_R               (10)
        #define JOY_BUTTON_L               (11)
        #define JOY_BUTTON_A               (12)
        #define JOY_BUTTON_B               (13)
        #define JOY_BUTTON_Y               (14)
        #define JOY_BUTTON_X               (15)
        #define JOY_BUTTON_VOLUP           (16)
        #define JOY_BUTTON_VOLDOWN         (17)
        #define JOY_BUTTON_CLICK           (18)

    #endif

#endif // CONSTANT_H_INCLUDED

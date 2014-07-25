#ifndef Fileattr_H
#define Fileattr_H


#include <sstream>
using namespace std;

    class FileProps{
        public:
            string filename;
            string filetype;
            string dir;
    };

    class FileLaunch{
        public:
             string rutaexe;
             string fileexe;
             string parmsexe;
             string rutaroms;
             string nombrerom;
             string titlerom;
             string nplayers;
             string categ;
             string id;
             bool isDescomprimir;
    };

#endif // Fileattr_H

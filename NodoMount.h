#ifndef NODOMOUNT_H
#define NODOMOUNT_H
#include <stddef.h>
#include <string>

class nodoMount{
public:
	std::string id ;
    std::string ruta;
    std::string nombreparticion ;
	nodoMount *siguienteMontado;
    nodoMount *anteriorMontado;
    nodoMount(){
        siguienteMontado = NULL;
        anteriorMontado = NULL;
        id = ' ';
        ruta = ' ';
        nombreparticion = ' ';
    }
private:
};

#endif
#ifndef NODOMOUNT_H
#define NODOMOUNT_H
#include <stddef.h>
#include <string>

class nodoMount{
public:
	std::string id;
    std::string ruta;
    std::string nombreparticion;
    std::string tipoparticion;
    int inicioparticion;
    int tamanioparticion;
    time_t horamontado;
	nodoMount *siguienteMontado;
    nodoMount *anteriorMontado;
    nodoMount(){
        siguienteMontado = NULL;
        anteriorMontado = NULL;
        id = ' ';
        ruta = ' ';
        nombreparticion = ' ';
        tipoparticion = ' ';
        inicioparticion = 0;
        tamanioparticion = 0;
    }
private:
};

#endif
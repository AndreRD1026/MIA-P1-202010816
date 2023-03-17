#ifndef NODOLOGIN_H
#define NODOLOGIN_H
#include <stddef.h>
#include <string>

class nodoLogin{
public:
	std::string usuario;
    std::string iddisco;
    nodoLogin(){
        usuario = ' ';
        iddisco = ' ';
    }
private:
};

#endif
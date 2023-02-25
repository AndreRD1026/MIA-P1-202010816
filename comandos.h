#ifndef COMANDOS_H
#define COMANDOS_H

#include <fstream>
#include <cstdio>
#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <string.h>

using namespace std;

typedef struct{
    string Comando = " ";
    string Size = " ";
    string Path = " ";
    string Fit = " ";
    string Unit = " ";
    string Tamano = " ";
    string Dimensional = " ";
    string Id = " ";
    string Nombre = " ";
    string Telefono = " ";
    string Direccion = " ";
    string X = " ";
}Parametros;

typedef struct {

    char part_status[1];
    char part_type[1];
    char part_fit[1];
    int part_start;
    int part_s;
    char part_name[16];
} Partition;

typedef struct{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit[1];
    Partition mbr_partition_1;
    Partition mbr_partition_2;
    Partition mbr_partition_3;
    Partition mbr_partition_4;
} MBR;




typedef struct{
    int id;
    char nombre[15];
    int telefono;
    char direccion[15];
} Ejemplo;

class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void crearArchivo(string tam, string path, string ajuste, string dim);
        void escribir(string id, string nombre, string tel, string dir, string x);
        void vertodo();
        void verX(string x);
        void eliminarX(string x);
        void mostrar_struct(Ejemplo ejm);
};

#endif // COMANDO_H
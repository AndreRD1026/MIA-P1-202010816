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
    string Name = " ";
    string Type = " ";
    string Delete = " ";
    string Add = " ";
}Parametros;

typedef struct {

    char part_status; //Indica si esta activa o no
    char part_type; //Valores P o E
    char part_fit; // Indica el Ajuste [B]est, [F]irst o [W]orst
    int part_start; // Indica el byte donde inicia la particion
    int part_s; // Tamanio total de la particion
    char part_name[16]; // Nombre de la particion
} Partition;

typedef struct{
    int mbr_tamano; //Tamanio del disco
    time_t mbr_fecha_creacion; // Fecha y hora de creacion del disco
    int mbr_dsk_signature; // Numero random, identifica de forma unica a cada disco
    char dsk_fit; //Ajuste de la particion [B]est, [F]irt o [W]orst
    Partition mbr_partition_1; // Estructura con informacion de la particion 1
    Partition mbr_partition_2; // Estructura con informacion de la particion 2
    Partition mbr_partition_3; // Estructura con informacion de la particion 3
    Partition mbr_partition_4; // Estructura con informacion de la particion 4
} MBR;


typedef struct {
    char part_status; // Indica si esta activa o no
    char part_fit; // Indica el Ajuste [B]est, [F]irst o [W]orst
    int part_start; // Indica el byte donde inicia la particion
    int part_s; // Tamanio total de la particion
    int part_next; // Byte en el que está el próximo EBR. -1 si no hay siguiente
    char part_name[16]; // Nombre de la particion
} EBR;

class Comando{
    public:
        Parametros param;
        void identificacionCMD(Parametros p);
        void comando_mkdisk(string tam, string path, string ajuste, string dim);
        void comando_rmdisk(string path);
        void comando_fdisk_creando(string size, string path, string name, string unit, string type, string fit, string del, string add);
        void comando_fdisk_modificando(string path, string name, string unit, string del, string add);
        void escritura(MBR actualizado, string path);
        void eliminar(string name, string path);
        void agregar(string name, string path, string unit, string size);
        void modificaradd(MBR disco, string path);
};

#endif // COMANDO_H
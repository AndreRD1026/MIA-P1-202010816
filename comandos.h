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
#include "NodoMount.h"
#include "NodoLogin.h"

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
    string ID = " ";
    string Fs = " ";
    string User = " ";
    string Pass = " ";
    string Grp = " ";
    string Ruta = " ";
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


// Estructuras para EXT2 Y EXT3

typedef struct {
    int s_filesystem_type; // Guarda el número que identifica el sistema de archivos utilizado
    int s_inodes_count; // Guarda el número total de inodos
    int s_blocks_count; // Guarda el número total de bloques
    int s_free_blocks_count; // Contiene el número de bloques libres
    int s_free_inodes_count; // Contiene el número de inodos libres
    time_t s_mtime; // Última fecha en el que el sistema fue montado
    time_t s_umtime; // Última fecha en que el sistema fue desmontado
    int s_mnt_count; // Indica cuantas veces se ha montado el sistema
    int s_magic; // Valor que identifica al sistema de archivos, tendrá el valor 0xEF53
    int s_inode_s; // Tamaño del inodo
    int s_block_s; // Tamaño del bloque
    int s_firts_ino; // Primer inodo libre
    int s_first_blo; // Primer bloque libre
    int s_bm_inode_start; // Guardará el inicio del bitmap de inodos
    int s_bm_block_start; // Guardará el inicio del bitmap de bloques
    int s_inode_start; // Guardará el inicio de la tabla de inodos
    int s_block_start; // Guardará el inicio de la tabla de bloques
} SuperBloque;


typedef struct {
    char j_type;
    char j_name[16];
    char j_destination[16];
    char j_content;
    time_t j_time;
}Journaling;


typedef struct {
    int i_uid; // UID del usuario propietario del archivo o carpeta
    int I_gid; // GID del grupo al que pertenece el archivo o carpeta.
    int i_s; // Tamaño del archivo en bytes
    time_t i_atime; // Última fecha en que se leyó el inodo sin modificarlo
    time_t i_ctime; // Fecha en la que se creó el inodo
    time_t i_mtime; // Última fecha en la que se modifica el inodo
    int i_block[15]; // Array en los que los primeros 12 registros son bloques directos.
    char i_type; // ndica si es archivo o carpeta. 1 = Archivo y 0 = Carpeta
    int i_perm; // Guardará los permisos del archivo o carpeta.
} Inodos;


typedef struct {
    char b_name[12]; // Nombre de la carpeta o archivo
    int b_inodo; // Apuntador hacia un inodo asociado al archivo o carpeta
} Content;


typedef struct {
    Content b_content[4];
} BloqueCarpeta;


typedef struct {
    char b_content[64]; // Array con el contenido del archivo
}BloqueArchivos;


typedef struct {
    int b_pointers[16]; // Array con los apuntadores a bloques (de archivo o carpeta)
}BloqueApuntadores;


typedef struct {
    char status;
}BitMapInodo;

typedef struct {
    char status;
}BitMapBloque;

//Aqui van todas las acciones de los comandos
class Comando{
    public:
        Parametros param;
        nodoMount *primeroMount;
        nodoMount *ultimoMount;
        nodoLogin *loginregistrado;
        Comando(){
            primeroMount == NULL;
            ultimoMount == NULL;
            loginregistrado == NULL;
        }

        void identificacionCMD(Parametros p);
        void comando_mkdisk(string tam, string path, string ajuste, string dim);
        void comando_rmdisk(string path);
        void comando_fdisk_creando(string size, string path, string name, string unit, string type, string fit, string del, string add);
        void comando_fdisk_modificando(string path, string name, string unit, string del, string add);
        void escritura(MBR actualizado, string path);
        void escrituraLogica(EBR particion, string path, int start);
        void eliminar(string name, string path);
        void agregar(string name, string path, string unit, string size);
        void modificaradd(MBR disco, string path);
        void actualizardisco(MBR disco, string path);
        void comando_mount(string path, string name);
        void montado(string id, string path, string nombreparticion,string tipoparticion,int inicioparticion, int tamaniopart);
        void comando_unmount(string id);
        void desmontado(string id);
        void verlista();
        void comando_mkfs(string id, string type, string fs);
        void crear_ext2(nodoMount *particion ,int n, int tipop);
        void crear_ext3(nodoMount *particion, int n, int tipop);
        void Escribir_SuperBloque(string path, SuperBloque SP, int inicio);
        void Escribir_BitMapInodos(string path, BitMapInodo bmI[], int inicio, int n);
        void Escribir_BitMapBloques(string path, BitMapBloque bmB[], int inicio, int n);
        void Escribir_Inodos(string path, Inodos inodo[] ,int inicio, int n);
        void Escribir_BloqueCarpeta(string path, BloqueCarpeta Carpeta, int inicio);
        void Escribir_BloqueArchivo(string path, BloqueArchivos Archivo, int inicio);
        void Escribir_BloqueApuntadores(string path, BloqueApuntadores Apuntador, int inicio);
        void Escribir_Journaling(string path, Journaling jour[], int inicio, int n);
        void Actualizar_SuperBloque(string path, SuperBloque SP, int inicio);
        void comando_login(string user, string pass, string id);
        void comando_logout();
        void comando_mkgrp(string name);
        void comando_rmgrp(string name);
        void crear_grupo(string name);
        void eliminar_grupo(string name);
        void comando_mkusr(string user, string pass, string grp);
        void crear_usuario(string user, string pass, string grp);
        void comando_rep(string name, string path, string id, string ruta);
        void reporte_mbr(string nombresalida, string path, string id);
        void reporte_disk(string nombresalida, string path, string id);

        void reporte_journaling(string nombresalida, string path, string id);

        void reporte_bm_inode(string nombresalida, string path, string id);
        void reporte_bm_bloc(string nombresalida, string path, string id);
        void reporte_Sb(string nombresalida, string path, string id);
        
};

#endif // COMANDO_H


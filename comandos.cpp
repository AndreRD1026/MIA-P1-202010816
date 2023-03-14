#include "comandos.h"
#include <cmath>
#include <vector>


void Comando::identificacionCMD(Parametros p){
    if(p.Comando=="mkdisk"){ // Se identifica el tipo de comando
        if(p.Size != " " && p.Path != " "){ // Se validan los parametros para el comando
            comando_mkdisk(p.Size,p.Path,p.Fit,p.Unit);
        }else{
            cout << "Error creando Disco: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="rmdisk"){ // Se identifica el tipo de comando
        if(p.Path != " "){ // Se validan los parametros para el comando
            comando_rmdisk(p.Path);
        }else{
            cout << "Error eliminando el Disco: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="fdisk"){ // Se identifica el tipo de comando
        if(p.Size != " " && p.Path != " " && p.Name != " "){ // Se validan los parametros para el comando
            comando_fdisk_creando(p.Size,p.Path,p.Name,p.Unit,p.Type,p.Fit,p.Delete,p.Add);
        } else if (p.Path != " " && p.Name != " "){
            comando_fdisk_modificando(p.Path,p.Name,p.Unit,p.Delete,p.Add);
        } else {
            cout << "Error al crear una particion en el Disco "<<endl;
        }
    }else if(p.Comando=="mount"){ // Se identifica el tipo de comando
        if(p.Path != " " && p.Name != " "){ // Se validan los parametros para el comando
            comando_mount(p.Path, p.Name);
        }else{
            cout << "Error para montar el Disco: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="unmount"){ // Se identifica el tipo de comando
        if(p.ID != " "){ // Se validan los parametros para el comando
            comando_unmount(p.ID);
        }else{
            cout << "Error para desmontar el Disco: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="mkfs"){ // Se identifica el tipo de comando
        if(p.ID != " "){ // Se validan los parametros para el comando
            comando_mkfs(p.ID, p.Type, p.Fs);
        }else{
            cout << "Error para formatear la particion: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="login"){ // Se identifica el tipo de comando
        if(p.User != " " && p.Pass != " " && p.ID != " "){ // Se validan los parametros para el comando
            comando_login(p.User,p.Pass,p.ID);
        }else{
            cout << "Error para Iniciar sesion : Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="logout"){ // Se identifica el tipo de comando
            comando_logout();
    }else if(p.Comando=="mkgrp"){ // Se identifica el tipo de comando
        if(p.Name != " "){ // Se validan los parametros para el comando
            comando_mkgrp(p.Name);
        }else{
            cout << "Error para crear un grupo:  Parametros obligatorios no definidos " << endl;
        }
    }
    
    else if(p.Comando=="rep"){ // Se identifica el tipo de comando
        if(p.Name != " " && p.Path != " " && p.ID != " "){ // Se validan los parametros para el comando
            comando_rep(p.Name,p.Path,p.ID,p.Ruta);
        }else{
            cout << "Error para crear reporte: Parametros obligatorios no definidos " << endl;
        }
    }
}

/* 

* ? Falta leer ruta con las comillas

*/

void Comando::comando_mkdisk(string tam, string path, string ajuste, string dim)
{
    // Calculo Tamaño del Archivo
    int size_file = 0, tamano = atoi(tam.c_str());
    char dimen = dim.at(0);

    // Creando dsk unico
    int dsk_s = 1 + rand() % 1000;

    int signature = dsk_s;

    if (dimen == 'k' || dimen == 'K')
    {
        size_file = tamano;
    }
    else if (dimen == 'm' || dimen == 'M' || dimen == ' ')
    {
        size_file = tamano * 1024;
    }
    else
    {
        cout << " ¡¡ ERROR !! La Unidad no pertenece al sistema\n" << endl;
        return;
    }

    if (tamano > 0)
    {

        string ruta = "";
        string rutacomillas = "";

        // if(path.)

        int inicio = 0;
        int fin = path.find("/");
        string delimitador = "/";

        while (fin != -1)
        {
            ruta += path.substr(inicio, fin - inicio);
            ruta += "/";
            inicio = fin + delimitador.size();
            fin = path.find("/", inicio);
        }

        string name = path.substr(path.find_last_of("/") + 1);

        string extension = name.substr(0, name.find("."));

        // Escritura de Bloque en Archivo
        int pos = name.find(".");
        name.erase(0, 1 + pos);

        if (name != "dsk")
        {
            cout << "La extension del archivo no corresponde a .dsk" << endl;
            return;
        }

        // Preparacion Bloque
        char bloque[1024];
        for (int i = 0; i < 1024; i++)
        {
            bloque[i] = '\0';
        }

        int limite = 0;
        FILE *archivo_binario;

        // Creacion del path
        string crear = "mkdir -p " + ruta;
        system(crear.c_str());
        if ((archivo_binario = fopen(path.c_str(), "w+b")) == NULL)
        {
            cout << "Error" << endl;
        }
        else
        {

            MBR disco;

            disco.mbr_tamano = size_file;
            disco.mbr_dsk_signature = signature;
            disco.mbr_fecha_creacion = std::time(0);

            if (ajuste == " ")
            {
                ajuste = "w";
            }
            else if (ajuste == "b")
            {
                ajuste = "b";
            }
            else if (ajuste == "f")
            {
                ajuste = "f";
            }
            else
            {
                cout << "¡¡ Error !! El ajuste de particiones solo puede ser: B, F, W " << endl;
                return;
            }

            disco.dsk_fit = ajuste.at(0);

            // CONFIGURANDO PARTICIONES

            disco.mbr_partition_1.part_status = '0';
            disco.mbr_partition_2.part_status = '0';
            disco.mbr_partition_3.part_status = '0';
            disco.mbr_partition_4.part_status = '0';

            disco.mbr_partition_1.part_type = '\0';
            disco.mbr_partition_2.part_type = '\0';
            disco.mbr_partition_3.part_type = '\0';
            disco.mbr_partition_4.part_type = '\0';

            disco.mbr_partition_1.part_fit = '\0';
            disco.mbr_partition_2.part_fit = '\0';
            disco.mbr_partition_3.part_fit = '\0';
            disco.mbr_partition_4.part_fit = '\0';

            disco.mbr_partition_1.part_start = 0;
            disco.mbr_partition_2.part_start = 0;
            disco.mbr_partition_3.part_start = 0;
            disco.mbr_partition_4.part_start = 0;

            disco.mbr_partition_1.part_s = 0;
            disco.mbr_partition_2.part_s = 0;
            disco.mbr_partition_3.part_s = 0;
            disco.mbr_partition_4.part_s = 0;

            strcpy(disco.mbr_partition_1.part_name, "");
            strcpy(disco.mbr_partition_2.part_name, "");
            strcpy(disco.mbr_partition_3.part_name, "");
            strcpy(disco.mbr_partition_4.part_name, "");

            fseek(archivo_binario, 0, SEEK_SET);
            fwrite(&disco, sizeof(MBR), 1, archivo_binario);

            while (limite != size_file)
            {
                fwrite(&bloque, 1024, 1, archivo_binario);
                limite++;
            }
            fclose(archivo_binario);

            cout << "" << endl;
            cout << "*                 Disco creado con exito                   *" << endl;
            cout << "" << endl;
        }
    }
    else
    {
        cout << "" << endl;
        cout << " ¡¡ ERROR !! El tamano del disco debe ser un numero positivo y mayor a 0\n" << endl;
    }

    dsk_s = 0;
}

/* 

* ? Falta leer ruta con las comillas

*/

void Comando::comando_rmdisk(string path)
{
    string ruta = "";

    int inicio = 0;
    int fin = path.find("/");
    string delimitador = "/";

    while (fin != -1)
    {
        ruta += path.substr(inicio, fin - inicio);
        ruta += "/";
        inicio = fin + delimitador.size();
        fin = path.find("/", inicio);
    }

    string name = path.substr(path.find_last_of("/") + 1);
    string extension = name.substr(0, name.find("."));
    int pos = name.find(".");
    name.erase(0, 1 + pos);

    if (name != "dsk")
    {
        cout << "La extension del archivo no corresponde a .dsk" << endl;
        return;
    }
    else
    {
        remove(path.c_str());

        cout << "" << endl;
        cout << "*                 Disco eliminado con exito                   *" << endl;
        cout << "" << endl;
    }
}

/* 

* ? Falta valida logica repetida

*/


void Comando::comando_fdisk_creando(string size, string path, string name, string unit, string type, string fit, string del, string add){
    
    int size_file = 0, tamano = atoi(size.c_str());

    string ajuste,tipo, nombre, unidad, tipoo, ajust;


    MBR lectura;
    FILE *discolectura;

    if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
    } else {
        fseek(discolectura, 0, SEEK_SET);
        fread(&lectura, sizeof (MBR), 1, discolectura);
        fclose(discolectura);
    }

        if(unit != " "){
            if(unit == "k" || unit == "K"){
                size_file = tamano;
            } else if(unit == "m" || unit == "M"){
                size_file = tamano * 1024;
            } else if (unit == "b" || unit == "B"){
                size_file = (tamano/1000);
            } else{
                cout<<"¡¡ Error !! La unidad solo puede ser: B , K, M "<<endl;
                return;
            }
        } else {
            size_file = tamano;
        }

        if(fit != " "){
            if(fit == "B" || fit == "b"){
                ajuste = "B";
            } else if(fit == "F" || fit == "f"){
                ajuste = "F";
            } else if(fit == "W" || fit == "w"){
                ajuste = "W";
            } else {
                cout<<"¡¡ Error !! El ajuste solo puede ser: B , F, W "<<endl;
                return;
            }
        } else {
            ajuste = "W";
        }


        if(type != " "){
            if(type == "P" || type == "p"){
                tipo = "P";
            } else if(type == "E" || type == "e"){
                tipo = "E";
            }else if(type == "L" || type == "l"){
                tipo = "L";
            } else {
                cout<<"¡¡ Error !! El tipo de particion solo puede ser: P , E, L "<<endl;
                return;
            }
        } else {
            tipo = "P";
        }

        MBR disco = lectura;
        Partition particion[4];
        particion[0] = disco.mbr_partition_1;
        particion[1] = disco.mbr_partition_2;
        particion[2] = disco.mbr_partition_3;
        particion[3] = disco.mbr_partition_4;

        if(disco.mbr_tamano > size_file){

        if(tipo!="L"){
        int tamanoDisponibleAntes = 0;
        int tamanoDisponible = 0;
        //Verificando estados 

        string pruebaa, otroo, sisss;
        for (int i = 0; i < 4; i++) {
            pruebaa=particion[i].part_name;
            otroo = particion[i].part_status;
            cout<<"Nombre? "<<pruebaa<<endl;
            cout<<"Estatus "<<otroo<<endl;
            cout<<"Tipo "<<particion[i].part_type<<endl;
            cout<<"Tamanio "<<particion[i].part_s<<endl;
            }

        string nombreparticion = "";
            for (int i = 0; i < 4; i++) {
                nombreparticion=particion[i].part_name;
                if(nombreparticion==name){
                    cout<<"¡¡ Error !! El nombre "<<name<<" ya está ocupado. "<<endl;
                    return;
                }
            }


        if(tipo == "E" || tipo == "e"){
            for (int i = 0; i < 4; i++) {
                string obtenertipo;
                obtenertipo = particion[i].part_type;
                if (obtenertipo == "E" || obtenertipo == "e"){
                    cout<<"¡¡ Error !! Ya existe una particion extendida"<<endl;
                    return;
                }
            }
        }

        string existetipo = "";
        for (int i=0; i<4; i++){
            existetipo = particion[i].part_type;
            if(existetipo != "P" || existetipo != "E"){
                if(particion[0].part_type == 'P' || particion[0].part_type == 'E'){
                    cout<<"Entra 1 "<<endl;
                    disco.mbr_partition_1.part_status = '1';
                }if(particion[1].part_type == 'P' || particion[1].part_type == 'E'){
                    cout<<"Entra 2 "<<endl;
                    disco.mbr_partition_2.part_status = '1';
                }if(particion[2].part_type == 'P' || particion[2].part_type == 'E'){
                    cout<<"Entra 3 "<<endl;
                    disco.mbr_partition_3.part_status = '1';
                }if(particion[3].part_type == 'P' || particion[3].part_type == 'E'){
                    cout<<"Entra 4 "<<endl;
                    disco.mbr_partition_4.part_status = '1';
                }
            }
        }

        if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
            tamanoDisponibleAntes = disco.mbr_tamano;
            if(tamanoDisponibleAntes > size_file){
                strcpy(disco.mbr_partition_1.part_name, name.c_str());
                disco.mbr_partition_1.part_status = '0';
                disco.mbr_partition_1.part_type = tipo.at(0);
                disco.mbr_partition_1.part_fit = ajuste.at(0);
                disco.mbr_partition_1.part_s = size_file;
                disco.mbr_partition_1.part_start = (sizeof(disco)+1);
                tamanoDisponible = disco.mbr_tamano - disco.mbr_partition_1.part_s;
                cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                escritura(disco,path);
                cout<<"Particion 1 asginada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
            tamanoDisponibleAntes = disco.mbr_tamano - disco.mbr_partition_1.part_s;
            if(tamanoDisponibleAntes > size_file){
                strcpy(disco.mbr_partition_2.part_name, name.c_str());
                disco.mbr_partition_1.part_status = '0';
                disco.mbr_partition_2.part_status = '0';
                disco.mbr_partition_2.part_type = tipo.at(0);
                disco.mbr_partition_2.part_fit = ajuste.at(0);
                disco.mbr_partition_2.part_s = size_file;
                disco.mbr_partition_2.part_start = (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s);
                cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                escritura(disco,path);
                cout<<"Particion 2 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
            tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s);
            if(tamanoDisponibleAntes > size_file){
                strcpy(disco.mbr_partition_3.part_name, name.c_str());
                disco.mbr_partition_1.part_status = '0';
                disco.mbr_partition_2.part_status = '0';
                disco.mbr_partition_3.part_status = '0';
                disco.mbr_partition_3.part_type = tipo.at(0);
                disco.mbr_partition_3.part_fit = ajuste.at(0);
                disco.mbr_partition_3.part_s = size_file;
                disco.mbr_partition_3.part_start = (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s);
                cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                escritura(disco,path);
                cout<<"Particion 3 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '0'){
            tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s);
            if(tamanoDisponibleAntes > size_file){
                strcpy(disco.mbr_partition_4.part_name, name.c_str());
                disco.mbr_partition_1.part_status = '0';
                disco.mbr_partition_2.part_status = '0';
                disco.mbr_partition_3.part_status = '0';
                disco.mbr_partition_4.part_status = '0';
                disco.mbr_partition_4.part_type = tipo.at(0);
                disco.mbr_partition_4.part_fit = ajuste.at(0);
                disco.mbr_partition_4.part_s = size_file;
                disco.mbr_partition_4.part_start = (disco.mbr_partition_3.part_start + disco.mbr_partition_3.part_s + 1);
                tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s+ disco.mbr_partition_4.part_s);
                cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                escritura(disco,path);
                cout<<"Particion 4 asignada"<<endl;
            }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '1'){
                
                int tamanioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start) - 1;
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                if(tamanioParticion1 >= size_file){
                    strcpy(disco.mbr_partition_1.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_1.part_type = tipo.at(0);
                    disco.mbr_partition_1.part_fit = ajuste.at(0);
                    disco.mbr_partition_1.part_s = size_file;
                    disco.mbr_partition_1.part_start =  (sizeof(disco)+1) ;
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                    escritura(disco,path);
                    cout<<"Particion 1 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }            
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '1'){
                
                int tamanioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int espacioRestante2 = tamanioParticion2 - disco.mbr_partition_2.part_s;
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                if(tamanioParticion2 >= size_file){
                    strcpy(disco.mbr_partition_2.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_2.part_type = tipo.at(0);
                    disco.mbr_partition_2.part_fit = ajuste.at(0);
                    disco.mbr_partition_2.part_s = size_file;
                    disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                    
                    escritura(disco,path);
                    cout<<"Particion 2 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '1'){
                
                int tamanioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                if(tamanioParticion3 >= size_file){
                    strcpy(disco.mbr_partition_3.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_3.part_type = tipo.at(0);
                    disco.mbr_partition_3.part_fit = ajuste.at(0);
                    disco.mbr_partition_3.part_s = size_file;
                    disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                    
                    escritura(disco,path);
                    cout<<"Particion 3 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        }else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '1'){

                int espacioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int espacioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int espaciototalparticion = espacioParticion1 + espacioParticion2;
                cout<<"Tamanio antes de la particion: "<<espaciototalparticion<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_1.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_1.part_type = tipo.at(0);
                    disco.mbr_partition_1.part_fit = ajuste.at(0);
                    disco.mbr_partition_1.part_s = size_file;
                    disco.mbr_partition_1.part_start =  (sizeof(disco)+1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                    escritura(disco,path);
                    cout<<"Particion 1 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '1'){
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion= disco.mbr_tamano - (disco.mbr_partition_4.part_s + espacioLibre);
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_1.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_1.part_type = tipo.at(0);
                    disco.mbr_partition_1.part_fit = ajuste.at(0);
                    disco.mbr_partition_1.part_s = size_file;
                    disco.mbr_partition_1.part_start =  (sizeof(disco)+1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                    escritura(disco,path);
                    cout<<"Particion 1 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '1'){
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion= disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_4.part_s + espacioLibre);
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_2.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_2.part_type = tipo.at(0);
                    disco.mbr_partition_2.part_fit = ajuste.at(0);
                    disco.mbr_partition_2.part_s = size_file;
                    disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                    escritura(disco,path);
                    cout<<"Particion 2 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } 
        else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '1'){
                
                int espacioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int espacioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int espacioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                int espacioNoUsado = disco.mbr_tamano - (espacioParticion1 + espacioParticion2 + espacioParticion3);
                int espacioParticion4 = espacioNoUsado;
                
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion = espacioParticion2 + espacioParticion3;
                cout<<"Tamanio antes de la particion: "<<espaciototalparticion<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_2.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_2.part_type = tipo.at(0);
                    disco.mbr_partition_2.part_fit = ajuste.at(0);
                    disco.mbr_partition_2.part_s = size_file;
                    disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                    tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                    cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                    cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                    cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                    escritura(disco,path);
                    cout<<"Particion 2 asignada"<<endl;
            }else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        }
            else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '0'){
                
                int tamanioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int tamanioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int tamanioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                int espacioNoUsado = disco.mbr_tamano - (tamanioParticion1 + tamanioParticion2 + tamanioParticion3);
                int tamanioParticion4 = espacioNoUsado;

                cout<<"Tamanio particion 2 "<<tamanioParticion2 <<endl;
                cout<<"Espacio no usado " << espacioNoUsado<<endl;
                cout<<"Tamanio Particion 4"<<tamanioParticion4 <<endl;
                if(ajuste == "B" || ajuste == "b"){
                    if(tamanioParticion2 < tamanioParticion4){
                        if (tamanioParticion2 >= size_file){
                            strcpy(disco.mbr_partition_2.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_2.part_type = tipo.at(0);
                            disco.mbr_partition_2.part_fit = ajuste.at(0);
                            disco.mbr_partition_2.part_s = size_file;
                            disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 2 asignada"<<endl;
                        }
                    } else if (tamanioParticion4 < tamanioParticion2){
                        if (tamanioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_3.part_start + disco.mbr_partition_3.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                } else if(ajuste == "F" || ajuste == "f"){
                        if (tamanioParticion2 >= size_file){
                            strcpy(disco.mbr_partition_2.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_2.part_type = tipo.at(0);
                            disco.mbr_partition_2.part_fit = ajuste.at(0);
                            disco.mbr_partition_2.part_s = size_file;
                            disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 2 asignada"<<endl;
                        }else if (tamanioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_3.part_start + disco.mbr_partition_3.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(ajuste == "W" || ajuste == "w"){
                    if(tamanioParticion4 > tamanioParticion2){
                        if (tamanioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_3.part_start + disco.mbr_partition_3.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else if (tamanioParticion2 > tamanioParticion4){
                        if (tamanioParticion2 >= size_file){
                            strcpy(disco.mbr_partition_2.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_2.part_type = tipo.at(0);
                            disco.mbr_partition_2.part_fit = ajuste.at(0);
                            disco.mbr_partition_2.part_s = size_file;
                            disco.mbr_partition_2.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 2 asignada"<<endl;
                        } 
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '1'){
                
                int tamanioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int tamanioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                
                cout<<"Tamanio particion 1 "<<tamanioParticion1 <<endl;
                //cout<<"Espacio no usado " << espacioLibre<<endl;
                cout<<"Tamanio Particion 3"<<tamanioParticion3 <<endl;
                if(ajuste == "B" || ajuste == "b"){
                    if(tamanioParticion1 < tamanioParticion3){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }
                    } else if (tamanioParticion3 < tamanioParticion1){
                        if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                } else if(ajuste == "F" || ajuste == "f"){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) +1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }else if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(ajuste == "W" || ajuste == "w"){
                    if(tamanioParticion3 > tamanioParticion1){
                        if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }
                    } else if (tamanioParticion1 > tamanioParticion3){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1 );
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        } 
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '0'){
                
                int espacioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int espacioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int tamanioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                int espacioNoUsado = disco.mbr_tamano - (espacioParticion1 + espacioParticion2 + tamanioParticion3);
                int espacioParticion4 = espacioNoUsado;
                
                cout<<"Tamanio particion 1 "<<espacioParticion1 <<endl;
                cout<<"Tamanio Particion 4"<<espacioParticion4 <<endl;
                if(ajuste == "B" || ajuste == "b"){
                    if(espacioParticion1 < espacioParticion4){
                        if (espacioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }
                    } else if (espacioParticion4 < espacioParticion1){
                        if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                } else if(ajuste == "F" || ajuste == "f"){
                        if (espacioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) +1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }else if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(ajuste == "W" || ajuste == "w"){
                    if(espacioParticion4 > espacioParticion1){
                        if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else if (espacioParticion1 > espacioParticion4){
                        if (espacioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1 );
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        } 
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '0'){
                
                int espacioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int espacioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int espacioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
                int espacioNoUsado = disco.mbr_tamano - (espacioParticion1 + espacioParticion2 + espacioParticion3);
                int espacioParticion4 = espacioNoUsado;

                int espacioparticion12 = espacioParticion1 + espacioParticion2;
                
                cout<<"Tamanio particion 1 "<<espacioParticion1 <<endl;
                cout<<"Tamanio Particion 4"<<espacioParticion4 <<endl;
                if(ajuste == "B" || ajuste == "b"){
                    if(espacioparticion12 < espacioParticion4){
                        if (espacioparticion12 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }
                    } else if (espacioParticion4 < espacioparticion12){
                        if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                } else if(ajuste == "F" || ajuste == "f"){
                        if (espacioparticion12 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) +1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }else if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(ajuste == "W" || ajuste == "w"){
                    if(espacioParticion4 > espacioparticion12){
                        if (espacioParticion4 >= size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_4.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }
                    } else if (espacioparticion12 > espacioParticion4){
                        if (espacioparticion12 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1 );
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        } 
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                }
        } else if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
                
                int tamanioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
                int tamanioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
                int tamanioParticion3 = disco.mbr_tamano - (tamanioParticion1 + tamanioParticion2);
                
                cout<<"Tamanio particion 1 "<<tamanioParticion1 <<endl;
                //cout<<"Espacio no usado " << espacioLibre<<endl;
                cout<<"Tamanio Particion 3"<<tamanioParticion3 <<endl;
                if(ajuste == "B" || ajuste == "b"){
                    if(tamanioParticion1 < tamanioParticion3){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }
                    } else if (tamanioParticion3 < tamanioParticion1){
                        if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                } else if(ajuste == "F" || ajuste == "f"){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) +1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        }else if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(ajuste == "W" || ajuste == "w"){
                    if(tamanioParticion3 > tamanioParticion1){
                        if (tamanioParticion3 >= size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start =  (disco.mbr_partition_2.part_start + disco.mbr_partition_2.part_s + 1);
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_3.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }
                    } else if (tamanioParticion1 > tamanioParticion3){
                        if (tamanioParticion1 >= size_file){
                            strcpy(disco.mbr_partition_1.part_name, name.c_str());
                            disco.mbr_partition_1.part_status = '0';
                            disco.mbr_partition_2.part_status = '0';
                            disco.mbr_partition_3.part_status = '0';
                            disco.mbr_partition_4.part_status = '0';
                            disco.mbr_partition_1.part_type = tipo.at(0);
                            disco.mbr_partition_1.part_fit = ajuste.at(0);
                            disco.mbr_partition_1.part_s = size_file;
                            disco.mbr_partition_1.part_start =  (sizeof(disco) + 1 );
                            tamanoDisponible = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                            cout<<"Start: "<<disco.mbr_partition_1.part_start<<endl;
                            cout<<"Tamano disponible en bytes "<<(tamanoDisponible*1024)<<endl;
                            cout<<"Tamanio actual de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                            escritura(disco,path);
                            cout<<"Particion 1 asignada"<<endl;
                        } 
                    } else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '1'){
            cout<<"¡¡ Error !! Ya no hay particiones disponibles"<<endl;
            return;
        }
    }else{

        //Cuando la particion es tipo Logica
        int tamanioTotalParticion;
        int inicioParticion;
        if(tipo == "L" || tipo == "l"){

        int inicioLogica = particion[0].part_start;
        int inicioLogica2 = particion[1].part_start;
        int inicioLogica3 = particion[2].part_start;
        int inicioLogica4 = particion[3].part_start;

        for(int i=0; i < 4; i++){
                if(particion[0].part_type == 'E'){
                
                    EBR prueba;
                    FILE *discolecturaE;

                    if ((discolecturaE = fopen(path.c_str(), "r+b")) == NULL) {
                            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
                    } else {
                        fseek(discolecturaE, inicioLogica, SEEK_SET);
                        fread(&prueba, sizeof (EBR), 1, discolecturaE);
                        fclose(discolecturaE);
                    }

                    EBR ParticionLogica = prueba;

                    if(ParticionLogica.part_fit != 'B' && ParticionLogica.part_fit != 'F' && ParticionLogica.part_fit != 'W'){
                        tamanioTotalParticion = particion[0].part_s;
                        inicioParticion = particion[0].part_start;
                        if(tamanioTotalParticion >= size_file){
                            ParticionLogica.part_status = '0';
                            ParticionLogica.part_fit = ajuste.at(0);
                            ParticionLogica.part_start = inicioParticion;
                            ParticionLogica.part_s = size_file;
                            strcpy(ParticionLogica.part_name, name.c_str());
                            // Como es la primera no tiene siguiente
                            ParticionLogica.part_next = -1;
                            escrituraLogica(ParticionLogica, path, inicioParticion);
                            return;
                    }else{
                        cout<<"¡¡ Error !! El tamanio de la particion logica supera a la particion Extendida "<<endl;
                        return;
                    }
                    }else if(ParticionLogica.part_fit == 'B' || ParticionLogica.part_fit == 'F' || ParticionLogica.part_fit == 'W') {
                        EBR Primero;
                        int inicioLogica1 = particion[0].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica1, SEEK_SET);
                        fread(&Primero, sizeof(EBR), 1, discolecturaE);
                        int tamanioTotal = particion[0].part_start + particion[0].part_s;
                        EBR Anterior;
                        Anterior = Primero;
                        EBR Siguiente;
                        int possig = Primero.part_next;

                        while(true){
                            fseek(discolecturaE, possig, SEEK_SET);
                            fread(&Siguiente, sizeof(EBR), 1, discolecturaE);

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                if(strcmp(Siguiente.part_name, name.c_str()) == 0){
                                cout<<"¡¡ Error !! Ya existe una particion con ese nombre "<<endl;
                                return;
                                
                            }
                                possig = Siguiente.part_next;
                                Anterior = Siguiente;
                            }
                            else {
                                Siguiente.part_start = Anterior.part_start + sizeof(EBR) + Anterior.part_s + 1;
                                cout<<"La siguiente seria "<<Siguiente.part_start<<endl;
                                fseek(discolecturaE, Siguiente.part_start, SEEK_SET);
                                Siguiente.part_status = '0';
                                Siguiente.part_fit = ajuste.at(0);
                                Siguiente.part_s = size_file;
                                Siguiente.part_next = -1;
                                strcpy(Siguiente.part_name, name.c_str());
                                Anterior.part_next = Siguiente.part_start;

                                if(Siguiente.part_start + Siguiente.part_s > tamanioTotal){
                                    cout<<"¡¡ Error !! El tamanio de la particion logica supera al de la extendida "<<endl;
                                    fclose(discolecturaE);
                                    return;
                                }else{
                                    fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                    fseek(discolecturaE, Anterior.part_start, SEEK_SET);
                                    fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);
                                    cout << "" << endl;
                                    cout << "*                 Particion logica creada con exito            *" << endl;
                                    cout << "" << endl;
                                }
                            }
                            break;
                        }
                        fclose(discolecturaE);
                        return;
                    }
                } else if(particion[1].part_type == 'E'){

                    EBR prueba;
                    FILE *discolecturaE;

                    if ((discolecturaE = fopen(path.c_str(), "r+b")) == NULL) {
                            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
                    } else {
                        fseek(discolecturaE, inicioLogica2, SEEK_SET);
                        fread(&prueba, sizeof (EBR), 1, discolecturaE);
                        fclose(discolecturaE);
                    }

                    EBR ParticionLogica = prueba;

                    if(ParticionLogica.part_fit != 'B' && ParticionLogica.part_fit != 'F' && ParticionLogica.part_fit != 'W'){
                        tamanioTotalParticion = particion[1].part_s;
                        inicioParticion = particion[1].part_start;
                        if(tamanioTotalParticion >= size_file){
                            ParticionLogica.part_status = '0';
                            ParticionLogica.part_fit = ajuste.at(0);
                            ParticionLogica.part_start = inicioParticion;
                            ParticionLogica.part_s = size_file;
                            strcpy(ParticionLogica.part_name, name.c_str());
                            // Como es la primera no tiene siguiente
                            ParticionLogica.part_next = -1;
                            escrituraLogica(ParticionLogica, path, inicioParticion);
                            return;
                    }else{
                        cout<<"¡¡ Error !! El tamanio de la particion logica supera a la particion Extendida "<<endl;
                        return;
                    }
                    }else if(ParticionLogica.part_fit == 'B' || ParticionLogica.part_fit == 'F' || ParticionLogica.part_fit == 'W') {
                        EBR Primero;
                        int inicioLogica2 = particion[1].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica2, SEEK_SET);
                        fread(&Primero, sizeof(EBR), 1, discolecturaE);
                        int tamanioTotal = particion[1].part_start + particion[1].part_s;
                        EBR Anterior;
                        Anterior = Primero;
                        EBR Siguiente;
                        int possig = Primero.part_next;

                        while(true){
                            fseek(discolecturaE, possig, SEEK_SET);
                            fread(&Siguiente, sizeof(EBR), 1, discolecturaE);

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                if(strcmp(Siguiente.part_name, name.c_str()) == 0){
                                cout<<"¡¡ Error !! Ya existe una particion con ese nombre "<<endl;
                                return;
                            }
                                possig = Siguiente.part_next;
                                Anterior = Siguiente;
                            }
                            else {
                                Siguiente.part_start = Anterior.part_start + sizeof(EBR) + Anterior.part_s + 1;
                                cout<<"La siguiente seria "<<Siguiente.part_start<<endl;
                                fseek(discolecturaE, Siguiente.part_start, SEEK_SET);
                                Siguiente.part_status = '0';
                                Siguiente.part_fit = ajuste.at(0);
                                Siguiente.part_s = size_file;
                                Siguiente.part_next = -1;
                                strcpy(Siguiente.part_name, name.c_str());
                                Anterior.part_next = Siguiente.part_start;

                                if(Siguiente.part_start + Siguiente.part_s > tamanioTotal){
                                    cout<<"¡¡ Error !! El tamanio de la particion logica supera al de la extendida "<<endl;
                                    fclose(discolecturaE);
                                    return;
                                }else{
                                    fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                    fseek(discolecturaE, Anterior.part_start, SEEK_SET);
                                    fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);
                                    cout << "" << endl;
                                    cout << "*                 Particion logica creada con exito            *" << endl;
                                    cout << "" << endl;
                                }
                            }
                            break;
                        }
                        fclose(discolecturaE);
                        return;
                    }
                } else if(particion[2].part_type == 'E'){

                    EBR prueba;
                    FILE *discolecturaE;

                    if ((discolecturaE = fopen(path.c_str(), "r+b")) == NULL) {
                            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
                    } else {
                        fseek(discolecturaE, inicioLogica3, SEEK_SET);
                        fread(&prueba, sizeof (EBR), 1, discolecturaE);
                        fclose(discolecturaE);
                    }

                    EBR ParticionLogica = prueba;

                    cout<<"AJuste "<<ParticionLogica.part_fit<<endl;

                    if(ParticionLogica.part_fit != 'B' && ParticionLogica.part_fit != 'F' && ParticionLogica.part_fit != 'W'){
                        cout<<"Entra aqui "<<endl;
                        tamanioTotalParticion = particion[2].part_s;
                        inicioParticion = particion[2].part_start;
                        if(tamanioTotalParticion >= size_file){
                            ParticionLogica.part_status = '0';
                            ParticionLogica.part_fit = ajuste.at(0);
                            ParticionLogica.part_start = inicioParticion;
                            ParticionLogica.part_s = size_file;
                            strcpy(ParticionLogica.part_name, name.c_str());
                            // Como es la primera no tiene siguiente
                            ParticionLogica.part_next = -1;
                            escrituraLogica(ParticionLogica, path, inicioParticion);
                            return;
                    }else{
                        cout<<"¡¡ Error !! El tamanio de la particion logica supera a la particion Extendida "<<endl;
                        return;
                    }
                    }else if(ParticionLogica.part_fit == 'B' || ParticionLogica.part_fit == 'F' || ParticionLogica.part_fit == 'W') {
                        EBR Primero;
                        int inicioLogica3 = particion[2].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica3, SEEK_SET);
                        fread(&Primero, sizeof(EBR), 1, discolecturaE);
                        int tamanioTotal = particion[2].part_start + particion[2].part_s;
                        EBR Anterior;
                        Anterior = Primero;
                        EBR Siguiente;
                        int possig = Primero.part_next;

                        while(true){
                            fseek(discolecturaE, possig, SEEK_SET);
                            fread(&Siguiente, sizeof(EBR), 1, discolecturaE);

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                if(strcmp(Siguiente.part_name, name.c_str()) == 0){
                                cout<<"¡¡ Error !! Ya existe una particion con ese nombre "<<endl;
                                return;
                            }
                                possig = Siguiente.part_next;
                                Anterior = Siguiente;
                            }
                            else {
                                Siguiente.part_start = Anterior.part_start + sizeof(EBR) + Anterior.part_s + 1;
                                cout<<"La siguiente seria "<<Siguiente.part_start<<endl;
                                fseek(discolecturaE, Siguiente.part_start, SEEK_SET);
                                Siguiente.part_status = '0';
                                Siguiente.part_fit = ajuste.at(0);
                                Siguiente.part_s = size_file;
                                Siguiente.part_next = -1;
                                strcpy(Siguiente.part_name, name.c_str());
                                Anterior.part_next = Siguiente.part_start;

                                if(Siguiente.part_start + Siguiente.part_s > tamanioTotal){
                                    cout<<"¡¡ Error !! El tamanio de la particion logica supera al de la extendida "<<endl;
                                    fclose(discolecturaE);
                                    return;
                                }else{
                                    fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                    fseek(discolecturaE, Anterior.part_start, SEEK_SET);
                                    fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);
                                    cout << "" << endl;
                                    cout << "*                 Particion logica creada con exito            *" << endl;
                                    cout << "" << endl;
                                }
                            }
                            break;
                        }
                        fclose(discolecturaE);
                        return;
                    }                    
                } else if(particion[3].part_type == 'E'){

                    EBR prueba;
                    FILE *discolecturaE;

                    if ((discolecturaE = fopen(path.c_str(), "r+b")) == NULL) {
                            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
                    } else {
                        fseek(discolecturaE, inicioLogica4, SEEK_SET);
                        fread(&prueba, sizeof (EBR), 1, discolecturaE);
                        fclose(discolecturaE);
                    }

                    EBR ParticionLogica = prueba;

                    if(ParticionLogica.part_fit != 'B' && ParticionLogica.part_fit != 'F' && ParticionLogica.part_fit != 'W'){
                        tamanioTotalParticion = particion[3].part_s;
                        inicioParticion = particion[3].part_start;
                        if(tamanioTotalParticion >= size_file){
                            ParticionLogica.part_status = '0';
                            ParticionLogica.part_fit = ajuste.at(0);
                            ParticionLogica.part_start = inicioParticion;
                            ParticionLogica.part_s = size_file;
                            strcpy(ParticionLogica.part_name, name.c_str());
                            // Como es la primera no tiene siguiente
                            ParticionLogica.part_next = -1;
                            escrituraLogica(ParticionLogica, path, inicioParticion);
                            return;
                    }else{
                        cout<<"¡¡ Error !! El tamanio de la particion logica supera a la particion Extendida "<<endl;
                        return;
                    }
                    }else if(ParticionLogica.part_fit == 'B' || ParticionLogica.part_fit == 'F' || ParticionLogica.part_fit == 'W') {
                        EBR Primero;
                        int inicioLogica1 = particion[3].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica4, SEEK_SET);
                        fread(&Primero, sizeof(EBR), 1, discolecturaE);
                        int tamanioTotal = particion[3].part_start + particion[3].part_s;
                        EBR Anterior;
                        Anterior = Primero;
                        EBR Siguiente;
                        int possig = Primero.part_next;

                        while(true){
                            fseek(discolecturaE, possig, SEEK_SET);
                            fread(&Siguiente, sizeof(EBR), 1, discolecturaE);

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                if(strcmp(Siguiente.part_name, name.c_str()) == 0){
                                cout<<"¡¡ Error !! Ya existe una particion con ese nombre "<<endl;
                                return;
                            }
                                possig = Siguiente.part_next;
                                Anterior = Siguiente;
                            }
                            else {
                                Siguiente.part_start = Anterior.part_start + sizeof(EBR) + Anterior.part_s + 1;
                                cout<<"La siguiente seria "<<Siguiente.part_start<<endl;
                                fseek(discolecturaE, Siguiente.part_start, SEEK_SET);
                                Siguiente.part_status = '0';
                                Siguiente.part_fit = ajuste.at(0);
                                Siguiente.part_s = size_file;
                                Siguiente.part_next = -1;
                                strcpy(Siguiente.part_name, name.c_str());
                                Anterior.part_next = Siguiente.part_start;

                                if(Siguiente.part_start + Siguiente.part_s > tamanioTotal){
                                    cout<<"¡¡ Error !! El tamanio de la particion logica supera al de la extendida "<<endl;
                                    fclose(discolecturaE);
                                    return;
                                }else{
                                    fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                    fseek(discolecturaE, Anterior.part_start, SEEK_SET);
                                    fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);
                                    cout << "" << endl;
                                    cout << "*                 Particion logica creada con exito            *" << endl;
                                    cout << "" << endl;
                                }
                            }
                            break;
                        }
                        fclose(discolecturaE);
                        return;
                    }
                } else {
                    cout<<"¡¡ Error !! Primero debe crear una particion Extendida "<<endl;
                    return;
                }
        }
        }
    } 
    } else {
            cout<<"¡¡ Error !! EL tamanio del disco en insuficiente para el tamanio de la particion"<<endl;
        }
} 

void Comando::escritura(MBR actualizado, string path){

    FILE *discoescritura;

    if ((discoescritura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !! no se ha podido acceder al disco!\n";
        
    } else {
        fseek(discoescritura, 0, SEEK_SET);
        fwrite(&actualizado, sizeof (MBR), 1, discoescritura);
        fclose(discoescritura);
        cout << "" << endl;
        cout << "*                 Particion creada con exito                   *" << endl;
        cout << "" << endl;
    }
}

void Comando::escrituraLogica(EBR logica, string path, int start){

    FILE *discoescritura;

    if ((discoescritura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !! no se ha podido acceder al disco!\n";
    } else {
        fseek(discoescritura, start, SEEK_SET);
        fwrite(&logica, sizeof (EBR), 1, discoescritura);
        fclose(discoescritura);
        cout << "" << endl;
        cout << "*                 Particion logica creada con exito            *" << endl;
        cout << "" << endl;
    }
}

void Comando::comando_fdisk_modificando(string path, string name, string unit, string del, string add){

    // Leemos el archivo para obtener las particiones

    MBR lectura;
    FILE *discolectura;

    if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
    } else {
        fseek(discolectura, 0, SEEK_SET);
        fread(&lectura, sizeof (MBR), 1, discolectura);
        fclose(discolectura);
    }


    MBR disco = lectura;
    Partition particion[4];
    particion[0] = disco.mbr_partition_1;
    particion[1] = disco.mbr_partition_2;
    particion[2] = disco.mbr_partition_3;
    particion[3] = disco.mbr_partition_4;

    if(del != " "){

        string nombrebuscado;
        if(name == particion[0].part_name){
            nombrebuscado = name;
            //cout<<"Encontrado"<<endl;
        }else if(name == particion[1].part_name){
            nombrebuscado = name;
            //cout<<"Encontrado 1"<<endl;
        } else if(name == particion[2].part_name){
            nombrebuscado = name;
            //cout<<"Encontrado 2"<<endl;
        } else if(name == particion[3].part_name){
            nombrebuscado = name;
            //cout<<"Encontrado 3"<<endl;
        }else{
            //cout<<"No encontrado"<<endl;
            cout<<"¡¡ Error !! NO se encuentra ninguna particion con ese nombre "<<endl;
            return;
        }

        string confirmacion="";
        cout << "La particion sera borrada, ¿esta seguro de eliminarla? ( Y / N )" << endl;
        getline(cin,confirmacion);
        if(confirmacion=="y"||confirmacion=="Y"){
            eliminar(name, path);
        }else{
            cout << "Regresando" << endl;
        }
        cout<<""<<endl;
    }

    else if(add != " "){

    string nombrebuscado;
        if(name == particion[0].part_name){
            nombrebuscado = name;
            cout<<"Encontrado"<<endl;
        }else if(name == particion[1].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 1"<<endl;
        } else if(name == particion[2].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 2"<<endl;
        } else if(name == particion[3].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 3"<<endl;
        }else{
            cout<<"No encontrado"<<endl;
            return;
        }
        agregar(name, path, unit, add);
    }
}

void Comando::eliminar(string name, string path){

    // Vuelve a leer el archivo
    MBR lectura;
    FILE *discolectura;

    if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
    } else {
        fseek(discolectura, 0, SEEK_SET);
        fread(&lectura, sizeof (MBR), 1, discolectura);
        fclose(discolectura);
    }

    // Buscamos todos los datos de la particion
    MBR disco = lectura;
    Partition particion[4];
    particion[0] = disco.mbr_partition_1;
    particion[1] = disco.mbr_partition_2;
    particion[2] = disco.mbr_partition_3;
    particion[3] = disco.mbr_partition_4;

    int inicioLogica = disco.mbr_partition_1.part_start;
    int inicioLogica2 = disco.mbr_partition_2.part_start;
    int inicioLogica3 = disco.mbr_partition_3.part_start;
    int inicioLogica4 = disco.mbr_partition_4.part_start;

    // Volvemos a su configuracion inicial los ajustes
    string nombreparticion = "";
        for (int i = 0; i < 4; i++) {

            if(particion[0].part_status == '0' || particion[1].part_status == '0' || particion[2].part_status == '0' || particion[3].part_status == '0'){
                
            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                
                if(nombreparticion == particion[0].part_name){
                    if(particion[0].part_status == '1'){
                    cout<<"¡¡ Error !! La particion debe ser desmontada primero"<<endl;
                    return;
                    }

                    if(particion[0].part_type == 'E'){
                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(path.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, inicioLogica, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                // Volvemos a su configuracion inicial
                                Anterior.part_fit = '\0';
                                Anterior.part_next = 0;
                                Anterior.part_s = 0;
                                Anterior.part_start = 0;
                                Anterior.part_status = '0';
                                memset(Anterior.part_name,0,16);

                                Siguiente.part_fit = '\0';
                                Siguiente.part_next = 0;
                                Siguiente.part_s = 0;
                                Siguiente.part_start = 0;
                                Siguiente.part_status = '0';
                                memset(Siguiente.part_name,0,16);

                                fseek(discolecturaE, posig, SEEK_SET);
                                fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                fseek(discolecturaE, inicioLogica, SEEK_SET);
                                fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);

                                disco.mbr_partition_1.part_status = '0';
                                disco.mbr_partition_1.part_type = '\0';
                                disco.mbr_partition_1.part_fit = '\0';
                                //disco.mbr_partition_1.part_start = 0;
                                //disco.mbr_partition_1.part_s = 0;
                                memset(disco.mbr_partition_1.part_name,0,16);
                                fseek(discolecturaE, 0, SEEK_SET);
                                fwrite(&disco, sizeof (MBR), 1, discolecturaE);
                                fclose(discolecturaE);
                            }else{
                                disco.mbr_partition_1.part_status = '0';
                                disco.mbr_partition_1.part_type = '\0';
                                disco.mbr_partition_1.part_fit = '\0';
                                //disco.mbr_partition_1.part_start = 0;
                                //disco.mbr_partition_1.part_s = 0;
                                memset(disco.mbr_partition_1.part_name,0,16);
                                actualizardisco(disco, path);
                            }

                        }
                    }else {
                        disco.mbr_partition_1.part_status = '0';
                        disco.mbr_partition_1.part_type = '\0';
                        disco.mbr_partition_1.part_fit = '\0';
                        //disco.mbr_partition_1.part_start = 0;
                        //disco.mbr_partition_1.part_s = 0;
                        memset(disco.mbr_partition_1.part_name,0,16);
                        actualizardisco(disco, path);
                    }
                } else if(nombreparticion == particion[1].part_name){
                    if (particion[1].part_status == '1'){
                    cout<<"¡¡ Error !! La particion debe ser desmontada primero"<<endl;
                    return;
                    }
                    
                    if(particion[1].part_type == 'E'){
                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(path.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, inicioLogica2, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                // Volvemos a su configuracion inicial
                                Anterior.part_fit = '\0';
                                Anterior.part_next = 0;
                                Anterior.part_s = 0;
                                Anterior.part_start = 0;
                                Anterior.part_status = '0';
                                memset(Anterior.part_name,0,16);

                                Siguiente.part_fit = '\0';
                                Siguiente.part_next = 0;
                                Siguiente.part_s = 0;
                                Siguiente.part_start = 0;
                                Siguiente.part_status = '0';
                                memset(Siguiente.part_name,0,16);

                                fseek(discolecturaE, posig, SEEK_SET);
                                fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                fseek(discolecturaE, inicioLogica2, SEEK_SET);
                                fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);

                                disco.mbr_partition_2.part_status = '0';
                                disco.mbr_partition_2.part_type = '\0';
                                disco.mbr_partition_2.part_fit = '\0';
                                //disco.mbr_partition_2.part_start = 0;
                                //disco.mbr_partition_2.part_s = 0;
                                memset(disco.mbr_partition_2.part_name,0,16);
                                fseek(discolecturaE, 0, SEEK_SET);
                                fwrite(&disco, sizeof (MBR), 1, discolecturaE);
                                fclose(discolecturaE);
                            }else{
                                disco.mbr_partition_2.part_status = '0';
                                disco.mbr_partition_2.part_type = '\0';
                                disco.mbr_partition_2.part_fit = '\0';
                                //disco.mbr_partition_2.part_start = 0;
                                //disco.mbr_partition_2.part_s = 0;
                                memset(disco.mbr_partition_2.part_name,0,16);
                                actualizardisco(disco, path);
                            }

                        }
                    }else {
                        disco.mbr_partition_2.part_status = '0';
                        disco.mbr_partition_2.part_type = '\0';
                        disco.mbr_partition_2.part_fit = '\0';
                        //disco.mbr_partition_2.part_start = 0;
                        //disco.mbr_partition_2.part_s = 0;
                        memset(disco.mbr_partition_2.part_name,0,16);
                        actualizardisco(disco, path);
                    }
                } else if(nombreparticion == particion[2].part_name){
                    if (particion[2].part_status == '1'){
                    cout<<"¡¡ Error !! La particion debe ser desmontada primero"<<endl;
                    return;
                    }
                    
                    if(particion[2].part_type == 'E'){
                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(path.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, inicioLogica3, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                // Volvemos a su configuracion inicial
                                Anterior.part_fit = '\0';
                                Anterior.part_next = 0;
                                Anterior.part_s = 0;
                                Anterior.part_start = 0;
                                Anterior.part_status = '0';
                                memset(Anterior.part_name,0,16);

                                Siguiente.part_fit = '\0';
                                Siguiente.part_next = 0;
                                Siguiente.part_s = 0;
                                Siguiente.part_start = 0;
                                Siguiente.part_status = '0';
                                memset(Siguiente.part_name,0,16);

                                fseek(discolecturaE, posig, SEEK_SET);
                                fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                fseek(discolecturaE, inicioLogica3, SEEK_SET);
                                fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);

                                disco.mbr_partition_3.part_status = '0';
                                disco.mbr_partition_3.part_type = '\0';
                                disco.mbr_partition_3.part_fit = '\0';
                                //disco.mbr_partition_3.part_start = 0;
                                //disco.mbr_partition_3.part_s = 0;
                                memset(disco.mbr_partition_3.part_name,0,16);
                                fseek(discolecturaE, 0, SEEK_SET);
                                fwrite(&disco, sizeof (MBR), 1, discolecturaE);
                                fclose(discolecturaE);
                            }else{
                                    disco.mbr_partition_3.part_status = '0';
                                    disco.mbr_partition_3.part_type = '\0';
                                    disco.mbr_partition_3.part_fit = '\0';
                                    //disco.mbr_partition_3.part_start = 0;
                                    //disco.mbr_partition_3.part_s = 0;
                                    memset(disco.mbr_partition_3.part_name,0,16);
                                    actualizardisco(disco, path);
                            }
                        }
                    }else {
                        disco.mbr_partition_3.part_status = '0';
                        disco.mbr_partition_3.part_type = '\0';
                        disco.mbr_partition_3.part_fit = '\0';
                        //disco.mbr_partition_3.part_start = 0;
                        //disco.mbr_partition_3.part_s = 0;
                        memset(disco.mbr_partition_3.part_name,0,16);
                        actualizardisco(disco, path);
                    }
                } else if(nombreparticion == particion[3].part_name){
                    if (particion[3].part_status == '1'){
                    cout<<"¡¡ Error !! La particion debe ser desmontada primero"<<endl;
                    return;
                    }

                    if(particion[3].part_type == 'E'){
                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(path.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, inicioLogica4, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                // Volvemos a su configuracion inicial
                                Anterior.part_fit = '\0';
                                Anterior.part_next = 0;
                                Anterior.part_s = 0;
                                Anterior.part_start = 0;
                                Anterior.part_status = '0';
                                memset(Anterior.part_name,0,16);

                                Siguiente.part_fit = '\0';
                                Siguiente.part_next = 0;
                                Siguiente.part_s = 0;
                                Siguiente.part_start = 0;
                                Siguiente.part_status = '0';
                                memset(Siguiente.part_name,0,16);

                                fseek(discolecturaE, posig, SEEK_SET);
                                fwrite(&Siguiente, sizeof(EBR), 1, discolecturaE);
                                fseek(discolecturaE, inicioLogica4, SEEK_SET);
                                fwrite(&Anterior, sizeof(EBR), 1, discolecturaE);

                                disco.mbr_partition_4.part_status = '0';
                                disco.mbr_partition_4.part_type = '\0';
                                disco.mbr_partition_4.part_fit = '\0';
                                //disco.mbr_partition_4.part_start = 0;
                                //disco.mbr_partition_4.part_s = 0;
                                memset(disco.mbr_partition_4.part_name,0,16);
                                fseek(discolecturaE, 0, SEEK_SET);
                                fwrite(&disco, sizeof (MBR), 1, discolecturaE);
                                fclose(discolecturaE);
                            }else{
                                    disco.mbr_partition_4.part_status = '0';
                                    disco.mbr_partition_4.part_type = '\0';
                                    disco.mbr_partition_4.part_fit = '\0';
                                    //disco.mbr_partition_4.part_start = 0;
                                    //disco.mbr_partition_4.part_s = 0;
                                    memset(disco.mbr_partition_4.part_name,0,16);
                                    actualizardisco(disco, path);
                            }

                        }
                    }else {
                        disco.mbr_partition_4.part_status = '0';
                        disco.mbr_partition_4.part_type = '\0';
                        disco.mbr_partition_4.part_fit = '\0';
                        //disco.mbr_partition_4.part_start = 0;
                        //disco.mbr_partition_4.part_s = 0;
                        memset(disco.mbr_partition_4.part_name,0,16);
                        actualizardisco(disco, path);
                    }
                }
            }
            }else{
                cout<<"¡¡ Error !! La particion debe ser desmontada primero"<<endl;
                return;
            }
        }
}

void Comando::agregar(string name, string path, string unit, string size){

    MBR lectura;
    FILE *discolectura;

    if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
    } else {
        fseek(discolectura, 0, SEEK_SET);
        fread(&lectura, sizeof (MBR), 1, discolectura);
        fclose(discolectura);
    }

    MBR disco = lectura;
    Partition particion[4];
    particion[0] = disco.mbr_partition_1;
    particion[1] = disco.mbr_partition_2;
    particion[2] = disco.mbr_partition_3;
    particion[3] = disco.mbr_partition_4;


    int tamanioParticion1,tamanioParticion2,tamanioParticion3,tamanioParticion4,tamaniosinasignar;
    int espacioRestante1,espacioRestante2,espacioRestante3, espacioRestante4,espacioFinal;
    int espacioFinal1, espacioFinal2, espacioFinal3, espacioFinal4, espacioNoUsado;
    int tamano = atoi(size.c_str());

    int size_file = 0;
    string unidad = "";

    if(unit != " "){
            if(unit == "k" || unit == "K"){
                unidad = "Kb";
                size_file = tamano;
            } else if(unit == "m" || unit == "M"){
                unidad = "Mb";
                size_file = tamano * 1024;
            } else if (unit == "b" || unit == "B"){
                unidad = "b";
            } else{
                cout<<"¡¡ Error !! La unidad solo puede ser: B , K, M "<<endl;
                return;
            }
        } else {
            unidad = "Kb";
            size_file = tamano;
        }

    tamanioParticion1 = (disco.mbr_partition_2.part_start - disco.mbr_partition_1.part_start ) - 1 ;
    tamanioParticion2 = (disco.mbr_partition_3.part_start - disco.mbr_partition_2.part_start ) - 1 ;
    tamanioParticion3 = (disco.mbr_partition_4.part_start - disco.mbr_partition_3.part_start ) - 1 ;
    tamanioParticion4 = (disco.mbr_partition_4.part_s);
    espacioNoUsado = disco.mbr_tamano - (tamanioParticion1 + tamanioParticion2 + tamanioParticion3 + tamanioParticion4);
    espacioRestante1 = tamanioParticion1 - disco.mbr_partition_1.part_s;
    espacioRestante2 = tamanioParticion2 - disco.mbr_partition_2.part_s;
    espacioRestante3 = tamanioParticion3 - disco.mbr_partition_3.part_s;
    espacioRestante4 = espacioNoUsado;

    string nombreparticion = "";
        for (int i = 0; i < 4; i++) {
            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                if(nombreparticion == particion[0].part_name){
                    if(size_file < 0){
                        espacioFinal1 = disco.mbr_partition_1.part_s + size_file;
                        int positivo;
                        positivo = stoi(size);
                        positivo = (positivo * -1);
                        cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                        disco.mbr_partition_1.part_s = espacioFinal1;
                        modificaradd(disco, path);
                    }else {
                        if (espacioRestante1 == 0 || espacioRestante1 < size_file){
                        cout << "¡¡ Error !!  No hay espacio suficiente para agregar tamaño a la particion" << endl;
                        } else if (espacioRestante1 >= size_file){
                        espacioFinal1 = disco.mbr_partition_1.part_s + size_file;
                        cout << "Tamanio final suma " << espacioFinal1 << endl;
                        cout << "Tamanio de la particion " << tamanioParticion1 << endl;
                        if (espacioFinal1 <= tamanioParticion1 && espacioFinal1 > 0){
                            if (size_file > 0){
                                cout << "Se han agregado " << size << " " << unidad << " a la particion" << endl;
                                disco.mbr_partition_1.part_s = espacioFinal1;
                                modificaradd(disco, path);
                            }
                        }
                        }else{
                        cout << "¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible" << endl;
                        }
                    }
                } else if(nombreparticion == particion[1].part_name){
                    if(size_file < 0){
                        espacioFinal2 = disco.mbr_partition_2.part_s + size_file;
                        int positivo;
                        positivo = stoi(size);
                        positivo = (positivo * -1);
                        cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                        disco.mbr_partition_2.part_s = espacioFinal2;
                        modificaradd(disco, path);
                    }else{
                        if (espacioRestante2 == 0 || espacioRestante2 < size_file){
                        cout<<"¡¡ Error !!  No hay espacio suficiente para agregar tamaño a la particion"<<endl;
                        } else if (espacioRestante2 >= size_file){
                        espacioFinal2 = disco.mbr_partition_2.part_s + size_file;
                        cout<<"Tamanio final suma "<<espacioFinal2<<endl;
                        cout<<"Tamanio de la particion "<<tamanioParticion2<<endl;
                        if(espacioFinal2 <= tamanioParticion2 && espacioFinal2 > 0){
                            if(size_file > 0){
                                cout<<"Se han agregado "<<size<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_2.part_s = espacioFinal2;
                                modificaradd(disco, path);
                            }
                    }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                } else if(nombreparticion == particion[2].part_name){
                        if(size_file < 0){
                        espacioFinal3 = disco.mbr_partition_3.part_s + size_file;
                        int positivo;
                        positivo = stoi(size);
                        positivo = (positivo * -1);
                        cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                        disco.mbr_partition_3.part_s = espacioFinal3;
                        modificaradd(disco, path);
                    }else{
                        if (espacioRestante3 == 0 || espacioRestante3 < size_file){
                            cout<<"¡¡ Error !!  No hay espacio suficiente para agregar tamaño a la particion"<<endl;
                        } else if (espacioRestante3 >= size_file){
                            espacioFinal3 = disco.mbr_partition_3.part_s + size_file;
                            cout<<"Tamanio final suma "<<espacioFinal3<<endl;
                            cout<<"Tamanio de la particion "<<tamanioParticion3<<endl;
                            if(espacioFinal3 <= tamanioParticion3 && espacioFinal3 > 0){
                                if(size_file > 0){
                                    cout<<"Se han agregado "<<size<<" "<<unidad<<" a la particion"<<endl;
                                    disco.mbr_partition_3.part_s = espacioFinal3;
                                    modificaradd(disco, path);
                                }
                    }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                } else if(nombreparticion == particion[3].part_name){
                    if(size_file < 0){
                        espacioFinal4 = disco.mbr_partition_4.part_s + size_file;
                        int positivo;
                        positivo = stoi(size);
                        positivo = (positivo * -1);
                        cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                        disco.mbr_partition_4.part_s = espacioFinal4;
                        modificaradd(disco, path);
                    }else{
                        if (espacioRestante4 == 0 || espacioRestante4 < size_file){
                            cout<<"¡¡ Error !! No hay espacio suficiente para agregar tamaño a la particion"<<endl;
                        } else if (espacioRestante4 >= size_file){
                            espacioFinal4 = disco.mbr_partition_4.part_s + size_file;
                            cout<<"Tamanio final suma "<<espacioFinal4<<endl;
                            cout<<"Tamanio de la particion "<<tamanioParticion4<<endl;
                            if(espacioFinal4 <= tamanioParticion4 && espacioFinal4 > 0){
                                if(size_file > 0){
                                    cout<<"Se han agregado "<<size<<" "<<unidad<<" a la particion"<<endl;
                                    disco.mbr_partition_4.part_s = espacioFinal4;
                                    modificaradd(disco, path);
                                }
                    }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                }
            }
        }
}

void Comando::modificaradd(MBR disco, string path){
    MBR actualizado = disco;

    FILE *discoescritura;

    if ((discoescritura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !! no se ha podido acceder al disco!\n";
    } else {
        fseek(discoescritura, 0, SEEK_SET);
        fwrite(&actualizado, sizeof (MBR), 1, discoescritura);
        fclose(discoescritura);
        cout << "" << endl;
        cout << "*                 Particion modificada con exito                *" << endl;
        cout << "" << endl;
    }
}

void Comando:: actualizardisco(MBR disco, string path){
    MBR actualizado = disco;
    FILE *discoescritura;

    if ((discoescritura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !! no se ha podido acceder al disco!\n";
    } else {
        fseek(discoescritura, 0, SEEK_SET);
        fwrite(&actualizado, sizeof (MBR), 1, discoescritura);
        fclose(discoescritura);
        cout << "" << endl;
        cout << "*                 Particion eliminada con exito                *" << endl;
        cout << "" << endl;
    }
}

void Comando:: comando_mount(string path, string name){
    //Empezamos obteniendo los datos
    MBR lectura;
    FILE *discolectura;

    string ultimodigito = "16";
    string numeroparticion = "";
    string nombredisco = "";
    string idnuevo = "";
    int inicioPart;
    string tipoPart;
    int tamaniop = 0;

    if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
    } else {
        fseek(discolectura, 0, SEEK_SET);
        fread(&lectura, sizeof (MBR), 1, discolectura);
        fclose(discolectura);

        string nuevoname = path.substr(path.find_last_of("/") + 1);
        string extension = nuevoname.substr(0, nuevoname.find("."));

        MBR disco = lectura;
        Partition particion[4];
        particion[0] = disco.mbr_partition_1;
        particion[1] = disco.mbr_partition_2;
        particion[2] = disco.mbr_partition_3;
        particion[3] = disco.mbr_partition_4;

        string nombrebuscado;
        if(name == particion[0].part_name){
            if(particion[0].part_status == '0'){
                numeroparticion = "1";
            nombrebuscado = name;
            idnuevo = ultimodigito + numeroparticion + extension;
            tamaniop = particion[0].part_s;
            inicioPart = particion[0].part_start;
            tipoPart = particion[0].part_type;
            if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            } else {
                fseek(discolectura, 0, SEEK_SET);
                disco.mbr_partition_1.part_status = '1';
                fwrite(&disco, sizeof(MBR), 1, discolectura);
                fclose(discolectura);
                montado(idnuevo,path,nombrebuscado,tipoPart,inicioPart, tamaniop);
                }
            }else{
                cout<<"¡¡ Error !! La particion ya se encuentra montada "<<endl;
            }
        }else if(name == particion[1].part_name){
            if(particion[1].part_status == '0'){
            numeroparticion = "2";
            nombrebuscado = name;
            idnuevo = ultimodigito + numeroparticion + extension;
            tamaniop = particion[1].part_s;
            inicioPart = particion[1].part_start;
            tipoPart = particion[1].part_type;
            if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            } else {
                fseek(discolectura, 0, SEEK_SET);
                disco.mbr_partition_2.part_status = '1';
                fwrite(&disco, sizeof(MBR), 1, discolectura);
                fclose(discolectura);
                montado(idnuevo,path,nombrebuscado,tipoPart,inicioPart,tamaniop);
                }
            }else{
                cout<<"¡¡ Error !! La particion ya se encuentra montada "<<endl;
            }            
        } else if(name == particion[2].part_name){
            if(particion[2].part_status == '0'){
            numeroparticion = "3";
            nombrebuscado = name;
            idnuevo = ultimodigito + numeroparticion + extension;
            tamaniop = particion[2].part_s;
            inicioPart = particion[2].part_start;
            tipoPart = particion[2].part_type;
            if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            } else {
                fseek(discolectura, 0, SEEK_SET);
                disco.mbr_partition_3.part_status = '1';
                fwrite(&disco, sizeof(MBR), 1, discolectura);
                fclose(discolectura);
                montado(idnuevo,path,nombrebuscado,tipoPart,inicioPart,tamaniop);
                }
                
            }else{
                cout<<"¡¡ Error !! La particion ya se encuentra montada "<<endl;
            }
        } else if(name == particion[3].part_name){
            if(particion[3].part_status == '0'){
            numeroparticion = "4";
            nombrebuscado = name;
            idnuevo = ultimodigito + numeroparticion + extension;
            tamaniop = particion[3].part_s;
            inicioPart = particion[3].part_start;
            tipoPart = particion[3].part_type;
            if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            } else {
                fseek(discolectura, 0, SEEK_SET);
                disco.mbr_partition_4.part_status = '1';
                fwrite(&disco, sizeof(MBR), 1, discolectura);
                fclose(discolectura);
                montado(idnuevo,path,nombrebuscado,tipoPart,inicioPart,tamaniop);
                }
            }else{
                cout<<"¡¡ Error !! La particion ya se encuentra montada "<<endl;
            }
        }else{
            cout<<"¡¡ Error !! NO se encuentra ninguna particion con ese nombre "<<endl;
            return;
        }
    }
}

void Comando:: montado(string id, string path, string nombreparticion, string tipoparticion, int inicioparticion ,int tamaniopart){
    nodoMount *nuevomontaje = new nodoMount();
    nuevomontaje->id = id;
    nuevomontaje->ruta = path;
    nuevomontaje->nombreparticion = nombreparticion;
    nuevomontaje->tipoparticion = tipoparticion;
    nuevomontaje->inicioparticion = inicioparticion;
    nuevomontaje->tamanioparticion = tamaniopart;
    nuevomontaje->horamontado = std::time(0);

    if(primeroMount ==NULL){
        primeroMount = nuevomontaje;
        primeroMount->siguienteMontado = NULL;
        ultimoMount = primeroMount;
    }else{
        ultimoMount->siguienteMontado = nuevomontaje;
        nuevomontaje->siguienteMontado = NULL;
        ultimoMount = nuevomontaje;
    }
    cout << "" << endl;
    cout << "*                 Particion montada con exito                *" << endl;
    cout << "" << endl;
    verlista();
}

void Comando:: comando_unmount(string id){
    FILE *discolectura;
    MBR lectura;
    string rut = " ";
    string nombre = " ";
    bool encontrado = false;
    nodoMount *actual = primeroMount;
    
    if(actual != NULL) {
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;
            rut = actual->ruta;
            nombre = actual->nombreparticion;
            if ((discolectura = fopen(rut.c_str(), "r+b")) == NULL) {
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            } else {
                fseek(discolectura, 0, SEEK_SET);
                fread(&lectura, sizeof (MBR), 1, discolectura);                

                MBR disco = lectura;
                if(disco.mbr_partition_1.part_name == nombre){
                    if(disco.mbr_partition_1.part_status == '1'){

                        fseek(discolectura, 0, SEEK_SET);
                        disco.mbr_partition_1.part_status = '0';
                        fwrite(&disco, sizeof(MBR), 1, discolectura);
                        desmontado(actual->id);
                        fclose(discolectura);
                    }else{
                        cout<<"¡¡ Error !! Primero debe montar la particion "<<endl;
                    }
                } else if(disco.mbr_partition_2.part_name == nombre){
                    if(disco.mbr_partition_2.part_status == '1'){

                        fseek(discolectura, 0, SEEK_SET);
                        disco.mbr_partition_2.part_status = '0';
                        fwrite(&disco, sizeof(MBR), 1, discolectura);
                        desmontado(actual->id);
                        fclose(discolectura);
                    }else{
                        cout<<"¡¡ Error !! Primero debe montar la particion "<<endl;
                    }
                }else if(disco.mbr_partition_3.part_name == nombre){
                    if(disco.mbr_partition_3.part_status == '1'){

                        fseek(discolectura, 0, SEEK_SET);
                        disco.mbr_partition_3.part_status = '0';
                        fwrite(&disco, sizeof(MBR), 1, discolectura);
                        desmontado(actual->id);
                        fclose(discolectura);
                    }else{
                        cout<<"¡¡ Error !! Primero debe montar la particion "<<endl;
                    }
                }else if(disco.mbr_partition_4.part_name == nombre){
                    if(disco.mbr_partition_4.part_status == '1'){

                        fseek(discolectura, 0, SEEK_SET);
                        disco.mbr_partition_4.part_status = '0';
                        fwrite(&disco, sizeof(MBR), 1, discolectura);
                        desmontado(actual->id);
                        fclose(discolectura);
                    }else{
                        cout<<"¡¡ Error !! Primero debe montar la particion "<<endl;
                    }
                }else{
                    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
                }
            break;
        }
        fclose(discolectura);
        }
        actual = actual->siguienteMontado;
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
    }  else if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }
    
    verlista();
}

void Comando:: desmontado(string id){
    nodoMount *actual = primeroMount;
    nodoMount *anterior = NULL;

if (primeroMount == ultimoMount) {
    if (primeroMount == NULL) {
        cout << "¡¡ Error !! No hay ninguna particion montada" << endl;
        return;
    } else {
        delete primeroMount;
        primeroMount = NULL;
        ultimoMount = NULL;
        cout << "" << endl;
        cout << "*                 Particion desmontada con exito                *" << endl;
        cout << "" << endl;
    }
} else {
    while (actual != NULL) {
        if (actual->id == id) {
            if (anterior == NULL) {
                // si el nodo a eliminar es el primero de la lista
                primeroMount = actual->siguienteMontado;
                if (primeroMount != NULL) {
                    primeroMount->anteriorMontado = NULL;
                } else {
                    ultimoMount = NULL;
                }
            } else if (actual->siguienteMontado == NULL) {
                // si el nodo a eliminar es el último de la lista
                anterior->siguienteMontado = NULL;
                ultimoMount = anterior;
            } else {
                // si el nodo a eliminar no es ni el primero ni el último de la lista
                anterior->siguienteMontado = actual->siguienteMontado;
                actual->siguienteMontado->anteriorMontado = anterior;
            }
            delete actual;
            break;
        }
        anterior = actual;
        actual = actual->siguienteMontado;
    }

    if (actual == NULL) {
        cout << "¡¡ Error !! No hay ninguna particion montada" << endl;
    }
        cout << "" << endl;
        cout << "*                 Particion desmontada con exito                *" << endl;
        cout << "" << endl;
}
}

void Comando:: verlista(){
    string h;
    string hora;
    nodoMount *actualmount = primeroMount;
    if(primeroMount!=NULL){
        cout<<"Particiones montadas actualmente "<<endl;
        cout <<"| ID " <<setw(15) <<"|" << "| Fecha y hora "<<setw(15) << "|"<<endl;
        //cout<<"Hora? "<<std::ctime(&primeroMount->horamontado);
        h = std::ctime(&primeroMount->horamontado);

        while (actualmount!=NULL)
        {
            cout<<"| "<<actualmount->id<<setw(4)<<"|" << "| "<< ctime(&actualmount->horamontado) << " |"<<endl;
            actualmount = actualmount->siguienteMontado;
        }
        }else{
            cout<<""<<endl;
            cout<<"Particiones montadas actualmente "<<endl;
            cout<<"| No ha montado ninguna una particion |"<<endl;
        }
}

void Comando:: comando_mkfs(string id, string type, string fs){
    string sistemaarchivos;
    string tipoformateo;
    int tparticion;
    int partb;

    if(fs == " "){
        sistemaarchivos = "2fs";
    }else if (fs == "2fs"){
        sistemaarchivos = "2fs";
    }else if (fs == "3fs"){
        sistemaarchivos = "3fs";
    }


    if(type == " "){
        tipoformateo = "full";
    }else if (type == "full"){
        tipoformateo = "full";
    }

    nodoMount *actual = primeroMount;
    bool encontrado = false;

    if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }else{
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;

            cout<<"Que datos salen: "<<actual->nombreparticion<<endl;
            cout<<"Que datos salen: "<<actual->id<<endl;
            cout<<"Que datos salen: "<<actual->ruta<<endl;
            cout<<"Que datos salen: "<<ctime(&actual->horamontado)<<endl;
            cout<<"Que datos salen: "<<actual->tamanioparticion<<endl;   

            tparticion = actual->tamanioparticion;

            if(sistemaarchivos == "2fs"){
                partb = tparticion*1024;
                double n; // Numero de Inodos y bloques
                double n_e; // Numero de estructuras

                //Obteniendo el numero de Inodos
                n = (partb-sizeof(SuperBloque))/(4+sizeof(Inodos)+3*64);

                n_e = floor(n);

                cout<<"que sale en particion: "<<partb<<endl;
                cout<<"que sale en super: "<<sizeof(SuperBloque)<<endl;
                cout<<"que sale en Inodos: "<<sizeof(Inodos)<<endl;
                cout<<"que sale en n: "<<n<<endl;
                cout<<"que sale en N_e: "<<n_e<<endl;                
                

                //Formato EXT2
                crear_ext2(actual, n_e, 2);
            }
            if(sistemaarchivos == "3fs"){
                partb = tparticion * 1024;
                double n; // Numero de Inodos y bloques
                double n_e; // Numero de estructuras

                n = (partb-sizeof(SuperBloque))/(4+sizeof(Journaling) + sizeof(Inodos) + 3*64);

                n_e = floor(n);

                cout<<"que sale en particion: "<<partb<<endl;
                cout<<"que sale en super: "<<sizeof(SuperBloque)<<endl;
                cout<<"que sale en Inodos: "<<sizeof(Inodos)<<endl;
                cout<<"que sale en Journaling "<<sizeof(Journaling)<<endl;
                cout<<"que sale en n: "<<n<<endl;
                cout<<"que sale en N_e: "<<n_e<<endl;  

                //Formato EXT3
                crear_ext3(actual, n_e, 3);

            }
            break;
        }
        actual = actual->siguienteMontado;
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
    }   
}

void Comando:: crear_ext2(nodoMount *actual ,int n, int tipop){
    // Se crea el SuperBloque
    SuperBloque SP;
        SP.s_filesystem_type = 2; // Formato EXT2
        SP.s_inodes_count = n;
        SP.s_blocks_count = 3*n;
        SP.s_free_blocks_count = 3*n-2;
        SP.s_free_inodes_count = n-2;
        SP.s_mtime = (actual->horamontado);
        SP.s_umtime = (actual->horamontado);
        SP.s_mnt_count = 1;
        SP.s_magic = 0XEF53;
        SP.s_inode_s = sizeof(Inodos); 
        SP.s_block_s = sizeof(BloqueCarpeta);
        SP.s_firts_ino = (actual->inicioparticion + sizeof(SuperBloque) +  3* n + n); // Es la suma del SuperBloque + BitmapInodos + BitmapBloques
        SP.s_first_blo = SP.s_firts_ino + n * sizeof(Inodos); // Es la suma del primer Primer Inodo Libre + el tamaño de Inodos
        SP.s_bm_inode_start = actual->inicioparticion + sizeof(SuperBloque); // Es la suma del Inicio de la particion + tamaño del SuperBloque
        SP.s_bm_block_start = SP.s_bm_inode_start + n; // Es la suma de donde empieza el BitmapInodos + tamaño de Inodos
        SP.s_inode_start = SP.s_firts_ino; // Es el primer Inodo libre
        SP.s_block_start = SP.s_inode_start + n *sizeof(Inodos); // Es el primer Bloque libre

        Escribir_SuperBloque(actual->ruta, SP, actual->inicioparticion);

    //Se crea el Bitmap de Inodos
    BitMapInodo bmInodo[n];
    BitMapInodo siguientes;

        bmInodo[0].status = '1';
        bmInodo[1].status = '1';
        siguientes.status = '0';

        for(int i =2; i<n;i++){
            bmInodo[i]= siguientes;
        }

        Escribir_BitMapInodos(actual->ruta,bmInodo, SP.s_bm_inode_start, n);

    BitMapBloque bmBloque[n*3];
    BitMapBloque siguientesM;

        bmBloque[0].status = '1';
        bmBloque[1].status = '1';
        siguientesM.status = '0';

        for(int i=2; i<n*3;i++){
            bmBloque[i] = siguientesM;
        }

        Escribir_BitMapBloques(actual->ruta, bmBloque, SP.s_bm_block_start, n*3);

    // Se crean manualmente los primeros Inodos

    //Como primer usuario el ROOT

    Inodos userRoot;

    for(int i =0; i <16; i++){
        userRoot.i_block[i] = -1;
    }

    Inodos Predeterminado[2];

    //Inodo Carpeta raiz
    Predeterminado[0].i_uid = 1;
    Predeterminado[0].I_gid = 1;
    Predeterminado[0].i_atime = std::time(0);
    Predeterminado[0].i_ctime = std::time(0);
    Predeterminado[0].i_mtime = std::time(0);
    Predeterminado[0].i_block[0] = 0;
    Predeterminado[0].i_type = '0';
    Predeterminado[0].i_perm = 664;

    //Inodo Archivo Users
    Predeterminado[1].i_uid = 1;
    Predeterminado[1].I_gid = 1;
    Predeterminado[1].i_atime = std::time(0);
    Predeterminado[1].i_ctime = std::time(0);
    Predeterminado[1].i_mtime = std::time(0);
    Predeterminado[1].i_block[0] = 1;
    Predeterminado[1].i_type = '1';
    Predeterminado[1].i_perm = 700;

    Escribir_Inodos(actual->ruta, Predeterminado, SP.s_inode_start, n);

    // Se actualiza el primero Inodo Libre
    SP.s_firts_ino = SP.s_firts_ino + 2 * sizeof(Inodos);

    //Se crean manualmente los primeros Bloques

    //Bloque Carpeta
    BloqueCarpeta Carpeta;
    Content contenidoCarpeta;
    contenidoCarpeta.b_inodo = 1;
    strcpy(contenidoCarpeta.b_name, "users.txt");

    Carpeta.b_content[0] = contenidoCarpeta;
    // Carpeta.b_content[1].b_inodo = -1;
    // Carpeta.b_content[2].b_inodo = -1;
    // Carpeta.b_content[3].b_inodo = -1;

    Escribir_BloqueCarpeta(actual->ruta, Carpeta, SP.s_first_blo);

    SP.s_first_blo = SP.s_first_blo + sizeof(BloqueCarpeta);

    //Bloque Archivos
    BloqueArchivos Archivo;
    strcpy(Archivo.b_content, "1,G,root\n1,U,root,root,123\n");

    Escribir_BloqueArchivo(actual->ruta, Archivo, SP.s_first_blo);

    SP.s_first_blo = SP.s_first_blo + sizeof(BloqueArchivos);

    //Se actualiza el SuperBloque
    Actualizar_SuperBloque(actual->ruta, SP, actual->inicioparticion);

    cout<<" "<<endl;
    cout<<"*                Formato EXT2 creado con exito             *"<<endl;
    cout<<" "<<endl;
}


void Comando:: crear_ext3(nodoMount *actual, int n, int inicio){
     // Se crea el SuperBloque
    SuperBloque SP;
        SP.s_filesystem_type = 3; // Formato EXT3
        SP.s_inodes_count = n;
        SP.s_blocks_count = 3*n;
        SP.s_free_blocks_count = 3*n-2;
        SP.s_free_inodes_count = n-2;
        SP.s_mtime = (actual->horamontado);
        SP.s_umtime = (actual->horamontado);
        SP.s_mnt_count = 1;
        SP.s_magic = 0XEF53;
        SP.s_inode_s = sizeof(Inodos); 
        SP.s_block_s = sizeof(BloqueCarpeta);
        SP.s_firts_ino = (actual->inicioparticion + sizeof(SuperBloque) + n + n * sizeof(Journaling) + 3 * n); // Es la suma del SuperBloque + Journaling + BitmapInodos + BitmapBloques
        SP.s_first_blo = SP.s_firts_ino + n * sizeof(Inodos); // Es la suma del primer Primer Inodo Libre + el tamaño de Inodos
        SP.s_bm_inode_start = actual->inicioparticion + sizeof(SuperBloque) + n * sizeof(Journaling); // Es la suma del Inicio de la particion + tamaño del SuperBloque
        SP.s_bm_block_start = SP.s_bm_inode_start + n; // Es la suma de donde empieza el BitmapInodos + tamaño de Inodos
        SP.s_inode_start = SP.s_firts_ino; // Es el primer Inodo libre
        SP.s_block_start = SP.s_inode_start + n *sizeof(Inodos); // Es el primer Bloque libre

        Escribir_SuperBloque(actual->ruta, SP, actual->inicioparticion);

    // Se crea el Journalist
    Journaling jour[n];
        int inicioJour = actual->inicioparticion + sizeof(SuperBloque);

        Escribir_Journaling(actual->ruta, jour, inicioJour, n);

    //Se crea el Bitmap de Inodos
    BitMapInodo bmInodo[n];
    BitMapInodo siguientes;

        bmInodo[0].status = '1';
        bmInodo[1].status = '1';
        siguientes.status = '0';

        for(int i =2; i<n;i++){
            bmInodo[i]= siguientes;
        }

        Escribir_BitMapInodos(actual->ruta,bmInodo, SP.s_bm_inode_start, n);

    BitMapBloque bmBloque[n*3];
    BitMapBloque siguientesM;

        bmBloque[0].status = '1';
        bmBloque[1].status = '1';
        siguientesM.status = '0';

        for(int i=2; i<n*3;i++){
            bmBloque[i] = siguientesM;
        }

        Escribir_BitMapBloques(actual->ruta, bmBloque, SP.s_bm_block_start, n*3);

    // Se crean manualmente los primeros Inodos

    //Como primer usuario el ROOT

    Inodos userRoot;

    for(int i =0; i <16; i++){
        userRoot.i_block[i] = -1;
    }

    Inodos Predeterminado[2];

    //Inodo Carpeta raiz
    Predeterminado[0].i_uid = 1;
    Predeterminado[0].I_gid = 1;
    Predeterminado[0].i_atime = std::time(0);
    Predeterminado[0].i_ctime = std::time(0);
    Predeterminado[0].i_mtime = std::time(0);
    Predeterminado[0].i_block[0] = 0;
    Predeterminado[0].i_type = '0';
    Predeterminado[0].i_perm = 664;

    //Inodo Archivo Users
    Predeterminado[1].i_uid = 1;
    Predeterminado[1].I_gid = 1;
    Predeterminado[1].i_atime = std::time(0);
    Predeterminado[1].i_ctime = std::time(0);
    Predeterminado[1].i_mtime = std::time(0);
    Predeterminado[1].i_block[0] = 1;
    Predeterminado[1].i_type = '1';
    Predeterminado[1].i_perm = 700;

    Escribir_Inodos(actual->ruta, Predeterminado, SP.s_inode_start, n);

    // Se actualiza el primero Inodo Libre
    SP.s_firts_ino = SP.s_firts_ino + 2 * sizeof(Inodos);

    //Se crean manualmente los primeros Bloques

    //Bloque Carpeta
    BloqueCarpeta Carpeta;
    Content contenidoCarpeta;
    contenidoCarpeta.b_inodo = 1;
    strcpy(contenidoCarpeta.b_name, "users.txt");

    Carpeta.b_content[0] = contenidoCarpeta;
    // Carpeta.b_content[1].b_inodo = -1;
    // Carpeta.b_content[2].b_inodo = -1;
    // Carpeta.b_content[3].b_inodo = -1;

    Escribir_BloqueCarpeta(actual->ruta, Carpeta, SP.s_first_blo);

    SP.s_first_blo = SP.s_first_blo + sizeof(BloqueCarpeta);

    //Bloque Archivos
    BloqueArchivos Archivo;
    strcpy(Archivo.b_content, "1,G,root\n1,U,root,root,123\n");

    Escribir_BloqueArchivo(actual->ruta, Archivo, SP.s_first_blo);

    SP.s_first_blo = SP.s_first_blo + sizeof(BloqueArchivos);

    //Se actualiza el SuperBloque
    Actualizar_SuperBloque(actual->ruta, SP, actual->inicioparticion);

    cout<<" "<<endl;
    cout<<"*                Formato EXT3 creado con exito             *"<<endl;
    cout<<" "<<endl;

}


void Comando:: Escribir_SuperBloque(string path, SuperBloque SP , int inicio){
    FILE *escribirSP;

     if ((escribirSP = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }

        fseek(escribirSP, inicio, SEEK_SET);
        fwrite(&SP, sizeof(SuperBloque), 1 , escribirSP);
        fclose(escribirSP);
}


void Comando:: Escribir_BitMapInodos(string path, BitMapInodo bmInodo[], int inicio, int n){
    FILE *escrituraBMI;

    if ((escrituraBMI = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<n;i++){
        fseek(escrituraBMI, inicio+i*(sizeof(BitMapInodo)), SEEK_SET);
        fwrite(&bmInodo[i], sizeof(BitMapInodo), 1 , escrituraBMI);
        }
        fclose(escrituraBMI);
}


void Comando:: Escribir_BitMapBloques(string path, BitMapBloque bmBloque[], int inicio, int n){
    FILE *escrituraBMB;

    if ((escrituraBMB = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<n;i++){
        fseek(escrituraBMB, inicio+i , SEEK_SET);
        fwrite(&bmBloque[i], sizeof(BitMapBloque), 1 , escrituraBMB);
        }
        fclose(escrituraBMB);

}


void Comando:: Escribir_Inodos(string path, Inodos inodo[], int inicio, int n){
    FILE *escrituraInodo;

    if ((escrituraInodo = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<2;i++){
        fseek(escrituraInodo, inicio+i*(sizeof(Inodos)) , SEEK_SET);
        fwrite(&inodo[i], sizeof(inodo[i]), 1 , escrituraInodo);
        }
        fclose(escrituraInodo);
}


void Comando:: Escribir_BloqueCarpeta(string path, BloqueCarpeta Carpeta, int inicio){
    FILE *escrituraBCarpeta;

    if ((escrituraBCarpeta = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<1;i++){
        fseek(escrituraBCarpeta, inicio , SEEK_SET);
        fwrite(&Carpeta, sizeof(BloqueCarpeta), 1 , escrituraBCarpeta);
        }
        fclose(escrituraBCarpeta);
}


void Comando:: Escribir_BloqueArchivo(string path, BloqueArchivos Archivo, int inicio){
    FILE *escrituraBArchivo;

    if ((escrituraBArchivo = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<1;i++){
        fseek(escrituraBArchivo, inicio , SEEK_SET);
        fwrite(&Archivo, sizeof(BloqueArchivos), 1 , escrituraBArchivo);
        }
        fclose(escrituraBArchivo);
}

void Comando:: Escribir_BloqueApuntadores(string path, BloqueApuntadores Apuntador, int inicio){
    FILE *escrituraBApuntador;

    if ((escrituraBApuntador = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<1;i++){
        fseek(escrituraBApuntador, inicio , SEEK_SET);
        fwrite(&Apuntador, sizeof(BloqueApuntadores), 1 , escrituraBApuntador);
        }
        fclose(escrituraBApuntador);
}

void Comando:: Escribir_Journaling(string path, Journaling jour[], int inicio, int n){
    FILE *escribirJournaling;

    if ((escribirJournaling = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }
        for(int i=0; i<n;i++){
        fseek(escribirJournaling, inicio + i * (sizeof(Journaling)), SEEK_SET);
        fwrite(&jour[i], sizeof(Journaling), 1 , escribirJournaling);
        }
        fclose(escribirJournaling);
}


void Comando:: Actualizar_SuperBloque(string path, SuperBloque SP, int inicio){
    FILE *escribirSP;

    if ((escribirSP = fopen(path.c_str(), "r+b")) == NULL){
            cout << "¡¡ Error !! No se pudo acceder al disco" << endl;
            return;
        }

        fseek(escribirSP, inicio, SEEK_SET);
        fwrite(&SP, sizeof(SuperBloque), 1 , escribirSP);
        fclose(escribirSP);
}


void Comando:: comando_login(string user, string pass, string id){
    nodoMount *actual = primeroMount;
    string pathdisco = " ";
    bool encontrado = false;

    SuperBloque lectura;
    FILE* discolectura;
    string usuariostxt;
    bool logeadoon = false;


    if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }else{
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;
            pathdisco = actual->ruta;

            nodoLogin *verificaruser = loginregistrado;

            if(verificaruser != NULL){
                logeadoon = true;
                cout<<"¡¡ Error !! Primero debe cerrar sesion "<<endl;
                return;
            }
            if(user == "root"){

                

                if ((discolectura = fopen(pathdisco.c_str(), "r+b")) == NULL) {
        
                cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
            
                } else {

                    fseek(discolectura, actual->inicioparticion, SEEK_SET);
                    fread(&lectura, sizeof(SuperBloque), 1, discolectura);

                    int pos2 = lectura.s_inode_start + sizeof(Inodos)  ;

                    Inodos usuarios;
                        fseek(discolectura, pos2 , SEEK_SET);
                        fread(&usuarios, sizeof(usuarios), 1, discolectura);
                        cout << "tipo sale de Inodo " << usuarios.i_type <<endl;
                        cout << "Grupo sale de Inodo " << usuarios.I_gid <<endl;
                        cout << "Usuario sale de Inodo " << usuarios.i_uid <<endl;
                        cout << "Permiso sale de Inodo " << usuarios.i_perm <<endl;
                    

                    // Se busca al usuario en el archivo

                    BloqueArchivos usuarioroot;

                    int posArchivo = lectura.s_block_start + sizeof(BloqueArchivos);
                    //int posArchivo = lectura.s_block_start + usuarios.i_block * sizeof(BloqueArchivos);

                        fseek(discolectura, posArchivo, SEEK_SET);
                        fread(&usuarioroot, sizeof(BloqueArchivos), 1 , discolectura);

                        

                        usuariostxt=usuarioroot.b_content;

                        cout<<"Que sale en prueba "<<usuariostxt<<endl;
                    fclose(discolectura);

                    // Separando los datos 
                    vector<string> separado;
                    size_t poss = 0;
                    string separador = ",";

                    while((poss = usuariostxt.find(separador)) != string::npos){
                        string palabra = usuariostxt.substr(0,poss);
                        separado.push_back(palabra);
                        usuariostxt.erase(0, poss + separador.length());
                    }
                    separado.push_back(usuariostxt);

                    string userroot = separado[5];
                    int passroot =  stoi(separado[6]);

                    cout<<"Que sale ? "<<userroot<<endl;
                    cout<<"que sale? "<<passroot<<endl;

                    cout<<"que sale en pass "<<pass<<endl;
                    int contra = stoi(pass);
                    
                    // if(contra == passroot){
                    //     cout<<"Entra al de la contra"<<endl;
                    // }

                    if(user == userroot && contra == passroot){
                        nodoLogin *usuarioactual = new nodoLogin();
                        usuarioactual->usuario = userroot;
                        if(loginregistrado == NULL){
                            loginregistrado = usuarioactual;
                            cout<<""<<endl;
                            cout<<"*           Se ha iniciado sesion como usuario root        *"<<endl;
                        }else{
                            cout<<"¡¡ Error !! Ya hay un usuario logeado"<<endl;
                        }
                        //cout<<"Tiene que salir el login bien "<<endl;
                        //return;
                    }
            }
            break;
        }
        actual = actual->siguienteMontado;
    }
        }
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
}

void Comando:: comando_logout(){
    nodoLogin *verificarlogin = loginregistrado;

    if(verificarlogin == NULL){
        cout<<"¡¡ Error !! No hay ningun usuario con la sesion iniciada"<<endl;
        return;
    }else{
        delete loginregistrado;
        loginregistrado = NULL;
        cout<<"*            Se ha cerrado sesion correctamente             *"<<endl;
    }
}

void Comando:: comando_mkgrp(string name){
    nodoLogin *verififcarlogin = loginregistrado;
    if(verififcarlogin == NULL){
        cout<<"¡¡ Error !! Primero debe iniciar sesion como usuario root"<<endl;
        return;
    }else{
        if(verififcarlogin->usuario == "root"){
            cout<<"Si es root"<<endl;
        }else{
            cout<<"¡¡ Error !! Este comando solo lo puede ejecutar el usuario root"<<endl;
        }
    }
}


void Comando:: comando_rep(string namerep, string path, string id, string rutaa){
        string ruta = " ";
        int inicio = 0;
        int fin = path.find("/");
        string delimitador = "/";

        while (fin != -1)
        {
            ruta += path.substr(inicio, fin - inicio);
            ruta += "/";
            inicio = fin + delimitador.size();
            fin = path.find("/", inicio);
        }
        string name = path.substr(path.find_last_of("/") + 1);
        string extension = name.substr(0, name.find("."));
        int pos = name.find(".");
        name.erase(0, 1 + pos);

        if(namerep == "mbr"){
            reporte_mbr(extension, path, id);
        }else if(namerep == "disk"){
            reporte_disk(extension,path, id);
        }

        else if(namerep == "sp"){
            reporte_Sb(extension,path, id);
        }
        else{
            cout<<"¡¡ Error !! No se reconoce ese reporte "<<endl;
            return;
        }
}


void Comando:: reporte_mbr(string nombresalida, string path, string id){
    nodoMount *actual = primeroMount;
    string pathdisco = " ";
    int tamanoMBR;
    int dskmbr;
    time_t fechacreacionmbr;
    string dot = "";
    bool encontrado = false;

    MBR lectura;
    FILE* discolectura;

    string rutap = "" ;
    int inicio = 0;
        int fin = path.find("/");
        string delimitador = "/";

        while (fin != -1)
        {
            rutap += path.substr(inicio, fin - inicio);
            rutap += "/";
            inicio = fin + delimitador.size();
            fin = path.find("/", inicio);
        }

    if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }else{
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;
            pathdisco = actual->ruta;

            if ((discolectura = fopen(pathdisco.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
            } else {
                fseek(discolectura, 0, SEEK_SET);
                fread(&lectura, sizeof (MBR), 1, discolectura);
                fclose(discolectura);

                MBR disco = lectura;
                Partition particion[4];
                particion[0] = disco.mbr_partition_1;
                particion[1] = disco.mbr_partition_2;
                particion[2] = disco.mbr_partition_3;
                particion[3] = disco.mbr_partition_4;

                tamanoMBR = (disco.mbr_tamano * 1024);
                dskmbr = disco.mbr_dsk_signature;
                fechacreacionmbr = disco.mbr_fecha_creacion;

                
                dot = dot + "digraph G {\n";
                dot = dot + "parent [\n";
                dot = dot + "shape=plaintext\n";
                dot = dot + "label=<\n";
                dot = dot + "<table border=\'1\' cellborder=\'1\'>\n";
                dot = dot + "<tr><td bgcolor=\"hotpink\" colspan=\"3\">REPORTE DE MBR</td></tr>\n";
                dot = dot + "<tr><td port='tamanio'>mbr_tamano</td><td port='size'>" + to_string(tamanoMBR) + "</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"hotpink2\" port=\'fecha\'>mbr_fecha</td><td bgcolor=\"hotpink2\" port=\'size\'>" + (ctime(&fechacreacionmbr) ) + "</td></tr>\n";
                dot = dot +  "<tr><td port='dsk'>mbr_dsk</td><td port='size'>" + to_string(dskmbr) +"</td></tr>\n";
                if(particion[0].part_fit != ' '){
                    if(particion[0].part_type == 'P'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[0].part_status;
                        typp = particion[0].part_type;
                        fiit = particion[0].part_fit;
                        int sttart = particion[0].part_start;
                        int ss = (particion[0].part_s *1024);
                        nombrep = particion[0].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";
                }else if (particion[0].part_type == 'E'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[0].part_status;
                        typp = particion[0].part_type;
                        fiit = particion[0].part_fit;
                        int sttart = particion[0].part_start;
                        int ss = (particion[0].part_s *1024);
                        nombrep = particion[0].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";


                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(pathdisco.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, particion[0].part_start, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                string statusa = " ", fitlog = " ", nombrelog = " ";
                                statusa = Anterior.part_status;
                                fitlog = Anterior.part_fit;
                                nombrelog = Anterior.part_name;
                                int neext = Anterior.part_next;
                                int sss = (Anterior.part_s *1024);
                                int startlog = Anterior.part_start;

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog + "</td></tr>\n";

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                string statusa1 = " ", fitlog1 = " ", nombrelog1 = " ";
                                statusa1 = Siguiente.part_status;
                                fitlog1 = Siguiente.part_fit;
                                nombrelog1 = Siguiente.part_name;
                                int neext1 = Siguiente.part_next;
                                int sss1 = (Siguiente.part_s * 1024);
                                int startlog1 = Siguiente.part_start;

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa1 + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog1 + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss1) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog1 + "</td></tr>\n";
                            }
                                fclose(discolecturaE);
                            }
                        }
                }
            }
            if(particion[1].part_fit != ' '){
                    if(particion[1].part_type == 'P'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[1].part_status;
                        typp = particion[1].part_type;
                        fiit = particion[1].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + 1);
                        int ss = (particion[1].part_s *1024);
                        nombrep = particion[1].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";
                }else if (particion[1].part_type == 'E'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[1].part_status;
                        typp = particion[1].part_type;
                        fiit = particion[1].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + 1);
                        int ss = (particion[1].part_s *1024);
                        nombrep = particion[1].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";


                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(pathdisco.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, particion[1].part_start, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                string statusa = " ", fitlog = " ", nombrelog = " ";
                                statusa = Anterior.part_status;
                                fitlog = Anterior.part_fit;
                                nombrelog = Anterior.part_name;
                                int neext = Anterior.part_next;
                                int sss = (Anterior.part_s *1024);
                                int startlog = (particion[0].part_start + (particion[0].part_s *1024) + 1);

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog + "</td></tr>\n";

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                string statusa1 = " ", fitlog1 = " ", nombrelog1 = " ";
                                statusa1 = Siguiente.part_status;
                                fitlog1 = Siguiente.part_fit;
                                nombrelog1 = Siguiente.part_name;
                                int neext1 = Siguiente.part_next;
                                int sss1 = (Siguiente.part_s * 1024);
                                int startlog1 = startlog + (Anterior.part_s *1024 ) + 1;

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa1 + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog1 + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss1) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog1 + "</td></tr>\n";
                            }
                                fclose(discolecturaE);
                            }
                        }
                }
            }
            if(particion[2].part_fit != ' '){
                    if(particion[2].part_type == 'P'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[2].part_status;
                        typp = particion[2].part_type;
                        fiit = particion[2].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + 1);
                        int ss = (particion[2].part_s *1024);
                        nombrep = particion[2].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";
                }else if (particion[2].part_type == 'E'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[2].part_status;
                        typp = particion[2].part_type;
                        fiit = particion[2].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + 1);
                        int ss = (particion[2].part_s *1024);
                        nombrep = particion[2].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";


                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(pathdisco.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, particion[2].part_start, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                string statusa = " ", fitlog = " ", nombrelog = " ";
                                statusa = Anterior.part_status;
                                fitlog = Anterior.part_fit;
                                nombrelog = Anterior.part_name;
                                int neext = Anterior.part_next;
                                int sss = (Anterior.part_s *1024);
                                int startlog = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + 1);

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog + "</td></tr>\n";

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                string statusa1 = " ", fitlog1 = " ", nombrelog1 = " ";
                                statusa1 = Siguiente.part_status;
                                fitlog1 = Siguiente.part_fit;
                                nombrelog1 = Siguiente.part_name;
                                int neext1 = Siguiente.part_next;
                                int sss1 = (Siguiente.part_s * 1024);
                                int startlog1 = startlog + (Anterior.part_s * 1024) + 1;

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa1 + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog1 + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss1) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog1 + "</td></tr>\n";
                            }
                                fclose(discolecturaE);
                            }
                        }
                }
            }
            if(particion[3].part_fit != ' '){
                    if(particion[3].part_type == 'P'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[3].part_status;
                        typp = particion[3].part_type;
                        fiit = particion[3].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + (particion[2].part_s * 1024) + 1);
                        int ss = (particion[3].part_s *1024);
                        nombrep = particion[3].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";
                }else if (particion[3].part_type == 'E'){
                        string statt = " ";
                        string typp = " ";
                        string fiit = " ";
                        string nombrep = " ";

                        statt = particion[3].part_status;
                        typp = particion[3].part_type;
                        fiit = particion[3].part_fit;
                        int sttart = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + (particion[2].part_s * 1024) + 1);
                        int ss = (particion[3].part_s *1024);
                        nombrep = particion[3].part_name;

                        dot = dot +  "<tr><td bgcolor=\"khaki1\" colspan=\"3\">PARTICION</td></tr>\n";
                        dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statt + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='type'>part_type</td><td bgcolor=\"khaki2\" port='size'>"+ typp + "</td></tr>\n";
                        dot = dot +  "<tr><td port='fit'>part_fit</td><td port='size'>"+ fiit + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='start'>part_start</td><td bgcolor=\"khaki2\" port='size'>"+ to_string(sttart) + "</td></tr>\n";
                        dot = dot +  "<tr><td port='s'>part_s</td><td port='size'>"+ to_string(ss) + "</td></tr>\n";
                        dot = dot +  "<tr><td bgcolor=\"khaki2\" port='name'>part_name</td><td bgcolor=\"khaki2\" port='size'>"+ nombrep + "</td></tr>\n";


                        EBR Logica;
                        FILE *discolecturaE;

                        if((discolecturaE = fopen(pathdisco.c_str(), "r+b")) == NULL){
                            cout<<"¡¡ Error !! No se puede acceder al disco \n"<<endl;
                        }else {
                            fseek(discolecturaE, particion[3].part_start, SEEK_SET);
                            fread(&Logica, sizeof(EBR), 1, discolecturaE);

                            if(Logica.part_fit == 'B' || Logica.part_fit == 'F' || Logica.part_fit == 'W'){
                                EBR Anterior;
                                Anterior = Logica;
                                int posig = Logica.part_next;
                                EBR Siguiente;
                                fseek(discolecturaE, Anterior.part_next, SEEK_SET);
                                fread(&Siguiente, sizeof(EBR) ,1, discolecturaE);

                                string statusa = " ", fitlog = " ", nombrelog = " ";
                                statusa = Anterior.part_status;
                                fitlog = Anterior.part_fit;
                                nombrelog = Anterior.part_name;
                                int neext = Anterior.part_next;
                                int sss = (Anterior.part_s *1024);
                                int startlog = (particion[0].part_start + (particion[0].part_s *1024) + (particion[1].part_s * 1024) + (particion[2].part_s * 1024) + 1);

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog + "</td></tr>\n";

                            if(Siguiente.part_fit == 'B' || Siguiente.part_fit == 'F' || Siguiente.part_fit == 'W'){
                                string statusa1 = " ", fitlog1 = " ", nombrelog1 = " ";
                                statusa1 = Siguiente.part_status;
                                fitlog1 = Siguiente.part_fit;
                                nombrelog1 = Siguiente.part_name;
                                int neext1 = Siguiente.part_next;
                                int sss1 = (Siguiente.part_s * 1024);
                                int startlog1 = startlog + (Anterior.part_s * 1024) + 1 ;

                                dot = dot +  "<tr><td bgcolor=\"lightblue1\" colspan=\"3\">PARTICION LOGICA</td></tr>\n";
                                dot = dot +  "<tr><td port='status'>part_status</td><td port='size'>"+ statusa1 + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='fit'>part_fit</td><td bgcolor=\"lightblue2\" port='size'>"+ fitlog1 + "</td></tr>\n";
                                dot = dot +  "<tr><td port='start'>part_start</td><td port='size'>"+ to_string(startlog1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='s'>part_s</td><td bgcolor=\"lightblue2\" port='size'>"+ to_string(sss1) + "</td></tr>\n";
                                dot = dot +  "<tr><td port='next'>part_next</td><td port='size'>"+ to_string(neext1) + "</td></tr>\n";
                                dot = dot +  "<tr><td bgcolor=\"lightblue2\" port='name'>part_name</td><td bgcolor=\"lightblue2\" port='size'>"+ nombrelog1 + "</td></tr>\n";
                            }
                                fclose(discolecturaE);
                            }
                        }
                }
            }
            break;
        }
        actual = actual->siguienteMontado;
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
    }
    dot = dot + "</table>\n";
    dot = dot + ">];\n";
    dot = dot + "}";
    ofstream file;
    file.open("ReporteMBR.dot");
    file << dot;
    file.close();

    string crear = "mkdir -p " + rutap;
    system(crear.c_str());
    string salida = "dot -Tpng ReporteMBR.dot -o " + rutap + nombresalida + ".png";
    system(salida.c_str()); 

    string confirmacion = "";

    cout << ""<<endl;
    cout << "*                 Reporte MRB creado con exito                *" << endl;
    cout << ""<<endl;

    cout <<"*  ¿ Desea abrir el reporte automaticamente ? (Y/N) "<<endl;
    getline(cin,confirmacion);
    if(confirmacion=="y"||confirmacion=="Y"){
        string abrirreporte = rutap + nombresalida + ".png";
        system(abrirreporte.c_str());
    }else{
        cout << " " << endl;
    }
}
}

/* 
* ? Falta Tener bien los espacios libres entre particiones
*/

void Comando:: reporte_disk(string nombresalida, string path, string id){
    nodoMount *actual = primeroMount;
    string pathdisco = " ";
    int tamanoMBR;
    int dskmbr;
    string dot = "";
    string nombredisco = "";
    bool encontrado = false;

    MBR lectura;
    FILE* discolectura;

    string rutap = "" ;
    

    if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }else{
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;
            pathdisco = actual->ruta;
            

            if ((discolectura = fopen(pathdisco.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
            } else {

                int inicio = 0;
                int fin = path.find("/");
                string delimitador = "/";

                while (fin != -1)
                {
                    rutap += path.substr(inicio, fin - inicio);
                    rutap += "/";
                    inicio = fin + delimitador.size();
                    fin = path.find("/", inicio);
                }
                string extension = pathdisco.substr(pathdisco.find_last_of("/") + 1);

                string nombreedisco = extension.substr(0, extension.find("."));

                fseek(discolectura, 0, SEEK_SET);
                fread(&lectura, sizeof (MBR), 1, discolectura);
                fclose(discolectura);

                MBR disco = lectura;
                Partition particion[4];
                particion[0] = disco.mbr_partition_1;
                particion[1] = disco.mbr_partition_2;
                particion[2] = disco.mbr_partition_3;
                particion[3] = disco.mbr_partition_4;

                tamanoMBR = (disco.mbr_tamano * 1024);
                dskmbr = disco.mbr_dsk_signature;

                cout<<"Entra a este reporte " <<endl;

                dot = dot + "digraph G {\n";
                dot = dot + "labelloc=\"t\"\n";
                dot = dot + "label=\"" + nombreedisco + ".dsk\"\n";
                dot = dot + "parent [\n";
                dot = dot + "shape=plaintext\n";
                dot = dot + "label=<\n";
                dot = dot + "<table border=\'1\' cellborder=\'1\'>\n";
                dot = dot + "<tr> <td rowspan='3'>MBR</td>\n";





//        <td rowspan="3"> Espacio Libre <br/>20% del disco </td>
//        <td colspan="4" rowspan="1">Extendida</td>
//        <td rowspan='3'>Primaria <br/>20% del disco</td>
//        <td rowspan='3'>Primaria <br/>10% del disco </td>
//        </tr>
//        <tr>
//        <td rowspan="2" port='log1'>EBR</td><td rowspan="2" port='log2'>Logica <br/>25% del disco</td>
//        <td rowspan="2" port='log1'>EBR</td><td rowspan="2" port='log2'>Logica <br/>25% del disco</td>


            break;
        }
        actual = actual->siguienteMontado;
    }
        }
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
    dot = dot + "</tr>\n";  
    dot = dot + "</table>\n";
    dot = dot +  ">];\n";
    dot = dot +  "}\n";
    ofstream file;
    file.open("ReporteDisk.dot");
    file << dot;
    file.close();

    string crear = "mkdir -p " + rutap;
    system(crear.c_str());
    string salida = "dot -Tpng ReporteDisk.dot -o " + rutap + nombresalida + ".png";
    system(salida.c_str());

    cout << ""<<endl;
    cout << "*                 Reporte Disk creado con exito                *" << endl;
    cout << ""<<endl;

}


void Comando:: reporte_Sb(string nombresalida, string path, string id){
    nodoMount *actual = primeroMount;
    string pathdisco = " ";
    int tamanoMBR;
    int dskmbr;
    string dot = "";
    string nombredisco = "";
    bool encontrado = false;

    MBR lectura;
    FILE* discolectura;

    string rutap = "" ;
    int inicio = 0;
    int fin = path.find("/");
    string delimitador = "/";

    while (fin != -1)
    {
        rutap += path.substr(inicio, fin - inicio);
        rutap += "/";
        inicio = fin + delimitador.size();
        fin = path.find("/", inicio);
    }
    

    if(actual == NULL){
        cout<<"¡¡ Error !! No hay ninguna particion montada "<<endl;
        return;
    }else{
        while(actual != NULL){
        if(actual->id == id){
            encontrado = true;
            pathdisco = actual->ruta;
            
            if ((discolectura = fopen(pathdisco.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
            } else {
                SuperBloque reporte;
                fseek(discolectura, actual->inicioparticion, SEEK_SET);
                fread(&reporte, sizeof (SuperBloque), 1, discolectura);
                fclose(discolectura);

                dot = dot + "digraph G {\n";
                dot = dot + "parent [\n";
                dot = dot + "shape=plaintext\n";
                dot = dot + "label=<\n";
                dot = dot + "<table border=\'1\' cellborder=\'1\'>\n";
                dot = dot + "<tr><td bgcolor=\"darkgreen\" colspan=\"3\">REPORTE DE SUPERBLOQUE</td></tr>\n";
                dot = dot + "<tr><td port='fl'>s_filesystem_type</td><td port='siz1'>" + to_string(reporte.s_filesystem_type) + "</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port=\'count\'>s_inodes_count</td><td bgcolor=\"forestgreen\" port=\'siz17\'>" + to_string(reporte.s_inodes_count) + "</td></tr>\n";
                dot = dot +  "<tr><td port='bcount'>s_blocks_count</td><td port='siz2'>" + to_string(reporte.s_blocks_count) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port=\'freeblocks\'>s_free_blocks_count</td><td bgcolor=\"forestgreen\" port=\'siz16\'>" + to_string(reporte.s_free_blocks_count) + "</td></tr>\n";
                dot = dot +  "<tr><td port='freeinodes'>s_free_inodes_count</td><td port='siz3'>" + to_string(reporte.s_free_inodes_count) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port=\'mounttime\'>s_mtime</td><td bgcolor=\"forestgreen\" port=\'size15\'>" + ctime(&reporte.s_mtime) + "</td></tr>\n";
                dot = dot +  "<tr><td port=\'unmounttime\'>s_umtime</td><td port=\'siz4\'>" + ctime(&reporte.s_umtime) + "</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port='mountcount'>s_mnt_count</td><td bgcolor=\"forestgreen\" port='siz14'>" + to_string(reporte.s_mnt_count) +"</td></tr>\n";
                dot = dot +  "<tr><td port='magic'>s_magic</td><td port='siz5'>" + to_string(reporte.s_magic) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port=\'inodes\'>s_inode_s</td><td bgcolor=\"forestgreen\" port=\'siz13\'>" + to_string(reporte.s_inode_s) + "</td></tr>\n";
                dot = dot +  "<tr><td port='sblock'>s_block_s</td><td port='siz6'>" + to_string(reporte.s_block_s) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port=\'sfirstino\'>s_firts_ino</td><td bgcolor=\"forestgreen\" port=\'siz12\'>" + to_string(reporte.s_firts_ino) + "</td></tr>\n";
                dot = dot +  "<tr><td port='sfirstblo'>s_first_blo</td><td port='siz7'>" + to_string(reporte.s_first_blo) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port='bminodes'>s_bm_inode_start</td><td bgcolor=\"forestgreen\" port='siz11'>" + to_string(reporte.s_bm_inode_start) +"</td></tr>\n";
                dot = dot +  "<tr><td port='bmblocks'>s_bm_block_start</td><td port='siz8'>" + to_string(reporte.s_bm_block_start) +"</td></tr>\n";
                dot = dot +  "<tr><td bgcolor=\"forestgreen\" port='inodestart'>s_inode_start</td><td bgcolor=\"forestgreen\" port='siz10'>" + to_string(reporte.s_inode_start) +"</td></tr>\n";
                dot = dot +  "<tr><td port='blockstart'>s_block_start</td><td port='siz9'>" + to_string(reporte.s_block_start) +"</td></tr>\n";

            break;
        }
        actual = actual->siguienteMontado;
    }
        }
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    } 
    dot = dot + "</table>\n";
    dot = dot +  ">];\n";
    dot = dot +  "}\n";
    ofstream file;
    file.open("ReporteSP.dot");
    file << dot;
    file.close();

    string crear = "mkdir -p " + rutap;
    system(crear.c_str());
    string salida = "dot -Tpng ReporteSP.dot -o " + rutap + nombresalida + ".png";
    system(salida.c_str());

    cout << ""<<endl;
    cout << "*                 Reporte SuperBloque creado con exito                *" << endl;
    cout << ""<<endl;

}
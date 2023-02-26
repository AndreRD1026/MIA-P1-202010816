#include "comandos.h"

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
        if(p.Size != " " && p.Path != " " && p.Name != " "){
            comando_fdisk_creando(p.Size,p.Path,p.Name,p.Unit,p.Type,p.Fit,p.Delete,p.Add);
        } else if (p.Path != " " && p.Name != " "){
            comando_fdisk_modificando(p.Path,p.Name,p.Unit,p.Delete,p.Add);
        } else {
            cout << "Error al crear una particion en el Disco "<<endl;
        }
    }
    //Borrar a partir de este        
    // }else if(p.Comando=="verx"){ // Se identifica el tipo de comando
    //     if(p.X != " "){ // Se validan los parametros para el comando
    //         verX(p.X);
    //     }else{
    //         cout << "Error visualizando x registro: Parametros obligatorios no definidos." << endl;
    //     }
    // }else if(p.Comando=="eliminarx"){ // Se identifica el tipo de comando
    //     if(p.X != " "){ // Se validan los parametros para el comando
    //         eliminarX(p.X);
    //     }else{
    //         cout << "Error elimando x registro: Parametros obligatorios no definidos." << endl;
    //     }
    // }
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

            disco.mbr_tamano = tamano;
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
            }

            disco.dsk_fit = ajuste.at(0);

            // CONFIGURANDO PARTICIONES

            disco.mbr_partition_1.part_status = '0';
            disco.mbr_partition_2.part_status = '0';
            disco.mbr_partition_3.part_status = '0';
            disco.mbr_partition_3.part_status = '0';

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
            fwrite(&disco, sizeof(disco), 1, archivo_binario);

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

void Comando::comando_fdisk_creando(string size, string path, string name, string unit, string type, string fit, string del, string add){
    
    int size_file = 0, tamano = atoi(size.c_str());

    string ajuste,tipo, nombre, unidad, tipoo;

    //Buscar el archivo
    MBR disco;
    FILE *f;

    if ((f = fopen(path.c_str(), "r+b")) == NULL) {
        
            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
        
    } else {
        fseek(f, 0, SEEK_SET);
        fread(&disco, sizeof (MBR), 1, f);
        fclose(f);


        Partition particion[4];
        particion[0] = disco.mbr_partition_1;
        particion[1] = disco.mbr_partition_2;
        particion[2] = disco.mbr_partition_3;
        particion[3] = disco.mbr_partition_4;

        if(unit != " "){
            if(unit == "k" || unit == "K"){
                size_file = tamano;
            } else if(unit == "m" || unit == "M"){
                size_file = tamano * 1024;
            } else if (unit == "b" || unit == "B"){

            } else{
                cout<<"¡¡ Error !! La unidad solo puede ser: B , K, M "<<endl;
            }
        } else {
            size_file = tamano;
        }

        if(fit != " "){
            if(fit == "BF" || fit == "bf"){
                ajuste = "BF";
            } else if(fit == "FF" || fit == "ff"){
                ajuste = "FF";
            } else if(fit == "WF" || fit == "wf"){
                ajuste = "WF";
            } else {
                cout<<"¡¡ Error !! El ajuste solo puede ser: BF , FF, WF "<<endl;
            }
        } else {
            ajuste = "WF";
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
            }
        } else {
            tipo = "P";
        }


        //disco.mbr_partition_1.part_name = name.c_str();
        strcpy(disco.mbr_partition_1.part_name, name.c_str());
        disco.mbr_partition_1.part_status = '1';
        disco.mbr_partition_1.part_type = 'P';
        disco.mbr_partition_1.part_fit = 'W';
        disco.mbr_partition_1.part_s = tamano;


        fseek(f, 0, SEEK_SET);
        fwrite(&disco, sizeof (disco), 1, f);
        fclose(f);

        cout<<"Actualizada"<<endl;
    }
    //return retorno;




    // if(tamano>0){

    // }
    
    
    //cout<<"Fdisk"<<endl;
}

void Comando::comando_fdisk_modificando(string path, string name, string unit, string del, string add){
    cout<<"Modificando"<<endl;
}



// Borrar estos despues

// void Comando::escribir(string id, string nombre, string tel, string dir, string x){
//     FILE *archivo_binario;
//     Ejemplo ejm;
//     string nm = "", direc = "";
//     int registros = atoi(x.c_str());

//     archivo_binario = fopen("Ejemplo4.dsk", "rb+");
//     for (int i = 0; i < registros; i++){
//         // Seteo de parametros en Struct
//         ejm.id = i + atoi(id.c_str());
//         nm = nombre;
//         nm += " " + to_string(ejm.id);
//         strcpy(ejm.nombre, nm.c_str());
//         ejm.telefono = (rand() + i) + atoi(tel.c_str());
//         direc = dir;
//         direc += " " + to_string(ejm.id);
//         strcpy(ejm.direccion, direc.c_str());
//         // Movimiento de puntero y escritura de Struct en archivo binario 
//         fseek(archivo_binario, i * sizeof(Ejemplo), SEEK_SET);
//         fwrite(&ejm, sizeof(ejm), 1, archivo_binario);
//         nm = "";
//         direc = "";
//     }
//     fclose(archivo_binario);
// }

// void Comando::vertodo(){
//     FILE *archivo_binario;
//     Ejemplo ejm;
//     int cont = 0;
//     archivo_binario = fopen("Ejemplo4.dsk", "rb+");
//     while (!feof(archivo_binario)){
//         // Movimiento de puntero y lectura de Struct en archivo binario 
//         fseek(archivo_binario,cont*sizeof(Ejemplo), SEEK_SET);
//         fread(&ejm, sizeof(ejm), 1, archivo_binario);
//         mostrar_struct(ejm);
//         cont++;
//     }
//     cont = 0;
//     fclose(archivo_binario);
// }

// void Comando::verX(string x){
//     FILE *archivo_binario;
//     Ejemplo ejm;
//     int xreg = atoi(x.c_str()) - 1;
//     archivo_binario = fopen("Ejemplo4.dsk", "rb+");
//     // Movimiento de puntero y lectura de Struct en archivo binario 
//     fseek(archivo_binario, xreg * sizeof(Ejemplo), SEEK_SET);
//     fread(&ejm, sizeof(ejm), 1, archivo_binario);
//     mostrar_struct(ejm);
//     fclose(archivo_binario);
// }

// void Comando::eliminarX(string x){
//     FILE *archivo_binario;
//     Ejemplo ejm;
//     ejm.id = 0;
//     strcpy(ejm.nombre, " ");
//     ejm.telefono = 0;
//     strcpy(ejm.direccion, " ");
//     int xreg = atoi(x.c_str()) - 1;
//     archivo_binario = fopen("Ejemplo4.dsk", "rb+");
//     // Movimiento de puntero y escritura de Struct en archivo binario
//     fseek(archivo_binario, xreg * sizeof(Ejemplo), SEEK_SET);
//     fwrite(&ejm, sizeof(ejm), 1, archivo_binario);
//     mostrar_struct(ejm);
//     fclose(archivo_binario);
// }

// void Comando::mostrar_struct(Ejemplo ejm){
//     if(ejm.id > 0 && ejm.nombre != "" && ejm.telefono > 0 && ejm.direccion != ""){
//         cout << "ID: ";
//         cout << ejm.id;
//         cout << ", Nombre: ";
//         cout << ejm.nombre;
//         cout << ", Telefono: ";
//         cout << ejm.telefono;
//         cout << ", Direccion: ";
//         cout << ejm.direccion << endl;
//     }
// }
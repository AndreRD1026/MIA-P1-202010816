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

        if(tipo!="1"){
        int tamanoDisponibleAntes = 0;
        int tamanoDisponible = 0;
        //Verificando estados 

        string pruebaa, otroo;
        for (int i = 0; i < 4; i++) {
            pruebaa=particion[i].part_name;
            otroo = particion[i].part_status;
            cout<<"Nombre? "<<pruebaa<<endl;
            cout<<"Estatus "<<otroo<<endl;
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

        if(tipo == "L" || tipo == "l"){
            for (int i = 0; i < 4; i++) {
                string obtenertipoE;
                obtenertipoE = particion[i].part_type;
                if (obtenertipoE == "E" || obtenertipoE == "e"){
                    if(disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
                        tamanoDisponibleAntes = disco.mbr_tamano - disco.mbr_partition_1.part_s;
                        if(tamanoDisponibleAntes > size_file){
                            strcpy(disco.mbr_partition_2.part_name, name.c_str());
                            disco.mbr_partition_2.part_status = '1';
                            disco.mbr_partition_2.part_type = tipo.at(0);
                            disco.mbr_partition_2.part_fit = ajuste.at(0);
                            disco.mbr_partition_2.part_s = size_file;
                            disco.mbr_partition_2.part_start = (sizeof(disco)+1);
                            cout<<"Start: "<<disco.mbr_partition_2.part_start<<endl;
                            escritura(disco,path);
                            cout<<"Particion 2 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
                        tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s);
                        if(tamanoDisponibleAntes > size_file){
                            strcpy(disco.mbr_partition_3.part_name, name.c_str());
                            disco.mbr_partition_3.part_status = '1';
                            disco.mbr_partition_3.part_type = tipo.at(0);
                            disco.mbr_partition_3.part_fit = ajuste.at(0);
                            disco.mbr_partition_3.part_s = size_file;
                            disco.mbr_partition_3.part_start = (sizeof(disco)+1);
                            cout<<"Start: "<<disco.mbr_partition_3.part_start<<endl;
                            escritura(disco,path);
                            cout<<"Particion 3 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    } else if(disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '0'){
                        tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s);
                        if(tamanoDisponibleAntes > size_file){
                            strcpy(disco.mbr_partition_4.part_name, name.c_str());
                            disco.mbr_partition_4.part_status = '1';
                            disco.mbr_partition_4.part_type = tipo.at(0);
                            disco.mbr_partition_4.part_fit = ajuste.at(0);
                            disco.mbr_partition_4.part_s = size_file;
                            disco.mbr_partition_4.part_start = (sizeof(disco)+1);
                            cout<<"Start: "<<disco.mbr_partition_4.part_start<<endl;
                            escritura(disco,path);
                            cout<<"Particion 4 asignada"<<endl;
                        }else{
                            cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                            return;
                        }
                    }
                }else {
                    cout<<" ¡¡ Error !! Para crear una particion logica debe asignar una extendida previamente"<<endl;
                    return;
                }
            }
        }

        if(disco.mbr_partition_1.part_status == '0' && disco.mbr_partition_2.part_status == '0' && disco.mbr_partition_3.part_status == '0' && disco.mbr_partition_4.part_status == '0'){
            tamanoDisponibleAntes = disco.mbr_tamano;
            if(tamanoDisponibleAntes > size_file){
                strcpy(disco.mbr_partition_1.part_name, name.c_str());
                disco.mbr_partition_1.part_status = '1';
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
                disco.mbr_partition_2.part_status = '1';
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
                disco.mbr_partition_3.part_status = '1';
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
                disco.mbr_partition_4.part_status = '1';
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
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion= disco.mbr_tamano - (disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s + espacioLibre);
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_1.part_s<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_1.part_name, name.c_str());
                    disco.mbr_partition_1.part_status = '1';
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
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion= disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s + espacioLibre);
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_2.part_name, name.c_str());
                    disco.mbr_partition_2.part_status = '1';
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
                tamanoDisponibleAntes = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_4.part_s);
                int espacioLibre = disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_3.part_s + disco.mbr_partition_4.part_s);
                int espaciototalparticion= disco.mbr_tamano - (disco.mbr_partition_1.part_s + disco.mbr_partition_2.part_s + disco.mbr_partition_4.part_s + espacioLibre);
                cout<<"Tamanio antes de la particion: "<<disco.mbr_partition_2.part_s<<endl;
                if(espaciototalparticion >= size_file){
                    strcpy(disco.mbr_partition_3.part_name, name.c_str());
                    disco.mbr_partition_3.part_status = '1';
                    disco.mbr_partition_3.part_type = tipo.at(0);
                    disco.mbr_partition_3.part_fit = ajuste.at(0);
                    disco.mbr_partition_3.part_s = size_file;
                    disco.mbr_partition_3.part_start =  (disco.mbr_partition_1.part_start + disco.mbr_partition_1.part_s + 1);
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
        }
            else{
                    cout<<"¡¡ Error !! EL tamanio asignado para la particion es mayor al tamanio disponible del disco"<<endl;
                    return;
                }
        } else if(disco.mbr_partition_1.part_status == '1' && disco.mbr_partition_2.part_status == '1' && disco.mbr_partition_3.part_status == '1' && disco.mbr_partition_4.part_status == '1'){
            cout<<"¡¡ Error !! Ya no hay particiones disponibles"<<endl;
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
        bool encontrado = false;

        string nombrebuscado;
        if(name == particion[0].part_name){
            nombrebuscado = name;
            cout<<"Encontrado"<<endl;
        }else if(name == particion[1].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 1"<<endl;
        } else if(name == particion[2].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 3"<<endl;
        } else if(name == particion[3].part_name){
            nombrebuscado = name;
            cout<<"Encontrado 3"<<endl;
        }else{
            cout<<"No encontrado"<<endl;
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

    // Volvemos a su configuracion inicial los ajustes
    string nombreparticion = "";
        for (int i = 0; i < 4; i++) {
            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                if(nombreparticion == particion[0].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_1.part_status = '0';
                    disco.mbr_partition_1.part_type = '\0';
                    disco.mbr_partition_1.part_fit = '\0';
                    disco.mbr_partition_1.part_start = 0;
                    //disco.mbr_partition_1.part_s = 0;
                    strcpy(disco.mbr_partition_1.part_name, "");
                } else if(nombreparticion == particion[1].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_2.part_type = '\0';
                    disco.mbr_partition_2.part_fit = '\0';
                    disco.mbr_partition_2.part_start = 0;
                    //disco.mbr_partition_1.part_s = 0;
                    strcpy(disco.mbr_partition_2.part_name, "");
                } else if(nombreparticion == particion[2].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_3.part_status = '0';
                    disco.mbr_partition_3.part_type = '\0';
                    disco.mbr_partition_3.part_fit = '\0';
                    disco.mbr_partition_3.part_start = 0;
                    //disco.mbr_partition_1.part_s = 0;
                    strcpy(disco.mbr_partition_3.part_name, "");
                } else if(nombreparticion == particion[4].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_4.part_type = '\0';
                    disco.mbr_partition_4.part_fit = '\0';
                    disco.mbr_partition_4.part_start = 0;
                    //disco.mbr_partition_1.part_s = 0;
                    strcpy(disco.mbr_partition_4.part_name, "");
                }
            }
        }

    // Actualizando disco
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
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
        if(p.Size != " " && p.Path != " " && p.Name != " "){ // Se validan los parametros para el comando
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

                    if(ParticionLogica.part_status != '0'){
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
                    }else if(ParticionLogica.part_status == '0') {
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

                    if(ParticionLogica.part_status != '0'){
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
                    }else if(ParticionLogica.part_status == '0') {
                        EBR Primero;
                        int inicioLogica1 = particion[1].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica1, SEEK_SET);
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

                    if(ParticionLogica.part_status != '0'){
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
                    }else if(ParticionLogica.part_status == '0') {
                        EBR Primero;
                        int inicioLogica1 = particion[2].part_start;
                        discolecturaE = fopen(path.c_str(), "r+b");
                        fseek(discolecturaE, inicioLogica1, SEEK_SET);
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
                        fseek(discolecturaE, inicioLogica2, SEEK_SET);
                        fread(&prueba, sizeof (EBR), 1, discolecturaE);
                        fclose(discolecturaE);
                    }

                    EBR ParticionLogica = prueba;

                    if(ParticionLogica.part_status != '0'){
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
                    }else if(ParticionLogica.part_status == '0') {
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

    // Volvemos a su configuracion inicial los ajustes
    string nombreparticion = "";
        for (int i = 0; i < 4; i++) {
            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                if(nombreparticion == particion[0].part_name){

                    if(particion[0].part_type == 'E'){
                        //Buscar logicas
                        EBR buscar;
                        EBR actualizo;

                        if ((discolectura = fopen(path.c_str(), "r+b")) == NULL) {
                            cout<<"¡¡ Error !!  No se ha podido acceder al disco!\n";
                        
                    } else {
                        fseek(discolectura, particion[0].part_start, SEEK_SET);
                        fread(&buscar, sizeof (EBR), 1, discolectura);
                        
                        buscar.part_fit = '\0';
                        buscar.part_status = '0';
                        buscar.part_s = 0;
                        buscar.part_start = 0;
                        stpcpy(buscar.part_name, " ");

                        fwrite(&buscar, sizeof(EBR), 1, discolectura);
                        fclose(discolectura);
                        disco.mbr_partition_1.part_status = '0';
                        disco.mbr_partition_1.part_type = '\0';
                        disco.mbr_partition_1.part_fit = '\0';
                        //disco.mbr_partition_1.part_start = 0;
                        //disco.mbr_partition_1.part_s = 0;
                        strcpy(disco.mbr_partition_1.part_name, "                ");
                    }
                    }else {
                        cout<<"Tamano: "<<particion[i].part_s<<endl;
                        cout<<"Inicio: "<<particion[i].part_start<<endl;
                        cout<<"Tipo: "<<particion[i].part_type<<endl;
                        cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                        cout<<"Nombre: "<<particion[i].part_name<<endl;
                        disco.mbr_partition_1.part_status = '0';
                        disco.mbr_partition_1.part_type = '\0';
                        disco.mbr_partition_1.part_fit = '\0';
                        //disco.mbr_partition_1.part_start = 0;
                        //disco.mbr_partition_1.part_s = 0;
                        strcpy(disco.mbr_partition_1.part_name, "");
                    }


                    
                } else if(nombreparticion == particion[1].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_2.part_status = '0';
                    disco.mbr_partition_2.part_type = '\0';
                    disco.mbr_partition_2.part_fit = '\0';
                    //disco.mbr_partition_2.part_start = 0;
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
                    //disco.mbr_partition_3.part_start = 0;
                    //disco.mbr_partition_1.part_s = 0;
                    strcpy(disco.mbr_partition_3.part_name, "");
                } else if(nombreparticion == particion[3].part_name){
                    cout<<"Tamano: "<<particion[i].part_s<<endl;
                    cout<<"Inicio: "<<particion[i].part_start<<endl;
                    cout<<"Tipo: "<<particion[i].part_type<<endl;
                    cout<<"Ajuste: "<<particion[i].part_fit<<endl;
                    cout<<"Nombre: "<<particion[i].part_name<<endl;
                    disco.mbr_partition_4.part_status = '0';
                    disco.mbr_partition_4.part_type = '\0';
                    disco.mbr_partition_4.part_fit = '\0';
                    //disco.mbr_partition_4.part_start = 0;
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

    // cout<<"Espacio restante en particion 1 "<<espacioRestante1<<endl;
    // cout<<"Espacio restante en particion 2 "<<espacioRestante2<<endl;
    // cout<<"Espacio restante en particion 3 "<<espacioRestante3<<endl;
    // cout<<"Espacio restante en particion 4 "<<espacioRestante4<<endl;

    string nombreparticion = "";
        for (int i = 0; i < 4; i++) {
            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                if(nombreparticion == particion[0].part_name){
                    if (espacioRestante1 == 0 || espacioRestante1 < size_file){
                        cout<<"¡¡ Error !!  No hay espacio suficiente para agregar tamaño a la particion"<<endl;
                    } else if (espacioRestante1 >= size_file){
                        espacioFinal1 = disco.mbr_partition_1.part_s + size_file;
                        cout<<"Tamanio final suma "<<espacioFinal1<<endl;
                        cout<<"Tamanio de la particion "<<tamanioParticion1<<endl;
                        if(espacioFinal1 <= tamanioParticion1 && espacioFinal1 > 0){
                            if(size_file > 0){
                                cout<<"Se han agregado "<<size<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_1.part_s = espacioFinal1;
                                modificaradd(disco, path);
                            } else {
                                int positivo;
                                positivo = stoi(size);
                                positivo = (positivo * -1);
                                cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_1.part_s = espacioFinal1;
                                modificaradd(disco, path);
                            }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                } else if(nombreparticion == particion[1].part_name){
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
                            } else {
                                int positivo;
                                positivo = stoi(size);
                                positivo = (positivo * -1);
                                cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_2.part_s = espacioFinal2;
                                modificaradd(disco, path);
                            }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                } else if(nombreparticion == particion[2].part_name){
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
                            } else {
                                int positivo;
                                positivo = stoi(size);
                                positivo = (positivo * -1);
                                cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_2.part_s = espacioFinal3;
                                modificaradd(disco, path);
                            }
                        }else {
                            cout<<"¡¡ Error !!  El tamaño que desea agregar es mayor al tamaño disponible"<<endl;
                        }
                    }
                } else if(nombreparticion == particion[3].part_name){
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
                            } else {
                                int positivo;
                                positivo = stoi(size);
                                positivo = (positivo * -1);
                                cout<<"Se han quitado "<<positivo<<" "<<unidad<<" a la particion"<<endl;
                                disco.mbr_partition_4.part_s = espacioFinal4;
                                modificaradd(disco, path);
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
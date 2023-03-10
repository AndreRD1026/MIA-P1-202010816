#include "comandos.h"
#include <cmath>


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
            cout << "Error para desmontar el Disco: Parametros obligatorios no definidos " << endl;
        }
    }else if(p.Comando=="rep"){ // Se identifica el tipo de comando
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

            if(particion[i].part_status == '0'){

            nombreparticion=particion[i].part_name;
            if(nombreparticion==name){
                
                if(nombreparticion == particion[0].part_name){

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
        cout <<"| ID " << setw(15) <<"|" << "| Fecha y hora "<<setw(15) << "|"<<endl;
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

                n = (partb-sizeof(SuperBloque))/(4+sizeof(Inodos)+(3*64));

                n_e = floor(n);

                cout<<"que sale en particion: "<<partb<<endl;
                cout<<"que sale en super: "<<sizeof(SuperBloque)<<endl;
                cout<<"que sale en Inodos: "<<sizeof(Inodos)<<endl;
                cout<<"que sale en n: "<<n<<endl;
                cout<<"que sale en N_e: "<<n_e<<endl;                
                //ni = ()
                //Obteniendo el numero de Inodos

                //Formato EXT2
                crear_ext2(actual, n_e, 2);
            }
            if(sistemaarchivos == "3fs"){

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

// Con este comando estoy llorando sangre :(

void Comando:: crear_ext2(nodoMount *actual ,int n, int tipop){
    // Se crea el SuperBloque

    SuperBloque SP;
        SP.s_filesystem_type = 2;
        SP.s_inodes_count = n;
        SP.s_blocks_count = 3*n;
        SP.s_free_blocks_count = 3*n-2;
        SP.s_free_inodes_count = n-2;
        SP.s_mtime = actual->horamontado;
        SP.s_umtime = actual->horamontado;
        SP.s_mnt_count = 1;
        SP.s_magic = 0XEF53;
        SP.s_inode_s = sizeof(Inodos);
        SP.s_block_s = sizeof(BloqueCarpeta);
        SP.s_firts_ino = (actual->inicioparticion + sizeof(SuperBloque) +  3* n + n);
        SP.s_first_blo = SP.s_firts_ino + n * sizeof(Inodos);
        SP.s_bm_inode_start = actual->inicioparticion + sizeof(SuperBloque);
        SP.s_bm_block_start = SP.s_bm_inode_start + n;
        SP.s_inode_start = SP.s_firts_ino;
        SP.s_block_start = SP.s_inode_start + n *sizeof(Inodos);


    //Se crea el Bitmap de Inodos


    // cout<<"Entra al ext2 "<<endl;
    // cout<<"Que sale ? "<<actual->id<<endl;
    // cout<<"Que sale ? "<<actual->nombreparticion<<endl;
    // cout<<"Que sale ? "<<actual->ruta<<endl;
    // cout<<"Que sale ? "<<actual->tipoparticion<<endl;
    // cout<<"Que sale ? "<<actual->tamanioparticion<<endl;
    // cout<<"Que sale ? "<<actual->inicioparticion<<endl;
    // cout<<"Que sale ? "<<ctime(&actual->horamontado)<<endl;


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
            cout<<"*        Generando reporte          * "<<endl;
            reporte_mbr(extension, path, id);
        }else if(namerep == "disk"){
            cout<<"Disk encontrado "<<endl;
            return;
        }
        else{
            cout<<"¡¡ Error !! No se reconoce ese reporte "<<endl;
            return;
        }
}


void Comando:: reporte_mbr(string nombresalida, string path, string id){
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
            break;
        }
        actual = actual->siguienteMontado;
    }
    if(!encontrado){
    cout<<"¡¡ Error !! No se encuentra ninguna particion con ese ID "<<endl;
    return;
    }
    }   
    // time_t pruebas;
    // pruebas = std::time(0);
    // //cout<<"HOla "<<endl;
    // string dot = "";

    // dot = dot + "digraph H {\n";
    // dot = dot + "parent [\n";
    // dot = dot + "shape=plaintext\n";
    // dot = dot + "label=<\n";
    // dot = dot + "<table border=\'1\' cellborder=\'1\'>\n";
    // dot = dot + "<tr><td colspan=\"3\">REPORTE DE MBR</td></tr>\n";
    // dot = dot + "<tr><td port='tamanio'>mbr_tamano</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td port=\'fecha\'>mbr_fecha</td><td port=\'size\'>" + (ctime(&pruebas) ) + "</td></tr>\n";
    // dot = dot +  "<tr><td port='dsk'>mbr_dsk</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td colspan=\"3\">PARTICION</td></tr>\n";
    // dot = dot +  "<tr><td port='tamanio'>mbr_tamano</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td port='fecha'>mbr_fecha</td><td port='size'>15/03/2023</td></tr>\n";
    // dot = dot + "<tr><td port='dsk'>mbr_dsk</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot + "</table>\n";
    // dot = dot + ">];\n";

    // dot = dot + "parent2 [\n";
    // dot = dot + "shape=plaintext\n";
    // dot = dot + "label=<\n";
    // dot = dot + "<table border=\'1\' cellborder=\'1\'>\n";
    // dot = dot + "<tr><td colspan=\"3\">REPORTE DE EBR</td></tr>\n";
    // dot = dot + "<tr><td port='tamanio'>mbr_tamano</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td port='fecha'>mbr_fecha</td><td port='size'>15/03/2023</td></tr>\n";
    // dot = dot +  "<tr><td port='dsk'>mbr_dsk</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td colspan=\"3\">PARTICION</td></tr>\n";
    // dot = dot +  "<tr><td port='tamanio'>mbr_tamano</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot +  "<tr><td port='fecha'>mbr_fecha</td><td port='size'>15/03/2023</td></tr>\n";
    // dot = dot + "<tr><td port='dsk'>mbr_dsk</td><td port='size'>1521221215t</td></tr>\n";
    // dot = dot + "</table>\n";
    // dot = dot + ">];\n";
    // dot = dot + "}";




    // ofstream file;
    // file.open("PruebaReporte.dot");
    // file << dot;
    // file.close();
    // system(("dot -Tpng PruebaReporte.dot -o  /home/andre/Desktop/ReporteMBR.png"));

}
#include "analizador.h"

vector<string> Analizador::split_txt(string text){ // Split para separar tipo de comando y parametros
    stringstream text_to_split(text);
    string segment;
    vector<string> splited;

    while(std::getline(text_to_split, segment, ' ')){
        splited.push_back(segment);
    }
    return splited;
}

string Analizador::replace_txt(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void Analizador::analizar(string entrada){
    string entradacmd = entrada;
    vector<string> cmdentrada = split_txt(entradacmd);
    vector<string> parametros;
    string comando = "";
    for(int i = 0; i< cmdentrada.size(); i++){
        if(i == 0){
            comando = cmdentrada.at(i);
        }else{
            parametros.push_back(cmdentrada.at(i));
        }
    }
    // Identificacion de comando, parametros y ejecucion
    identificarParametros(comando, parametros);
}

void Analizador::identificarParametros(string comando, vector<string> parametros){
    string param = "";
    if(comando == "mkdisk"){
        cmd.param.Comando = "mkdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">size=") == 0){
                param = replace_txt(param, ">size=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Size = param;
            } else if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                //string nuevo = param;

                //cout<< "que sale? "<<nuevo<<endl;

                // if (param.substr(0, 1) == "\"") {
                //     cout << "La cadena comienza con comillas dobles." << endl;
                //     string ruta = nuevo.substr(1, nuevo.length() - 2);
                //     cout << "Y ahora? "<<ruta<<endl;
                // } else {
                //     cout << "La cadena no comienza con comillas dobles." << endl;
                // }
                cmd.param.Path = param;
            } else if(param.find(">fit=") == 0){
                param = replace_txt(param, ">fit=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Fit = param;
                transform(cmd.param.Fit.begin(), cmd.param.Fit.end(), cmd.param.Fit.begin(), ::tolower);
            } else if(param.find(">unit=") == 0){
                param = replace_txt(param, ">unit=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Unit = param;
                transform(cmd.param.Unit.begin(), cmd.param.Unit.end(), cmd.param.Unit.begin(), ::tolower);
            } else{
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if(comando== "rmdisk"){
        cmd.param.Comando = "rmdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Path = param;
            } else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    }  else if(comando== "fdisk"){
        cmd.param.Comando = "fdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Path = param;
            } else if(param.find(">size=") == 0){
                param = replace_txt(param, ">size=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Size = param;
            } else if(param.find(">name=") == 0){
                param = replace_txt(param, ">name=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Name = param;
            } else if(param.find(">unit=") == 0){
                param = replace_txt(param, ">unit=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Unit = param;
                transform(cmd.param.Unit.begin(), cmd.param.Unit.end(), cmd.param.Unit.begin(), ::tolower);
            }else if(param.find(">type=") == 0){
                param = replace_txt(param, ">type=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Type = param;
                transform(cmd.param.Type.begin(), cmd.param.Type.end(), cmd.param.Type.begin(), ::tolower);
            } else if(param.find(">fit=") == 0){
                param = replace_txt(param, ">fit=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Fit = param;
                transform(cmd.param.Fit.begin(), cmd.param.Fit.end(), cmd.param.Fit.begin(), ::tolower);
            }else if(param.find(">delete=") == 0){
                param = replace_txt(param, ">delete=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Delete = param;
            }else if(param.find(">add=") == 0){
                param = replace_txt(param, ">add=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Add = param;
            }    else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "mount"){
        cmd.param.Comando = "mount";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Path = param;
            }else if(param.find(">name=") == 0){
                param = replace_txt(param, ">name=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Name = param;
            } else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "unmount"){
        cmd.param.Comando = "unmount";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">id=") == 0){
                param = replace_txt(param, ">id=", "");
                cmd.param.ID = param;
            }else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "mkfs"){
        cmd.param.Comando = "mkfs";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">id=") == 0){
                param = replace_txt(param, ">id=", "");
                cmd.param.ID = param;
            }else if(param.find(">type=") == 0){
                param = replace_txt(param, ">type=", "");
                cmd.param.Type = param;
            }else if(param.find(">fs=") == 0){
                param = replace_txt(param, ">fs=", "");
                cmd.param.Fs = param;
            } else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "Login" || comando == "login"){
        cmd.param.Comando = "login";
        transform(cmd.param.Comando.begin(), cmd.param.Comando.end(), cmd.param.Comando.begin(), ::tolower);
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">user=") == 0){
                param = replace_txt(param, ">user=", "");
                cmd.param.User = param;
            }else if(param.find(">pass=") == 0){
                param = replace_txt(param, ">pass=", "");
                cmd.param.Pass = param;
            }else if(param.find(">id=") == 0){
                param = replace_txt(param, ">id=", "");
                cmd.param.ID = param;
            } else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "Logout" || comando == "logout"){
        cmd.param.Comando = "logout";
        transform(cmd.param.Comando.begin(), cmd.param.Comando.end(), cmd.param.Comando.begin(), ::tolower);
        cmd.identificacionCMD(cmd.param);
    } else if (comando == "mkgrp"){
        cmd.param.Comando = "mkgrp";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">name=") == 0){
                param = replace_txt(param, ">name=", "");
                cmd.param.Name = param;
            }else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    } 
    
    
    

    
     else if (comando == "rep"){
        cmd.param.Comando = "rep";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">name=") == 0){
                param = replace_txt(param, ">name=", "");
                cmd.param.Name = param;
                transform(cmd.param.Name.begin(), cmd.param.Name.end(), cmd.param.Name.begin(), ::tolower);
            }else if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Path = param;
            }else if(param.find(">id=") == 0){
                param = replace_txt(param, ">id=", "");
                cmd.param.ID = param;
            }else if(param.find(">ruta=") == 0){
                param = replace_txt(param, ">ruta=", "");
                cmd.param.Ruta = param;
            }  else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
        cmd.identificacionCMD(cmd.param);
    }



    else if (comando == "execute"){
        cmd.param.Comando = "execute";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find(">path=") == 0){
                param = replace_txt(param, ">path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.Path = param;

                string ruta = "";
                int inicio = 0;
                int fin = param.find("/");
                string delimitador = "/";

                while (fin != -1) {
                    ruta += param.substr(inicio, fin - inicio);
                    ruta += "/";
                    inicio = fin + delimitador.size();
                    fin = param.find("/", inicio);
                }

                string extension = param.substr(param.find_last_of("/") + 1);
                string name = extension.substr(0, extension.find("."));
                int pos = extension.find(".");
                extension.erase(0, 1 + pos);

                if (extension != "eea"){
                    cout << " "<<endl;
                    cout << "¡¡ Error !! La extension del archivo no corresponde a .eea" << endl;
                    cout << " "<<endl;
                    return;
                }else{
                    ifstream archivolectura(param);
                    string lineacomando = "";
                    while (getline(archivolectura, lineacomando)){
                        size_t encontrarcomentario = lineacomando.find('#');
                        if (encontrarcomentario != std::string::npos){
                            if(encontrarcomentario == 0){
                                string comentario = lineacomando.substr(encontrarcomentario +1);
                                cout<<"Comentario - "<<comentario<<endl;
                            }else{
                                string comandoantes = lineacomando.substr(0, encontrarcomentario);
                                if(comandoantes.empty()){
                                    string comentario = lineacomando.substr(encontrarcomentario +1);
                                    cout<<"Comentario - "<<comentario<<endl;
                                }else{
                                    cout << "*----------------------------------------------------------*" << endl;
                                    cout << "*                      [MIA] Proyecto 1                    *" << endl;
                                    cout << "*           Cesar Andre Ramirez Davila 202010816           *"<<endl;
                                    cout << "*----------------------------------------------------------*" << endl;
                                    cout << "Ejecutando el comando - " <<lineacomando<<endl;
                                    analizar(comandoantes);
                                }
                            }
                            string comentario = lineacomando.substr(encontrarcomentario +1);
                            cout<<"Comentario - "<<comentario<<endl;
                        }else{
                            cout << "*----------------------------------------------------------*" << endl;
                            cout << "*                      [MIA] Proyecto 1                    *" << endl;
                            cout << "*           Cesar Andre Ramirez Davila 202010816           *"<<endl;
                            cout << "*----------------------------------------------------------*" << endl;
                            cout << "Ejecutando el comando - " <<lineacomando<<endl;
                            analizar(lineacomando);
                        }
                        // if(lineacomando.find("#") == 0){
                        //     cout<<"Comentario - "<<lineacomando<<endl;
                        // }else{
                        //     cout << "*----------------------------------------------------------*" << endl;
                        //     cout << "*                      [MIA] Proyecto 1                    *" << endl;
                        //     cout << "*           Cesar Andre Ramirez Davila 202010816           *"<<endl;
                        //     cout << "*----------------------------------------------------------*" << endl;
                        //     cout << "Ejecutando el comando - " <<lineacomando<<endl;
                        //     analizar(lineacomando);
                        // }
                    }
                    cout << " "<<endl;
                    cout<<"*                 Script ejecutado con exito               *"<<endl;
                    cout << " "<<endl;
                }                
            }else {
                cout<<"Un parametro es incorrecto"<<endl;
            }
        }
    }else if (comando == "exit"){
        cout << " "<<endl;
        cout<<"Gracias por usar el programa :D"<<endl;
    }

    else {
        cout<<"\n ¡¡ Error !!  Comando no reconocido por el sistema"<<endl;
    }

    // Limpiando los parametros
    cmd.param.Comando = " ";
    cmd.param.Size = " ";
    cmd.param.Path = " ";
    cmd.param.Fit = " ";
    cmd.param.Unit = " ";
    cmd.param.Type = " ";
    cmd.param.Delete = " ";
    cmd.param.Add = " ";
    cmd.param.Name = " ";
    cmd.param.ID = " ";
    cmd.param.Fs = " ";
    cmd.param.Ruta = " ";
}
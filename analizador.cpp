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
}
#include "analizador.cpp"

int main(){
    Analizador analisis;
    string entradacmd = "";
    while(entradacmd != "exit"){
        cout << "*----------------------------------------------------------*" << endl;
        cout << "*                      [MIA] Proyecto 1                    *" << endl;
        cout << "*           Cesar Andre Ramirez Davila 202010816           *"<<endl;
        cout << "*----------------------------------------------------------*" << endl;
        cout << "Ingrese un comando - ";
        getline(cin,entradacmd);
        analisis.analizar(entradacmd);
    }
    return 0;
}
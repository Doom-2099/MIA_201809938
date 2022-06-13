#include <iostream>
#include "Analizador.h"

using namespace std;

int main() {
    cout << "\tMANEJO E IMPLEMENTACION DE ARCHIVOS" << endl;
    cout << "\tJORGE MARIO CASTAÑEDA CRAGUA - 201809938" << endl;
    cout << "\tPROYECTO NO. 1" << endl
         << endl
         << "\t";

    while(true)
    {
        string comando = "";
        getline(cin, comando); 
        if(strstr(comando.c_str(), "exit") != NULL) { break; }
        analizarComando(comando);
        cout << "\t";
    }
}
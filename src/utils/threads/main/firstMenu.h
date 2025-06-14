#pragma once
#include <iostream>
using namespace std;
// Return 1 = le dio a jugar
// Return 2 = le dio a continuar
// Return 3 = le dio a salir
// Return 0 = error en algo
int StartMenu()
{
    int option;

    cout << "====== MENU PRINCIPAL ======\n";
    cout << "1. Jugar\n";
    cout << "2. Continuar\n";
    cout << "Seleccione una opción: ";
    cin >> opcion;

    if (opcion == 1)
        return 1;
    else if (opcion == 2)
        return 2;
    else
        return 0;

}
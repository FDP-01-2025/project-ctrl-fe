#include <windows.h>
#include "./src/utils/global.h"
#include <iostream>

using namespace std;

// TODO_________________________________________ MAIN _______________________________________TODO

int main()
{
    //Configuraciones iniciales
    Console C(50, 25, 90, 25);
    C.configConsole();
    C.setTitle(L"LA TORRE DE LEUGIM");

    //Mapear
    identfierMap();

    return 0;
};

#include <windows.h>
#include "./src/utils/global.h"
using namespace std;

// TODO_________________________________________ MAIN _______________________________________TODO
//* Objetivo: Estructura del orden de ejecución.

int main()
{
    Global G;
    //*Primera accion: Mostrar la pantalla de Inicio
    G.showInitialScreen(L"LA TORRE DE LEUGIM");
    std::cin.get(); // Espera a que el usuario presione Enter

    //* Segunda acción: Leer en que estado está el jugador (es decir, primera ejecución, o en algun punto x del juego)
    G.readPlayerStatusPoint();

    return 0;
};

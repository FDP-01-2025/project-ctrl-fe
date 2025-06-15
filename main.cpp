#include <windows.h>
#include "./global.h"
using namespace std;

// TODO_________________________________________ MAIN _______________________________________TODO
//* Objetivo: Estructura del orden de ejecución.

int main()
{
    Global mainCalls(100, 45);
    //! ----- (1) -----
    //* Primera acción inicialización (logo)
    mainCalls.Initializer();

    //! ----- (2) -----
    //* Segunda acción: Leer en que estado está el jugador (es decir, primera ejecución, o en algun punto x del juego)
     mainCalls.ReadPlayerStatusPoint();

    //! ----- (3) -----
    //*Tercera acción: mostrar el storyboard
     mainCalls.SetStoryBoard();

    //! ----- (4) -----
    //*Cuarta accion: Mostrar la pantalla de Inicio
    mainCalls.ShowInitialMenu(L"Menu");// Este método llama automáticamente a ShowSecondMenu() si el jugador elige "Jugar"

    //! ----- (5) -----
    //*Quinta accion: Mostrar meu de niveles
    // mainCalls.ShowSecondMenu();

    //! ----- (6) -----
    //* Sexta acción, iniciar el hilo de juegos
    mainCalls.StartGame();

    //* El juego iniciará el thread correspondiente al estado del usuario
    std::cin.get(); // Espera a que el usuario presione Enter
    return 0;
};

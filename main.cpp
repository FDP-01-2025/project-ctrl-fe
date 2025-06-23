#include <windows.h>
#include "./global.h"

// TODO_________________________________________ MAIN _______________________________________TODO
//* Objetivo: Estructura del orden de ejecución.

int main()
{
    Global mainCalls(110, 30, 110, 30);
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
    mainCalls.ShowInitialMenu(L"Menu"); // Este método llama automáticamente a ShowSecondMenu() si el jugador elige "Jugar"

    //! ----- (5) -----
    //*Quinta accion: Mostrar meu de niveles
    // mainCalls.ShowSecondMenu();
    
    //! ----- (6) -----
    //* Sexta acción, iniciar el hilo de juegos
    mainCalls.StartGame();
    return 0;
};

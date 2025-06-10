#pragma once

//* Todos los headers
#include "./src/utils/settings/console.h"
#include "./src/utils/settings/maping.h"
#include "./src/utils/threads/main/main.h"

class Global
{
public:
    //* Valores iniciales
    Global(int w = 50, int h = 30, int mW = 50, int mH = 30)
        : consoleW(w), consoleH(h), mapW(mW), mapH(mH) {}

    void showInitialScreen(const wchar_t *title)
    {
        //* Configuraciones de la consola de inicio
        Console C(consoleW, consoleH); // Parametros que mando
        Map M;

        C.configConsole();        // Aplicar configuraciones
        C.setTitle(title);        // Aplicar titulo
        M.drawMap(0, mapW, mapH); // Dibujar el mapa "0", osea el inicial
    }

    void readPlayerStatusPoint()
    {
        // Leer el file (osea, la base de datos trucha)
        int status = 0; // 0 = primera ejecución status = el estado dependiendo de lo que se lea del file
        // Después de identificar el estado las acciones
        // Primero, si es la primera ejecución, entonces el "mainThread"
        // mainThread = El hilo de acciones de la primera ejecución
        // Idea: hacer el orden de las acciones por "Threads" ejemplo, bomberManThread mainThread etc

        switch (status)
        {
        case 0: // Osea primera ejecucion
            mainThread();
            break;
        case 1: // Digamos que es el primer checkPoint, el primer checkpoint,

            break;

        default:
            break;
        }
    }

protected:
    int consoleW;
    int consoleH;

    int mapW;
    int mapH;
};
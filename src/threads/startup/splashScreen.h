#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "../../utils/screen/multiColors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"

bool InitializerThread(int width, int height, Console consoleSettings, Utils utils)
{
    /*consoleSettings.SetConsoleFont(12, 18, L"Lucida console");
    consoleSettings.SetColor(FOREGROUND_BLUE       );
    utils.PrintCentered(L"Bienvenido a..");
    Sleep(1000);
    system("cls");
    consoleSettings.SetConsoleFont(20, 26, L"Lucida console");
    utils.PrintCentered(L"LA TORRE DE LEGUIM");
    Sleep(1000);
    consoleSettings.SetConsoleFont();*/
    return true;
}

/*
SetConsoleTextAttribute es una función de la API de Windows que cambia el color del texto y el fondo en la consola.
Se le pasa el handle de la salida de la consola y el color
*/
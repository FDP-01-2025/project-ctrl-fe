#pragma once

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"

using std::wstring;

// Obtiene el ancho actual de la consola
static int GetConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
}

// Centra un texto en un ancho dado
static wstring CenterText(const wstring &text, int width)
{
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    return wstring(padding, L' ') + text;
}

// Menú principal: Jugar / Continuar / Salir
// Devuelve 1..3
static int StartMenu(Console &consoleSettings, Utils &utils)
{
    consoleSettings.SetConsoleFont(16, 24, L"Lucida Console");

    const wstring options[] = { L"Jugar", L"Continuar", L"Salir" };
    const int totalOptions = 3;
    int currentOption = 0;

    while (true)
    {
        system("cls");
        utils.SetUtf8();
        int width = GetConsoleWidth();

        // Título
        std::wcout << WHITE_BRIGHT << L"\n\n"
                   << CenterText(L"TORRE DE LEGUIM ☻", width) << L"\n"
                   << GRAY << CenterText(L"------------------------", width) << L"\n\n";

        // Opciones
        for (int i = 0; i < totalOptions; ++i)
        {
            wstring line = (i == currentOption)
                ? L">> " + options[i] + L" <<"
                : L"   " + options[i] + L"   ";

            std::wcout << (i == currentOption ? GREEN : WHITE_BRIGHT)
                       << CenterText(line, width)
                       << RESET << L"\n";
        }

        int key = _getch();
        if (key == 224)  // flecha
        {
            key = _getch();
            if (key == 72)       // arriba
                currentOption = (currentOption - 1 + totalOptions) % totalOptions;
            else if (key == 80)  // abajo
                currentOption = (currentOption + 1) % totalOptions;
        }
        else if (key == 13)   // Enter
        {
            return currentOption + 1;
        }
    }
}

// Menú de confirmación de salida: Sí / No
// Devuelve true si elige "Sí"
static bool ConfirmExitMenu(Console &consoleSettings, Utils &utils)
{
    consoleSettings.SetConsoleFont(16, 24, L"Lucida Console");

    const wstring choices[] = { L"Sí", L"No" };
    const int totalChoices = 2;
    int option = 0;

    while (true)
    {
        system("cls");
        int width = GetConsoleWidth();
        utils.SetUtf8();

        // Pregunta
        std::wcout << WHITE_BRIGHT << L"\n\n"
                   << CenterText(L"¿Seguro que quieres salir?", width)
                   << L"\n\n";

        // Opciones
        for (int i = 0; i < totalChoices; ++i)
        {
            wstring line = (i == option)
                ? L">> " + choices[i] + L" <<"
                : L"   " + choices[i] + L"   ";

            std::wcout << (i == option ? GREEN : WHITE_BRIGHT)
                       << CenterText(line, width)
                       << RESET << L"\n";
        }

        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == 72)       // arriba
                option = (option - 1 + totalChoices) % totalChoices;
            else if (key == 80)  // abajo
                option = (option + 1) % totalChoices;
        }
        else if (key == 13)    // Enter
        {
            return (option == 0);
        }
    }
}

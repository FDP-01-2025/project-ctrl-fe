#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/screen/colors.h"
#include "utils\functions\utils.h"

int GetConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
}

wstring CenterText(const wstring &text, int width)
{
    int padding = (width - (int)text.length()) / 2;
    if (padding < 0)
        padding = 0;
    return wstring(padding, ' ') + text;
}

int StartMenu()
{
    Utils utils;
    wstring options[] = {L"Jugar", L"Continuar", L"Salir"};
    int currentOption = 0;
    int totalOptions = 3;

    while (true)
    {
        system("cls");
        utils.SetUtf8();
        int width = GetConsoleWidth();

        wcout << WHITE_BRIGHT << L"\n\n"
              << CenterText(L"TORRE DE LEUGIM ☻", width) << L"\n";
        wcout << GRAY << CenterText(L"------------------------", width) << L"\n\n";

        for (int i = 0; i < totalOptions; ++i)
        {
            wstring text = options[i];
            wstring line;

            if (i == currentOption)
            {
                // Flechas a ambos lados
                line = L">> " + text + L" <<";
            }
            else
            {
                // Mismo espacio pero con espacios en lugar de flechas
                line = L"   " + text + L"   ";
            }

            // Centrar toda la línea con flechas o espacios para que no se mueva
            wcout << (i == currentOption ? GREEN : WHITE_BRIGHT) << CenterText(line, width) << RESET << endl;
        }

        int key = _getch();

        if (key == 224)
        {
            key = _getch();
            if (key == 72)
                currentOption = (currentOption - 1 + totalOptions) % totalOptions;
            else if (key == 80)
                currentOption = (currentOption + 1) % totalOptions;
        }
        else if (key == 13)
        {
            return currentOption + 1;
        }
    }

    return 0;
}

// Funcion para salir si el usuario quiere
bool ConfirmExitMenu()
{
    Utils utils;
    int option = 0;
    const wstring choices[] = {L"Sí", L"No"};
    int totalChoices = 2;

    while (true)
    {
        system("cls");
        int width = GetConsoleWidth();
        utils.SetUtf8();

        wcout << WHITE_BRIGHT << "\n\n"
              << CenterText(L"¿Seguro que quieres salir?", width) << "\n\n";

        for (int i = 0; i < totalChoices; ++i)
        {
            wstring line = (i == option) ? L">> " + choices[i] + L" <<" : L"   " + choices[i] + L"   ";
            wcout << (i == option ? GREEN : WHITE_BRIGHT) << CenterText(line, width) << RESET << endl;
        }

        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == 72) // ↑
                option = (option - 1 + totalChoices) % totalChoices;
            else if (key == 80) // ↓
                option = (option + 1) % totalChoices;
        }
        else if (key == 13)
        {
            return option == 0; // Devuelve true si eligió "Sí"
        }
    }
}

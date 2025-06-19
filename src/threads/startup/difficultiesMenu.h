#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/screen/colors.h"
#include "utils\functions\utils.h"

int StartSecondMenu()
{
    Utils utils;

    wstring difficulties[] = {L"Normal", L"Media", L"Difícil"};
    int currentOption = 0;
    int totalOptions = 3;

    const int consoleWidth = 100;

    while (true)
    {
        system("cls");
        utils.SetUtf8();

        // Encabezado centrado horizontalmente
        wstring title = L"ELIGE LA DIFICULTAD";
        int paddingTitle = (consoleWidth - title.length()) / 2;
        wcout << WHITE_BRIGHT << L"\n\n" << wstring(paddingTitle, ' ') << title << endl;

        // Línea separadora centrada
        wstring separator = L"------------------------";
        int paddingSeparator = (consoleWidth - separator.length()) / 2;
        wcout << GRAY << wstring(paddingSeparator, ' ') << separator << L"\n\n";

        // Mostrar opciones centradas
        for (int i = 0; i < totalOptions; ++i)
        {
            wstring optionText = difficulties[i];
            wstring optionLine;

            if (i == currentOption)
            {
                // Flechas a ambos lados
                optionLine = L">> " + optionText + L" <<";
            }
            else
            {
                // Igual largo, pero con espacios en lugar de flechas para no mover texto
                int arrowsLength = 6; // ">> " + " <<"
                // Espacios a cada lado para mantener longitud igual
                optionLine = wstring(arrowsLength / 2, ' ') + optionText + wstring(arrowsLength / 2, ' ');
            }

            int padding = (consoleWidth - optionLine.length()) / 2;

            if (i == currentOption)
                wcout << GREEN << wstring(padding, ' ') << optionLine << RESET << endl;
            else
                wcout << WHITE_BRIGHT << wstring(padding, ' ') << optionLine << RESET << endl;
        }

        // Captura de teclas
        int key = _getch();

        if (key == 224) // Flechas
        {
            key = _getch();
            if (key == 72) // ↑
                currentOption = (currentOption - 1 + totalOptions) % totalOptions;
            else if (key == 80) // ↓
                currentOption = (currentOption + 1) % totalOptions;
        }
        else if (key == 13) // ENTER
        {
            return currentOption + 1;
        }
    }
}

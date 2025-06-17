#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/screen/colors.h"

int StartSecondMenu()
{
    string difficulties[] = {"Normal", "Media", "Difícil"};
    int currentOption = 0;
    int totalOptions = 3;

    const int consoleWidth = 100;

    while (true)
    {
        system("cls");

        // Encabezado centrado horizontalmente
        string title = "ELIGE LA DIFICULTAD";
        int paddingTitle = (consoleWidth - title.length()) / 2;
        cout << WHITE_BRIGHT << "\n\n" << string(paddingTitle, ' ') << title << endl;

        // Línea separadora centrada
        string separator = "------------------------";
        int paddingSeparator = (consoleWidth - separator.length()) / 2;
        cout << GRAY << string(paddingSeparator, ' ') << separator << "\n\n";

        // Mostrar opciones centradas
        for (int i = 0; i < totalOptions; ++i)
        {
            string optionText = difficulties[i];
            string optionLine;

            if (i == currentOption)
            {
                // Flechas a ambos lados
                optionLine = ">> " + optionText + " <<";
            }
            else
            {
                // Igual largo, pero con espacios en lugar de flechas para no mover texto
                int arrowsLength = 6; // ">> " + " <<"
                // Espacios a cada lado para mantener longitud igual
                optionLine = string(arrowsLength / 2, ' ') + optionText + string(arrowsLength / 2, ' ');
            }

            int padding = (consoleWidth - optionLine.length()) / 2;

            if (i == currentOption)
                cout << GREEN << string(padding, ' ') << optionLine << RESET << endl;
            else
                cout << WHITE_BRIGHT << string(padding, ' ') << optionLine << RESET << endl;
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

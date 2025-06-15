#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/settings/colors.h"

int GetConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
}

string CenterText(const string &text, int width)
{
    int padding = (width - (int)text.length()) / 2;
    if (padding < 0)
        padding = 0;
    return string(padding, ' ') + text;
}

int StartMenu()
{
    string options[] = {"Jugar", "Continuar", "Salir"};
    int currentOption = 0;
    int totalOptions = 3;

    while (true)
    {
        system("cls");
        int width = GetConsoleWidth();

        cout << WHITE_BRIGHT << "\n\n"
             << CenterText("TORRE DE LEUGIM", width) << "\n";
        cout << GRAY << CenterText("------------------------", width) << "\n\n";

        for (int i = 0; i < totalOptions; ++i)
        {
            string text = options[i];
            string line;

            if (i == currentOption)
            {
                // Flechas a ambos lados
                line = ">> " + text + " <<";
            }
            else
            {
                // Mismo espacio pero con espacios en lugar de flechas
                line = "   " + text + "   ";
            }

            // Centrar toda la línea con flechas o espacios para que no se mueva
            cout << (i == currentOption ? GREEN : WHITE_BRIGHT) << CenterText(line, width) << RESET << endl;
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
        int option = 0;
        const string choices[] = {"Sí", "No"};
        int totalChoices = 2;

        while (true)
        {
            system("cls");
            int width = GetConsoleWidth();

            cout << WHITE_BRIGHT << "\n\n"
                 << CenterText("¿Seguro que quieres salir?", width) << "\n\n";

            for (int i = 0; i < totalChoices; ++i)
            {
                string line = (i == option) ? ">> " + choices[i] + " <<" : "   " + choices[i] + "   ";
                cout << (i == option ? GREEN : WHITE_BRIGHT) << CenterText(line, width) << RESET << endl;
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

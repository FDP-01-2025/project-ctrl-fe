#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/settings/colors.h" 

int StartMenu()
{
    string options[] = {"Jugar", "Continuar", "Salir"};
    int currentOption = 0;
    int totalOptions = 3;

    while (true)
    {
        system("cls");

        cout << WHITE_BRIGHT << "\n\n\t\tTORRE DE LEUGIM\n";
        cout << GRAY << "\t\t------------------------\n\n";

        for (int i = 0; i < totalOptions; ++i)
        {
            if (i == currentOption)
                cout << GREEN << "\t\t>> " << options[i] << " <<" << RESET << endl;
            else
                cout << WHITE_BRIGHT << "\t\t   " << options[i] << RESET << endl;
        }

        int key = _getch();

        if (key == 224) // flecha
        {
            key = _getch();
            if (key == 72) // ↑
                currentOption = (currentOption - 1 + totalOptions) % totalOptions;
            else if (key == 80) // ↓
                currentOption = (currentOption + 1) % totalOptions;
        }
        else if (key == 13) // ENTER
        {
            return currentOption + 1; // 1 = jugar, 2 = continuar, 3 = salir
        }
    }

    return 0;
}

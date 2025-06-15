#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "utils/settings/colors.h"
int StartSecondMenu()
{
    string difficulties[] = {"Normal", "Media", "Difícil"};
    int currentOption = 0;
    int totalOptions = 3;

    while (true)
    {
        system("cls");

        cout << WHITE_BRIGHT << "\n\n\t\tELIGE LA DIFICULTAD\n";
        cout << GRAY << "\t\t------------------------\n\n";

        for (int i = 0; i < totalOptions; ++i)
        {
            if (i == currentOption)
                cout << GREEN << "\t\t>> " << difficulties[i] << " <<" << RESET << endl;
            else
                cout << WHITE_BRIGHT << "\t\t   " << difficulties[i] << RESET << endl;
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
            return currentOption + 1; // 1 = normal, 2 = media, 3 = difícil
        }
    }
}


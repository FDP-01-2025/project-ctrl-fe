#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

#include "utils/screen/colors.h"
#include "utils/functions/utils.h"

int StartSecondMenu(Console& consoleSettings, Utils& utils)
{
    wstring difficulties[] = {L"Easy", L"Normal", L"Hard"};
    int currentOption = 0;
    int totalOptions = 3;

    // Función para obtener el ancho real de la consola
    auto GetConsoleWidth = []() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    };

    while (true)
    {
        system("cls");
        utils.SetUtf8();

        int consoleWidth = GetConsoleWidth();

        // Encabezado centrado
        wstring title = L"CHOOSE THE DIFFICULTY";
        int paddingTitle = (consoleWidth - title.length()) / 2;
        wcout << WHITE_BRIGHT << L"\n\n" << wstring(paddingTitle, ' ') << title << endl;

        // Separador centrado
        wstring separator = L"------------------------";
        int paddingSeparator = (consoleWidth - separator.length()) / 2;
        wcout << GRAY << wstring(paddingSeparator, ' ') << separator << L"\n\n";

        // Opciones centradas
        for (int i = 0; i < totalOptions; ++i)
        {
            wstring optionText = difficulties[i];
            wstring optionLine;

            if (i == currentOption)
            {
                optionLine = L">> " + optionText + L" <<";
            }
            else
            {
                int arrowsLength = 6;
                optionLine = wstring(arrowsLength / 2, ' ') + optionText + wstring(arrowsLength / 2, ' ');
            }

            int padding = (consoleWidth - optionLine.length()) / 2;

            wcout << (i == currentOption ? GREEN : WHITE_BRIGHT)
                  << wstring(padding, ' ') << optionLine << RESET << endl;
        }

        // Captura de teclas
        int key = _getch();

        if (key == 224)
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

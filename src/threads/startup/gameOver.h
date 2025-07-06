#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include "utils/screen/colors.h"
#include "utils/functions/utils.h"

class GameOverScreen {
public:
    // Devuelve true si el jugador elige “Reiniciar”, false si elige “Salir”.
    static bool Show(Utils &utils) {
        utils.SetUtf8();
        int choice = 0;
        const wchar_t* options[] = { L"Restart", L"Exit" };
        const int nOptions = 2;

        while (true) {
            system("cls");
            // Título
            std::wstring title = L"GAME OVER";
            int w = utils.GetConsoleWidth();
            int x = (w - title.size())/2;
            utils.MoveCursor(x, 5);
            std::wcout << RED_BRIGHT << title << RESET << L"\n\n";

            // Opciones
            for (int i = 0; i < nOptions; ++i) {
                x = (w - wcslen(options[i]))/2;
                utils.MoveCursor(x, 8 + i*2);
                if (i == choice) std::wcout << GREEN_BRIGHT << L"> ";
                else            std::wcout << L"  ";
                std::wcout << options[i] << RESET << L"\n";
            }

            // Leer flechas y Enter
            int c = _getch();
            if (c == 0 || c == 0xE0) {
                c = _getch();
                if      (c == 72) choice = (choice + nOptions - 1) % nOptions; // Up
                else if (c == 80) choice = (choice + 1) % nOptions;            // Down
            }
            else if (c == '\r') {
                return (choice == 0); // Restart==0 → true; Exit==1 → false
            }
        }
    }
};

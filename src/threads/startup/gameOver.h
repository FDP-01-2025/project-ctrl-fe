#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include "utils/screen/colors.h"
#include "utils/functions/utils.h"

class GameOverScreen {
public:
    static bool Show(Utils &utils) {
        utils.SetUtf8();
        int choice = 0;
        const wchar_t* options[] = { L"REINICIAR", L"SALIR" };
        const int nOptions = 2;

        const std::wstring gameOverArt[] = {
            L"  ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ ",
            L" ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗",
            L" ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝",
            L" ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔═══╝ ",
            L" ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║     ",
            L"  ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝     "
        };

        const int artHeight = sizeof(gameOverArt) / sizeof(gameOverArt[0]);

        while (true) {
            system("cls");
            int width = utils.GetConsoleWidth();

            // Mostrar arte de "GAME OVER"
            for (int i = 0; i < artHeight; ++i) {
                std::wstring line = gameOverArt[i];
                int padding = (width - line.size()) / 2;
                std::wcout << std::wstring(padding, L' ') << RED_BRIGHT << line << RESET << L"\n";
            }

            std::wcout << L"\n\n";

            // Mostrar opciones
            for (int i = 0; i < nOptions; ++i) {
                std::wstring text = (i == choice)
                    ? L">> " + std::wstring(options[i]) + L" <<"
                    : L"   " + std::wstring(options[i]) + L"   ";

                int padding = (width - text.length()) / 2;

                std::wcout << (i == choice ? GREEN_BRIGHT : WHITE_BRIGHT)
                           << std::wstring(padding, L' ') << text << RESET << L"\n";
            }

            // Captura de tecla
            int key = _getch();
            if (key == 224 || key == 0) {
                key = _getch();
                if (key == 72) choice = (choice - 1 + nOptions) % nOptions; // Up
                else if (key == 80) choice = (choice + 1) % nOptions;       // Down
            } else if (key == 13) {
                return (choice == 0); // Reiniciar (true) o Salir (false)
            }
        }
    }
};

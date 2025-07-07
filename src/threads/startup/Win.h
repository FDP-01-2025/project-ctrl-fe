#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include "utils/screen/colors.h"
#include "utils/functions/utils.h"

class VictoryScreen {
public:
    static bool Show(Utils &utils) {
        utils.SetUtf8();
        int choice = 0;
        const wchar_t* options[] = { L"RESTART", L"EXIT" };
        const int nOptions = 2;

        const std::wstring story[] = {
            L"After countless trials and fearless courage,",
            L"the brave princess finally reached the dark tower,",
            L"defeated the evil sorcerer, and rescued her beloved prince.",
            L"A new era of peace and love begins..."
        };

        const std::wstring heart = L"   ♥   ";
        const std::wstring icons = L" (இ)   (O) ";

        const std::wstring victoryArt[] = {
            L"__     ______  _    _   __          ___    _ ",
            L"\\ \\   / / __ \\| |  | |  \\ \\        / / |  | |",
            L" \\ \\_/ / |  | | |  | |   \\ \\  /\\  / /| |  | |",
            L"  \\   /| |  | | |  | |    \\ \\/  \\/ / | |  | |",
            L"   | | | |__| | |__| |     \\  /\\  /  | |__| |",
            L"   |_|  \\____/ \\____/       \\/  \\/    \\____/ "
        };

        const int storyLines = sizeof(story) / sizeof(story[0]);
        const int artHeight = sizeof(victoryArt) / sizeof(victoryArt[0]);

        while (true) {
            system("cls");
            int width = utils.GetConsoleWidth();
            int height = utils.GetConsoleHeight();

            int totalLines = storyLines + 4 + artHeight + nOptions;
            int verticalPadding = (height - totalLines) / 2;

            std::wcout << std::wstring(verticalPadding, L'\n');

            // Mostrar historia
            for (const auto& line : story) {
                int padding = (width - line.length()) / 2;
                std::wcout << std::wstring(padding, L' ') << YELLOW_BRIGHT << line << RESET << L"\n";
            }

            std::wcout << L"\n";

            // Mostrar íconos y corazón
            int heartPadding = (width - heart.length()) / 2;
            std::wcout << std::wstring(heartPadding, L' ') << RED_BRIGHT << heart << RESET << L"\n";

            int iconsPadding = (width - icons.length()) / 2;
            std::wcout << std::wstring(iconsPadding, L' ') << WHITE_BRIGHT << icons << RESET << L"\n";

            std::wcout << L"\n";

            // Mostrar "YOU WON!" en grande
            for (const auto& line : victoryArt) {
                int padding = (width - line.length()) / 2;
                std::wcout << std::wstring(padding, L' ') << GREEN_BRIGHT << line << RESET << L"\n";
            }

            std::wcout << L"\n";

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
                return (choice == 0); // Restart (true) or Exit (false)
            }
        }
    }
};

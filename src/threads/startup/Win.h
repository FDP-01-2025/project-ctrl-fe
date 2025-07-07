#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <thread>
#include "utils/screen/colors.h"
#include "utils/functions/utils.h"
#include "core/engine/tool/dialogues.h" // Asegúrate de tener DrawDialogue y ClearDialogue aquí

class VictoryScreen
{
public:
    static bool Show(Utils &utils)
    {
        utils.SetUtf8();
        system("cls");
        int choice = 0;
        const wchar_t *options[] = {L"RESTART", L"EXIT"};
        const int nOptions = 2;

        const std::wstring story[] = {
            L"After countless trials and fearless courage,",
            L"the brave princess finally reached the dark tower,",
            L"defeated the evil sorcerer, and rescued her beloved prince.",
            L"A new era of peace and love begins..."};

        const std::wstring heart = L"   ♥   ";
        const std::wstring icons = L" (இ)   (O) ";

        // Arte ASCII estilo bloques cuadrados para YOU WON
        const std::wstring victoryArt[] = {
            L"██    ██  ██████  ██    ██     ██     ██  ██████  ███    ██",
            L" ██  ██  ██    ██ ██    ██     ██     ██ ██    ██ ████   ██",
            L"  ████   ██    ██ ██    ██     ██  █  ██ ██    ██ ██ ██  ██",
            L"   ██    ██    ██ ██    ██     ██ ███ ██ ██    ██ ██  ██ ██",
            L"   ██    ██    ██ ██    ██     ███ ████ ██    ██ ██   ████",
            L"   ██     ██████   ██████       █   ██   ██████  ██    ███"};

        const int artHeight = sizeof(victoryArt) / sizeof(victoryArt[0]);
        const int width = utils.GetConsoleWidth();
        const int height = utils.GetConsoleHeight();

        // Parte 1: Historia (efecto de máquina de escribir)
        int startY = height / 2 - 10;
        for (int i = 0; i < sizeof(story) / sizeof(story[0]); ++i)
        {
            const std::wstring &line = story[i];
            for (size_t j = 0; j <= line.size(); ++j)
            {
                DrawDialogue(line.substr(0, j), 10, startY + (i * 4)); // cada diálogo ocupa 3 líneas + 1 espacio
                Sleep(30);
            }
            Sleep(500);
        }

        Sleep(800);

        // Parte 2: Mostrar íconos y corazón
        system("cls");
        int heartPadding = (width - heart.length()) / 2;
        int iconsPadding = (width - icons.length()) / 2;
        utils.MoveCursor(heartPadding, startY);
        std::wcout << RED_BRIGHT << heart << RESET;

        utils.MoveCursor(iconsPadding, startY + 2);
        std::wcout << WHITE_BRIGHT << icons << RESET;

        Sleep(1000);

        // Parte 3: Mostrar arte "YOU WON!" con efecto cuadro por cuadro
        for (int i = 0; i < artHeight; ++i)
        {
            int padding = (width - victoryArt[i].length()) / 2;
            utils.MoveCursor(padding, startY + 5 + i);
            std::wcout << GREEN_BRIGHT << victoryArt[i] << RESET;
            Sleep(200);
        }

        Sleep(800);

        // Parte 4: Menú interactivo
        while (true)
        {
            for (int i = 0; i < nOptions; ++i)
            {
                std::wstring text = (i == choice)
                                        ? L">> " + std::wstring(options[i]) + L" <<"
                                        : L"   " + std::wstring(options[i]) + L"   ";
                int padding = (width - text.length()) / 2;

                utils.MoveCursor(padding, startY + 12 + artHeight + i);
                std::wcout << (i == choice ? GREEN_BRIGHT : WHITE_BRIGHT)
                           << text << RESET << L"\n";
            }

            // Captura de tecla
            int key = _getch();
            if (key == 224 || key == 0)
            {
                key = _getch();
                if (key == 72)
                    choice = (choice - 1 + nOptions) % nOptions;
                else if (key == 80)
                    choice = (choice + 1) % nOptions;
            }
            else if (key == 13)
            {
                return (choice == 0); // true = restart, false = exit
            }

            // Limpieza para actualizar opciones
            for (int i = 0; i < nOptions; ++i)
            {
                utils.MoveCursor(0, startY + 12 + artHeight + i);
                std::wcout << std::wstring(width, L' ');
            }
        }
    }
};

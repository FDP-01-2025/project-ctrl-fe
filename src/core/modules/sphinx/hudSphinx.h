#ifndef HUD_SPHINX_H
#define HUD_SPHINX_H

#include <iostream>
#include <string>
#include <windows.h>

#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

class HUDSphinx
{
public:
    void SetCenteredOffset(int x) { hudOffsetX = x; }

    // statusCode: -1 = sin mensaje, 0 = incorrecto, 1 = correcto
    void Draw(Player player, int currentQuestion, int mapWidth, int statusCode)
    {
        int hudX = hudOffsetX + mapWidth + 3;
        int totalLines = 14;
        int consoleHeight = utils.GetConsoleHeight();
        int yStart = ((consoleHeight - totalLines) / 3) + 1;
        if (yStart < 0)
            yStart = 0;

        // Limpiar el área del HUD
        for (int i = 0; i < totalLines; ++i)
        {
            std::wcout << L"\033[" << (yStart + i) << L";" << hudX << L"H" << std::wstring(25, L' ');
        }

        int y = yStart;

        PrintAndAdvance(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        PrintAndAdvance(hudX, y, L"Question: " + ToWString(currentQuestion) + L"/5", CYAN);
        y++;

        PrintAndAdvance(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"Answer 3 correctly");
        PrintAndAdvance(hudX, y, L"Before running out of lives");
        y++;

        PrintAndAdvance(hudX, y, L"[Controls]:", WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"[WASD] Move");
        PrintAndAdvance(hudX, y, L"[1-3] Select option");

        // Mostrar mensaje si aplica
        if (statusCode != -1)
        {
            y++;
            // Borra la línea completa antes de imprimir el mensaje nuevo
            std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, L' ');

            std::wstring message = (statusCode == 1) ? L"✔ Correct!" : L"✖ Incorrect!";
            std::wstring color = (statusCode == 1) ? GREEN : RED;
            PrintAndAdvance(hudX, y, message, color);
        }

        std::wcout << RESET;
    }

private:
    int hudOffsetX = 0;
    Utils utils;

    void PrintAndAdvance(int x, int &y, const std::wstring &text, const std::wstring &color = GRAY_BRIGHT)
    {
        utils.PrintLine(x, y, text, color);
        ++y;
    }
};

#endif

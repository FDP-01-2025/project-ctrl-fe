#ifndef HUD_SPHINX_H
#define HUD_SPHINX_H

#include <iostream>
#include <string>
#include <windows.h>

#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

class HUDSphinx {
public:
    Utils utils;
    void SetCenteredOffset(int x) { hudOffsetX = x; }

    int GetWidth() const {
        return 25;
    }

    void Draw(Player player, int currentLevel, int mapWidth = 0) {
        int hudX = hudOffsetX + mapWidth + 3;
        int totalLines = 12;
        int consoleHeight = utils.GetConsoleHeight();
        int yStart = ((consoleHeight - totalLines) / 3) + 1;
        if (yStart < 0) yStart = 0;

        // Borra área del HUD
        for (int i = 0; i < totalLines; ++i) {
            std::wcout << L"\033[" << (yStart + i) << L";" << hudX << L"H" << std::wstring(25, L' ');
        }

        int y = yStart;

        PrintAndAdvance(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"Subroom - " + ToWString(currentLevel), GRAY);
        PrintAndAdvance(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        PrintAndAdvance(hudX, y, L"¤ - " + ToWString(player.GetBombs()), ORANGE);

        PrintAndAdvance(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"Responder 3 bien");
        PrintAndAdvance(hudX, y, L"Sin perder todas las vidas");

        PrintAndAdvance(hudX, y, L"Controls:", WHITE_BRIGHT);
        PrintAndAdvance(hudX, y, L"[WASD]: Mover");
        PrintAndAdvance(hudX, y, L"1-3: Elegir opción");

        std::wcout << RESET;
    }

private:
    int hudOffsetX = 0;
    void PrintAndAdvance(int x, int &y, const std::wstring &text, const std::wstring &color = GRAY_BRIGHT) {
        utils.PrintLine(x, y, text, color);
        ++y;
    }
};

#endif

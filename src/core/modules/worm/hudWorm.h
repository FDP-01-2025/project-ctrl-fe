#ifndef HUD_WORM_H
#define HUD_WORM_H

// Include required libraries
#include <iostream>
#include <string>
#include <windows.h>

// Include headers for used classes
#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

// Class to draw the HUD (HUD = info panel) for the Bomberman game
class HudWorm
{
public:
    Utils utils; // Instance of Utils class for utility functions
    void SetCenteredOffset(int x) { hudOffsetX = x; }
    // Get the width of the HUD
    // 'override' means this replaces a method from the base class
    int GetWidth() const
    {
        return 25;
    }

    // Function to draw the HUD on the screen
    // Takes the player, current level, and optional map width
    void Draw(Player player, int currentLevel, int mapWidth = 0, std::wstring keyCombination = L"A", int corrects = 0, int maxCorrects = 10, std::wstring userInput = L"none", int secondsLeft = 6)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 2; // X position of the HUD
        int totalLines = 12;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3);
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"type the key combination");
        utils.PrintLine(hudX, y, keyCombination, RED);
        y++;

        utils.PrintLine(hudX, y, L"[Your enter]:", WHITE_BRIGHT);
        std::wstring paddedInput = userInput + std::wstring(10 - userInput.length(), L' ');
        utils.PrintLine(hudX, y, paddedInput, GREEN);
        y++;

        utils.PrintLine(hudX, y, L"[Correctas]:", WHITE_BRIGHT);
        std::wstring correct = ToWString(corrects) + L"/" + ToWString(maxCorrects);
        utils.PrintLine(hudX, y, correct, YELLOW_BRIGHT);

        utils.PrintLine(hudX, y, L"[Tiempo restante]:", WHITE_BRIGHT);
        std::wstring time = ToWString(secondsLeft) + L"s";
        utils.PrintLine(hudX, y, time, CYAN);
        y++;

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

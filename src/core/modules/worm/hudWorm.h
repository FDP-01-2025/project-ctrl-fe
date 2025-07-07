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

// Class to draw the HUD (Heads-Up Display) for the Worm game
class HudWorm
{
public:
    Utils utils; // Instance of the Utils class for helper functions

    // Set an optional horizontal offset for centering the HUD
    void SetCenteredOffset(int x) { hudOffsetX = x; }

    // Get the fixed width of the HUD
    int GetWidth() const
    {
        return 25;
    }

    // Function to draw the HUD on the screen
    // Displays player lives, level info, target combo, user input, progress and time
    void Draw(Player player, int currentLevel, int mapWidth = 0, std::wstring keyCombination = L"A", int corrects = 0, int maxCorrects = 10, std::wstring userInput = L"none", int secondsLeft = 6)
    {
        utils.ClearScreen();                  // Clear screen before rendering the HUD
        int hudX = hudOffsetX + mapWidth + 2; // X coordinate for the HUD
        int totalLines = 12;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3);
        if (y < 0)
            y = 0;

        // Player lives
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Objective: Show the target key sequence
        utils.PrintLine(hudX, y, L"[Objective]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"type the");
        utils.PrintLine(hudX, y, L"key combination");
        utils.PrintLine(hudX, y, keyCombination, RED);
        y++;

        // Player's current input
        utils.PrintLine(hudX, y, L"[Your input]:", WHITE_BRIGHT);
        std::wstring paddedInput = userInput + std::wstring(10 - userInput.length(), L' ');
        utils.PrintLine(hudX, y, paddedInput, GREEN);
        y++;

        // Number of correct inputs so far
        utils.PrintLine(hudX, y, L"[Correct]:", WHITE_BRIGHT);
        std::wstring correct = ToWString(corrects) + L"/" + ToWString(maxCorrects);
        utils.PrintLine(hudX, y, correct, YELLOW_BRIGHT);
        y++;

        // Time left to complete the current task
        utils.PrintLine(hudX, y, L"[Time left]:", WHITE_BRIGHT);
        std::wstring time = ToWString(secondsLeft) + L"s";
        utils.PrintLine(hudX, y, time, CYAN);
        y++;

        std::wcout << RESET; // Reset console text color
    }

private:
    int hudOffsetX = 0; // Horizontal offset for positioning the HUD
};

#endif

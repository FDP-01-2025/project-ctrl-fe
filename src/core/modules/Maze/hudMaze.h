#ifndef HUD_MAZE_H
#define HUD_MAZE_H

// Include required libraries
#include <iostream>
#include <string>
#include <windows.h>

// Include headers for used classessD
#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

// Class to draw the HUD (HUD = info panel) for the Bomberman game
class HUDMaze
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
    void Draw(Player player, bool hasKey, int openedBoxes, int mapWidth = 0)
    {
        utils.ClearScreen();                   // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 10; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3) + 1;
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Find the door");
        utils.PrintLine(hudX, y, L"Find the key");
        utils.PrintLine(hudX, y, L"You can check all the boxes");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[WASD]: Move");
        utils.PrintLine(hudX, y, L"[Q]: Open the boxes");
        y++;
        // Información de cajas y llave
        utils.PrintLine(hudX, y, L"Open boxex: " + ToWString(openedBoxes), CYAN);
        utils.PrintLine(hudX, y, L"Key: " + std::wstring(hasKey ? L"Yes" : L"No"), hasKey ? GREEN : RED);
        y++;

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

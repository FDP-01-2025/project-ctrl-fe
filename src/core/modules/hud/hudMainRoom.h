#ifndef HUD_MAIN_H
#define HUD_MAIN_H

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
class HudMain
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
    void Draw(Player player, int currentLevel, int mapWidth = 0)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 2; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3);
        if (y < 0)
            y = 0;

        // Game objective
        utils.PrintLine(hudX, y, L" |With every", GRAY);
        utils.PrintLine(hudX, y, L" |trial you", GRAY);
        utils.PrintLine(hudX, y, L" |overcome", GRAY);
        utils.PrintLine(hudX, y, L" |three doors", GRAY);
        utils.PrintLine(hudX, y, L" |arise. One", GRAY);
        utils.PrintLine(hudX, y, L" |by one", GRAY);
        utils.PrintLine(hudX, y, L" |Cross them", GRAY);
        utils.PrintLine(hudX, y, L" |all don’t", GRAY);
        utils.PrintLine(hudX, y, L" |lose your", GRAY);
        utils.PrintLine(hudX, y, L" |way and", GRAY);
        utils.PrintLine(hudX, y, L" |reach your", GRAY);
        utils.PrintLine(hudX, y, L" |love at", GRAY);
        utils.PrintLine(hudX, y, L" |break of", GRAY);
        utils.PrintLine(hudX, y, L" |day", GRAY);

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

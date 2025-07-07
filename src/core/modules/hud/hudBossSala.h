#ifndef HUD_BOSSSALA_H
#define HUD_BOSSSALA_H

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
class HudBossSala
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
        utils.PrintLine(hudX, y, L" |You're close", GRAY);
        utils.PrintLine(hudX, y, L" |to the end,", GRAY);
        utils.PrintLine(hudX, y, L" |your courage", GRAY);
        utils.PrintLine(hudX, y, L" |won't bend.", GRAY);
        utils.PrintLine(hudX, y, L" |Face the fear,", GRAY);
        utils.PrintLine(hudX, y, L" |defy the pain,", GRAY);
        utils.PrintLine(hudX, y, L" |end this dark", GRAY);
        utils.PrintLine(hudX, y, L" |and deadly game.", GRAY);

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

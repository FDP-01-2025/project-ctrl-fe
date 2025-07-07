#ifndef HUD_WAY_H
#define HUD_WAY_H

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
class HudWay
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
        utils.PrintLine(hudX, y, L" |To find", GRAY);
        utils.PrintLine(hudX, y, L" |your love", GRAY);
        utils.PrintLine(hudX, y, L" |once more", GRAY);
        utils.PrintLine(hudX, y, L" |this hidden", GRAY);
        utils.PrintLine(hudX, y, L" |path you", GRAY);
        utils.PrintLine(hudX, y, L" |must explore", GRAY);
        utils.PrintLine(hudX, y, L" |Beyond doors", GRAY);
        utils.PrintLine(hudX, y, L" |with trials", GRAY);
        utils.PrintLine(hudX, y, L" |lie faces", GRAY);
        utils.PrintLine(hudX, y, L" |them all", GRAY);
        utils.PrintLine(hudX, y, L" |or say", GRAY);
        utils.PrintLine(hudX, y, L" |goodbye", GRAY);
        y++;

        // Controls
        utils.PrintLine(hudX, y, L" |Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L" |[WASD]: Move");

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

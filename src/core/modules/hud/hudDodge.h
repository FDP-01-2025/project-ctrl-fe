#ifndef HUD_DODGE_H
#define HUD_DODGE_H

// Include required libraries
#include <iostream>
#include <string>
#include <windows.h>

// Include headers for used classes
#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

// Class to draw the HUD (HUD = info panel)
class HUDDodge
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
    void DrawDodgeEvent(Player player, int currentLevel, int mapWidth, int count)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 3; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3) + 5;
        if (y < 0)
            y = 0;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[WASD]: Move");
        utils.PrintLine(hudX, y, L"Avoid the projectiles!", RED);
        utils.PrintLine(hudX, y, L"Then the door will be opened", YELLOW_BRIGHT);
        utils.PrintLine(hudX, y, L"Projectiles: " + ToWString(count) + L" ", CYAN); // the limit
        std::wcout << RESET;                                                        // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

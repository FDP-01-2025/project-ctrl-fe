#ifndef HUD_ELEVATOR_H
#define HUD_ELEVATOR_H

// Include required libraries
#include <iostream>
#include <string>
#include <windows.h>

// Include headers for used classes
#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"
#include "utils/functions/toWstring.h"

// Class to draw the HUD (Heads-Up Display) for the Bomberman game
class HudElevator
{
public:
    Utils utils; // Instance of Utils class for utility functions
    
    // Set horizontal offset to center the HUD
    void SetCenteredOffset(int x) { hudOffsetX = x; }
    
    // Get the width of the HUD panel
    int GetWidth() const
    {
        return 25;
    }

    // Draw the HUD on screen
    // Parameters:
    //  - player: the player object to get stats from
    //  - currentLevel: the current game level (unused here)
    //  - mapWidth: width of the map to position HUD correctly
    //  - corrects: number of correct answers
    //  - secondsLeft: time left in seconds
    //  - maxCorrects: maximum correct answers required
    void Draw(Player player, int currentLevel, int mapWidth = 0, int corrects = 0, int secondsLeft = 6, int maxCorrects = 3)
    {
        utils.ClearScreen();                  // Clear screen before drawing HUD
        int hudX = hudOffsetX + mapWidth + 2; // X position for the HUD (right of map)
        int totalLines = 9;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3);
        if (y < 0)
            y = 0;

        // Display player lives with heart symbol
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Display correct answers count
        utils.PrintLine(hudX, y, L"[Correct Answers]:", WHITE_BRIGHT);
        std::wstring correct = ToWString(corrects) + L"/" + ToWString(maxCorrects);
        utils.PrintLine(hudX, y, correct, YELLOW_BRIGHT);
        y++;

        // Display time left
        utils.PrintLine(hudX, y, L"[Time Left]:", WHITE_BRIGHT);
        std::wstring time = ToWString(secondsLeft) + L"s";
        utils.PrintLine(hudX, y, time, CYAN);
        y++;

        std::wcout << RESET; // Reset text color to default
    }

private:
    int hudOffsetX = 0; // Horizontal offset for centering the HUD
};

#endif

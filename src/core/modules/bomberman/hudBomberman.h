#ifndef HUD_BOMBERMAN_H
#define HUD_BOMBERMAN_H

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
class HUDBomberman
{
public:

    Utils utils; // Instance of Utils class for utility functions
    
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
        utils.ClearScreen(); // Clear the screen before drawing the HUD
        int hudX = mapWidth + 3; // X position of the HUD
        int y = 3; // Y starting position

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(currentLevel), WHITE_BRIGHT);
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Lives - " + ToWString(player.GetLives()), RED);
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"B - " + ToWString(player.GetBombs()), ORANGE);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Find the door");
        utils.PrintLine(hudX, y, L"Destroy % with bombs");
        utils.PrintLine(hudX, y, L"Don't die by fire");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[WASD]: Move");
        utils.PrintLine(hudX, y, L"[B]: Place bomb");

        std::wcout << RESET; // Reset text color
    }
};

#endif 

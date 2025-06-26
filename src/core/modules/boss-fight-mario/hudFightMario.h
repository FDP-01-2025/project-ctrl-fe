#ifndef HUD_FIGHT_MARIO_H
#define HUD_FIGHT_MARIO_H

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
class HUDFightMario
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
    void Draw(Player player, int mapWidth = 0)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 3; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3) + 2;
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Defeat the boss");
        utils.PrintLine(hudX, y, L"To go to the next room");
        utils.PrintLine(hudX, y, L"Shoot and jump, avoid the boss");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[D]: Move to right");
        utils.PrintLine(hudX, y, L"[A]: Move to left");
        utils.PrintLine(hudX, y, L"[W]: Jump");
        utils.PrintLine(hudX, y, L"[K]: Shoot right");
        utils.PrintLine(hudX, y, L"[J]: Shoot left");

        std::wcout << RESET; // Reset text color
    }

    void DrawExit(Player player, int mapWidth = 0)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 3; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3) + 2;
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Go to the exit");
        utils.PrintLine(hudX, y, L"                                  ");
        utils.PrintLine(hudX, y, L"                                  ");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[D]: Move to right");
        utils.PrintLine(hudX, y, L"[A]: Move to left");
        utils.PrintLine(hudX, y, L"[W]: Jump");
        utils.PrintLine(hudX, y, L"[K]: Shoot right");
        utils.PrintLine(hudX, y, L"[J]: Shoot left");

        std::wcout << RESET; // Reset text color
    }

private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

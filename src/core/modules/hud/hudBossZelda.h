#ifndef HUD_BOSS_ZELDA_H
#define HUD_BOSS_ZELDA_H

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
class HUDBossZelda
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
    void DrawDefeat(Player player, int mapWidth = 0)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 3; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight() - 5;
        int y = ((consoleHeight - totalLines) / 3) + 4;
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"¤ - " + ToWString(player.GetBombs()), ORANGE);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Destroy the statue with the bombs");
        utils.PrintLine(hudX, y, L"Be careful with the shots");
        utils.PrintLine(hudX, y, L"Dodge, grab the bombs,");
        utils.PrintLine(hudX, y, L"place it and defeat it");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[WASD]: Move");
        utils.PrintLine(hudX, y, L"[B]: Place bomb ¤");

        std::wcout << RESET; // Reset text color
    }

    void DrawGoAway(Player player, int mapWidth = 0)
    {
        utils.ClearScreen();                  // Clear the screen before drawing the HUD
        int hudX = hudOffsetX + mapWidth + 3; // X position of the HUD
        int totalLines = 10;
        int consoleHeight = utils.GetConsoleHeight();
        int y = ((consoleHeight - totalLines) / 3) + 4;
        if (y < 0)
            y = 0;

        // Player information
        std::wcout << L"\033[" << y << L";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"Room - " + ToWString(player.GetRoom()), WHITE_BRIGHT);
        y++;

        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"♥ - " + ToWString(player.GetLives()), RED);
        std::wcout << L"\033[" << y << ";" << hudX << L"H" << std::wstring(25, ' ');
        utils.PrintLine(hudX, y, L"¤ - " + ToWString(player.GetBombs()), ORANGE);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, L"[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"Go to the exit" );
        utils.PrintLine(hudX, y, L"Be careful with the shots");
        y++;

        // Controls
        utils.PrintLine(hudX, y, L"Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, L"[WASD]: Move");
        utils.PrintLine(hudX, y, L"[B]: Place bomb ¤");

        std::wcout << RESET; // Reset text color
    }

    private:
    int hudOffsetX = 0; // Offset for centering the HUD horizontally
};

#endif

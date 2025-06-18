#ifndef HUD_BOMBERMAN_H
#define HUD_BOMBERMAN_H

// Include required libraries
#include <iostream>
#include <string>

// Include headers for used classes
#include "utils/screen/colors.h"
#include "utils/player/player.h"
#include "utils/functions/utils.h"


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
        std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' ');
        utils.PrintLine(hudX, y, "Room - " + std::to_string(currentLevel), WHITE_BRIGHT);
        y++;

        std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' ');
        utils.PrintLine(hudX, y, "Lives - " + std::to_string(player.GetLives()), RED);
        std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' ');
        utils.PrintLine(hudX, y, "B - " + std::to_string(player.GetBombs()), ORANGE);
        y++;

        // Game objective
        utils.PrintLine(hudX, y, "[Goal]:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, "Find the door");
        utils.PrintLine(hudX, y, "Destroy % with bombs");
        utils.PrintLine(hudX, y, "Don't die by fire");
        y++;

        // Controls
        utils.PrintLine(hudX, y, "Controls:", WHITE_BRIGHT);
        utils.PrintLine(hudX, y, "[WASD]: Move");
        utils.PrintLine(hudX, y, "[B]: Place bomb");

        std::cout << RESET; // Reset text color
    }
};

#endif 

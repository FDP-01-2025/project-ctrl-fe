#ifndef BOMB_RENDERER_H // Prevents this file from being included more than once
#define BOMB_RENDERER_H

// Includes necessary libraries
#include <iostream>

// Includes header files of the used classes
#include "utils/functions/utils.h"
#include "core/modules/bomberman/bomb.h"
#include "utils/screen/colors.h"

// Class that draws bombs on the screen
class BombRenderer
{
public:
    Utils utils; // Helper object to do things like moving the cursor (cursor = where the text appears)

    // Method to draw all the bombs on the screen
    // bombs[]: array of bombs to draw
    // count: how many bombs are in the array
    // offsetX and offsetY: optional position shift (offset = move the position)
    void Draw(Bomb bombs[], int count, int offsetX = 0, int offsetY = 0);
};

// Code for the Draw method
void BombRenderer::Draw(Bomb bombs[], int count, int offsetX, int offsetY)
{
    // Loop through all the bombs
    for (int i = 0; i < count; ++i)
    {
        int x = bombs[i].GetX(); // Get the bomb's X position
        int y = bombs[i].GetY(); // Get the bomb's Y position
        if (x >= 0 && y >= 0)    // Check if the bomb's position is valid (not negative)
        {
            utils.MoveCursor(offsetX + x, offsetY + y); // Move cursor to where the bomb should appear
            std::wcout << ORANGE << L"¤" << RESET;      // Print bomb as '0' in orange, then reset color
        }
    }
}

#endif

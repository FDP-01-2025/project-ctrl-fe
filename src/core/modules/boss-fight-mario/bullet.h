#ifndef BULLET_H
#define BULLET_H

// Include the utility class for screen operations
#include "utils/functions/utils.h"

// This class represents a bullet that can be shot, moved, and drawn on screen
class Bullet
{
public:
    // Bullet's current position
    int x, y;

    // Flag to check if the bullet is currently active (moving)
    bool isActive = false;

    // Direction of bullet movement: 1 = right, -1 = left
    int direction = 1;

    // This method activates the bullet and sets its initial position and direction
    void Shoot(int startX, int startY, int dir)
    {
        // Only shoot if the bullet is not already active
        if (!isActive)
        {
            direction = dir;           // Set movement direction
            x = startX + dir;          // Position bullet in front of shooter
            y = startY;                // Same vertical level as shooter
            isActive = true;           // Mark bullet as active
        }
    }

    // This method moves the bullet and checks for collisions
    void Move(Utils utils, int offsetX, int offsetY)
    {
        // If the bullet is not active, skip movement
        if (!isActive)
            return;

        // Clear the bullet's previous position from the screen
        utils.MoveCursor(x + offsetX, y + offsetY);
        std::wcout << L" ";

        // Move the bullet forward according to its direction
        x += direction;

        // Read the character at the new position (used to detect collision)
        wchar_t currentChar = utils.ReadCharAt(x + offsetX, y + offsetY);

        // If bullet hits a wall or an obstacle, deactivate it
        if (currentChar == L'#' || currentChar == L'=')
        {
            isActive = false;
            return;
        }

        // If bullet goes out of horizontal screen bounds, deactivate it
        if (x <= 1 || x >= 78)
        {
            isActive = false;
        }
    }

    // This method draws the bullet on the screen
    void Draw(Utils utils, int offsetX, int offsetY)
    {
        // If bullet is not active, there's nothing to draw
        if (!isActive)
            return;

        // Erase bullet's previous position
        utils.MoveCursor(x - direction, y);
        std::wcout << L" ";

        // Draw bullet at current position with directional symbol
        utils.MoveCursor(offsetX + x, offsetY + y);
        std::wcout << GREEN_BRIGHT << (direction == 1 ? L">" : L"<") << RESET;
    }

    // This method manually deactivates the bullet (useful when hitting enemies)
    void Deactivate()
    {
        isActive = false;
    }
};

#endif

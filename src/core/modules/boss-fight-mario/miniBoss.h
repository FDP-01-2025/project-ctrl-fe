#ifndef MINI_BOSS_H
#define MINI_BOSS_H

// Include necessary headers for utilities and player difficulty enum
#include "utils/functions/utils.h"
#include "utils/player/player.h"

// This class represents a moving mini boss enemy
class MiniBoss
{
public:
    // Current position of the mini boss
    int x = 30, y = 13;

    // Previous position used to erase the previous frame
    int previousX = 40, previousY = 19;

    // Health values: current and maximum
    int health = 50, maxHealth = 50;

    // Status flags
    bool isAlive = true;      // Indicates if the boss is still alive
    bool movingRight = true;  // Direction of movement

    // Movement speed control
    int speed = 6;            // The lower the value, the faster it moves
    int speedCounter = 0;     // Counter to control timing between moves

    // Initializes the boss stats based on selected difficulty
    void Init(Player::Difficulty dif)
    {
        switch (dif)
        {
        case Player::EASY:
            health = 25;
            maxHealth = 25;
            speed = 8; // Moves slower on easy
            break;
        case Player::NORMAL:
            health = 30;
            maxHealth = 30;
            speed = 6; // Default speed
            break;
        case Player::HARD:
            health = 60;
            maxHealth = 60;
            speed = 3; // Moves faster on hard
            break;
        }
    }

    // Controls boss movement between minX and maxX bounds
    void Move(int minX, int maxX)
    {
        // If boss is dead, do not move
        if (!isAlive)
            return;

        // Move only when speedCounter reaches speed
        if (speedCounter++ < speed)
            return;

        // Reset counter for next move
        speedCounter = 0;

        // Store previous position before moving
        previousX = x;
        previousY = y;

        // Move right until reaching maxX, then change direction
        if (movingRight)
        {
            if (x < maxX)
                x++;
            else
                movingRight = false;
        }
        else // Move left until reaching minX, then change direction
        {
            if (x > minX)
                x--;
            else
                movingRight = true;
        }
    }

    // Draws the mini boss at the new position and erases the previous one
    void Draw(Utils &utils, int offsetX, int offsetY)
    {
        // If the position changed, clear the previous character
        if (previousX != x || previousY != y)
        {
            utils.MoveCursor(offsetX + previousX, offsetY + previousY);
            std::wcout << L" ";
        }

        // Only draw if the boss is still alive
        if (isAlive)
        {
            utils.MoveCursor(offsetX + x, offsetY + y);
            std::wcout << RED << L"♛" << RESET; // Red crown character
        }

        // Update stored previous position
        previousX = x;
        previousY = y;
    }

    // Handles when the boss receives damage
    void TakeHit()
    {
        health--;           // Decrease health
        if (health <= 0)    // If health drops to 0 or less, mark as dead
        {
            isAlive = false;
        }
    }
};

#endif

#ifndef PLAYER_MARIO_H
#define PLAYER_MARIO_H

// Include utility functions and base player class
#include "utils/functions/utils.h"
#include "utils/player/player.h"

// This class represents a Mario-style player character
class PlayerMario
{
public:
    Player playerBase; // Instance of the base player class (used for lives, difficulty, etc.)
    int x = 5, y = 13; // Starting position near the left and just above the floor

    int fallCooldown = 0;          // Counter to control fall timing
    int previousX = 10, previousY = 19; // Stores previous position for clearing character
    bool isJumping = false;        // Flag to indicate if the player is in a jump
    int jumpCounter = 0;           // How many jump steps remain
    int jumpCooldown = 0;          // Time delay between jump frames
    int fallCounter = 0;           // (Unused in this version)
    int immunityFrames = 0;        // Temporary invincibility after getting hit
    int moveCooldown = 0;          // Prevents moving too fast continuously
    const int moveDelay = 2;       // Delay between consecutive moves

    // Draws the character and clears the previous drawing
    void Draw(Utils utils, int offsetX, int offsetY)
    {
        // Clean 3x3 area around the previous position
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int cleanX = previousX + dx;
                int cleanY = previousY + dy;
                // Only clear if inside visible area
                if (cleanX >= 1 && cleanX <= 78 && cleanY >= 1 && cleanY <= 19)
                {
                    utils.MoveCursor(offsetX + cleanX, offsetY + cleanY);
                    std::wcout << L" ";
                }
            }
        }

        // Draw character at current position if it's inside visible bounds
        if (x >= 1 && x <= 78 && y >= 1 && y <= 19)
        {
            utils.MoveCursor(offsetX + x, offsetY + y);
            std::wcout << PINK << L"இ" << RESET; // Use custom character and color
        }

        // Update previous position for next frame
        previousX = x;
        previousY = y;
    }

    // Reduces movement cooldown each frame
    void UpdateMovementCooldown()
    {
        if (moveCooldown > 0)
            moveCooldown--;
    }

    // Initiates a jump if not already jumping and player is on the ground
    void Jump()
    {
        if (!isJumping && y == 13)
        {
            isJumping = true;
            jumpCounter = 6;   // Total jump height
            jumpCooldown = 0;  // Reset delay counter
        }
    }

    // Moves the player left if not at the edge and cooldown allows
    void MoveLeft()
    {
        if (x > 1 && moveCooldown == 0)
        {
            previousX = x;
            previousY = y;
            x--;
            moveCooldown = moveDelay; // Prevents immediate next move
        }
    }

    // Moves the player right within limits and respecting cooldown
    void MoveRight()
    {
        if (x < 38 && moveCooldown == 0)
        {
            previousX = x;
            previousY = y;
            x++;
            moveCooldown = moveDelay;
        }
    }

    // Updates jump logic over time
    void UpdateJump()
    {
        if (isJumping)
        {
            // Slow down jump with cooldown
            if (jumpCooldown++ % 2 == 0)
                return;

            previousX = x;
            previousY = y;

            // Move up if jumpCounter > 0 and not at top limit
            if (jumpCounter > 0 && y > 1)
            {
                y--;
                jumpCounter--;
            }
            else // Start falling down after reaching top of jump
            {
                y++;
                // If back on the floor, end the jump
                if (y >= 13)
                {
                    y = 13;
                    isJumping = false;
                }
            }
        }
    }

    // Applies gravity if player is not on the ground and not jumping
    void Fall()
    {
        if (!isJumping && y < 13)
        {
            // Control fall speed
            if (fallCooldown++ < 2)
                return;
            fallCooldown = 0;

            previousX = x;
            previousY = y;
            y++;
            if (y > 13) // Clamp to ground
                y = 13;
        }
    }

    // Handles when the player takes damage
    void LoseLife()
    {
        // Only if not currently immune
        if (immunityFrames == 0)
        {
            playerBase.LoseLife();  // Reduce life from base class
            immunityFrames = 15;    // Activate temporary invincibility
        }
    }

    // Updates immunity countdown
    void UpdateImmunity()
    {
        if (immunityFrames > 0)
            immunityFrames--;
    }

    // Checks if player still has lives remaining
    bool IsAlive()
    {
        return playerBase.GetLives() > 0;
    }
};

#endif
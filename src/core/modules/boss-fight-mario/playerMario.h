#ifndef PLAYER_MARIO_H
#define PLAYER_MARIO_H

#include "utils/functions/utils.h"
#include "utils/player/player.h"

class PlayerMario
{
public:
    Player playerBase; // Instancia del jugador base
    int x = 5, y = 13; // más a la izquierda, justo encima del piso

    int fallCooldown = 0;
    int previousX = 10, previousY = 19;
    bool isJumping = false;
    int jumpCounter = 0;
    int jumpCooldown = 0;
    int fallCounter = 0;
    int immunityFrames = 0;
    int moveCooldown = 0;
    const int moveDelay = 2; // Cambia este valor para ajustar la lentitud

    void Draw(Utils utils, int offsetX, int offsetY)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                int cleanX = previousX + dx;
                int cleanY = previousY + dy;
                if (cleanX >= 1 && cleanX <= 78 && cleanY >= 1 && cleanY <= 19)
                {
                    utils.MoveCursor(offsetX + cleanX, offsetY + cleanY);
                    std::wcout << L" ";
                }
            }
        }

        if (x >= 1 && x <= 78 && y >= 1 && y <= 19)
        {
            utils.MoveCursor(offsetX + x, offsetY + y);
            std::wcout << PINK << L"இ" << RESET;
        }

        previousX = x;
        previousY = y;
    }

    void UpdateMovementCooldown()
    {
        if (moveCooldown > 0)
            moveCooldown--;
    }

    void Jump()
    {
        if (!isJumping && y == 13)
        {
            isJumping = true;
            jumpCounter = 6;
            jumpCooldown = 0;
        }
    }

    void MoveLeft()
    {
        if (x > 1 && moveCooldown == 0)
        {
            previousX = x;
            previousY = y;
            x--;
            moveCooldown = moveDelay;
        }
    }

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

    void UpdateJump()
    {
        if (isJumping)
        {
            if (jumpCooldown++ % 2 == 0)
                return;

            previousX = x;
            previousY = y;

            // Evita que salte hacia el muro superior
            if (jumpCounter > 0 && y > 1)
            {
                y--;
                jumpCounter--;
            }
            else
            {
                y++;
                if (y >= 13)
                {
                    y = 13;
                    isJumping = false;
                }
            }
        }
    }

    void Fall()
    {
        if (!isJumping && y < 13)
        {
            if (fallCooldown++ < 2)
                return;
            fallCooldown = 0;

            previousX = x;
            previousY = y;
            y++;
            if (y > 13)
                y = 13;
        }
    }

    void LoseLife()
    {
        if (immunityFrames == 0)
        {
            playerBase.LoseLife();
            immunityFrames = 15;
        }
    }

    void UpdateImmunity()
    {
        if (immunityFrames > 0)
            immunityFrames--;
    }

    bool IsAlive()
    {
        return playerBase.GetLives() > 0;
    }
};

#endif
#ifndef PLAYER_MARIO_H
#define PLAYER_MARIO_H

#include "utils/functions/utils.h"

class PlayerMario
{
public:
    int x = 10, y = 19;
    int fallCooldown = 0;
    int previousX = 10, previousY = 19;
    bool isJumping = false;
    int jumpCounter = 0;
    int jumpCooldown = 0;
    int fallCounter = 0;
    int lives = 3;
    int immunityFrames = 0;
    int moveCooldown = 0;
    const int moveDelay = 2; // Cambia este valor para ajustar la lentitud

    void Draw(Utils &utils)
    {
        // Limpia SIEMPRE la posición anterior
        if (previousX != x || previousY != y)
        {
            utils.MoveCursor(previousX, previousY);
            std::wcout << L" ";
        }

        // Dibuja nueva posición
        utils.MoveCursor(x, y);
        std::wcout << PINK << L"இ" << RESET;

        // Guarda posición actual como anterior para el siguiente frame
        previousX = x;
        previousY = y;
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
        if (x < 78 && moveCooldown == 0)
        {
            previousX = x;
            previousY = y;
            x++;
            moveCooldown = moveDelay;
        }
    }

    void UpdateMovementCooldown()
    {
        if (moveCooldown > 0)
            moveCooldown--;
    }

    void Jump()
    {
        if (!isJumping && y == 19)
        {
            isJumping = true;
            jumpCounter = 6;
            jumpCooldown = 0;
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

            if (jumpCounter > 0)
            {
                y--;
                jumpCounter--;
            }
            else
            {
                y++;
                if (y >= 19)
                {
                    y = 19;
                    isJumping = false;
                }
            }
        }
    }

    void Fall()
    {
        if (!isJumping && y < 19)
        {
            if (fallCooldown++ < 2)
                return; // Más lento al caer
            fallCooldown = 0;

            previousX = x;
            previousY = y;
            y++;
        }
    }

    void LoseLife()
    {
        if (immunityFrames == 0)
        {
            lives--;
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
        return lives > 0;
    }
};

#endif
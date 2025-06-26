#ifndef MINI_BOSS_H
#define MINI_BOSS_H

#include "utils/functions/utils.h"
#include "utils/player/player.h"

class MiniBoss
{

public:
    int x = 30, y = 13;
    int previousX = 40, previousY = 19;
    int health = 50;
    bool isAlive = true;
    bool movingRight = true;
    int speed = 6; // menor es más rápido
    int speedCounter = 0;

    void Init(Player::Difficulty dif)
    {
        switch (dif)
        {
        case Player::EASY:
            health = 25;
            speed = 8; // más lento
            break;
        case Player::NORMAL:
            health = 30;
            speed = 6;
            break;
        case Player::HARD:
            health = 60;
            speed = 3; // más rápido
            break;
        }
    }

    void Move(int minX, int maxX)
    {
        if (!isAlive)
            return;

        if (speedCounter++ < speed)
            return;
        speedCounter = 0;

        previousX = x;
        previousY = y;

        if (movingRight)
        {
            if (x < maxX)
                x++;
            else
                movingRight = false;
        }
        else
        {
            if (x > minX)
                x--;
            else
                movingRight = true;
        }
    }

    void Draw(Utils &utils, int offsetX, int offsetY)
    {
        if (previousX != x || previousY != y)
        {
            utils.MoveCursor(offsetX + previousX, offsetY + previousY);
            std::wcout << L" ";
        }

        if (isAlive)
        {
            utils.MoveCursor(offsetX + x, offsetY + y);
            std::wcout << RED << L"♛" << RESET;
        }

        previousX = x;
        previousY = y;
    }

    void TakeHit()
    {
        health--;
        if (health <= 0)
        {
            isAlive = false;
        }
    }
};

#endif

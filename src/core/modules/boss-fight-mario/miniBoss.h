#ifndef MINI_BOSS_H
#define MINI_BOSS_H

#include "utils/functions/utils.h"

class MiniBoss
{
public:
    int x = 40, y = 19;
    int previousX = 40, previousY = 19;
    int health = 50;
    bool isAlive = true;
    bool movingRight = true;

    void Move(int minX, int maxX)
    {
        if (!isAlive)
            return;

        static int moveCooldown = 0;
        if (moveCooldown++ < 6)
            return;
        moveCooldown = 0;

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

    void Draw(Utils utils)
    {
        if (!isAlive)
            return;
        utils.MoveCursor(previousX, previousY);
        std::wcout << L" ";
        utils.MoveCursor(x, y);
        std::wcout << RED << L"M" << RESET;
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
#endif // MINI_BOSS_H
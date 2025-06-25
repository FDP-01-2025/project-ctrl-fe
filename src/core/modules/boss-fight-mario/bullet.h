#ifndef BULLET_H
#define BULLET_H
#include "utils/functions/utils.h"

class Bullet {
public:
    int x, y;
    bool isActive = false;
    int direction = 1; // 1 = derecha, -1 = izquierda

    void Shoot(int startX, int startY, int dir) {
        if (!isActive) {
            direction = dir;
            x = startX + dir;
            y = startY;
            isActive = true;
        }
    }

    void Move() {
        if (!isActive) return;
        x += direction;
        if (x <= 1 || x >= 78) {
            isActive = false;
        }
    }

    void Draw(Utils &utils) {
        if (!isActive) return;
        utils.MoveCursor(x - direction, y);  // Borrar anterior
        std::wcout << L" ";
        utils.MoveCursor(x, y);
        std::wcout << GREEN_BRIGHT << (direction == 1 ? L">" : L"<") << RESET;
    }

    void Deactivate() {
        isActive = false;
    }
};

#endif

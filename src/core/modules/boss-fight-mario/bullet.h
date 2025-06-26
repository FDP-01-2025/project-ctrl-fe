#ifndef BULLET_H
#define BULLET_H
#include "utils/functions/utils.h"

class Bullet
{
public:
    int x, y;
    bool isActive = false;
    int direction = 1; // 1 = derecha, -1 = izquierda

    void Shoot(int startX, int startY, int dir)
    {
        if (!isActive)
        {
            direction = dir;
            x = startX + dir;
            y = startY;
            isActive = true;
        }
    }

    void Move(Utils utils, int offsetX, int offsetY)
    {
        if (!isActive)
            return;

        // Limpia posición anterior
        utils.MoveCursor(x + offsetX, y + offsetY);
        std::wcout << L" ";

        // Avanza
        x += direction;

        // Lee el carácter en la nueva posición
        wchar_t currentChar = utils.ReadCharAt(x + offsetX, y+ offsetY);

        // Si es muro, desactivar
        if (currentChar == L'#' || currentChar == L'=')
        {
            isActive = false;
            return;
        }

        // Si se sale del mapa
        if (x <= 1 || x >= 78)
        {
            isActive = false;
        }
    }

    void Draw(Utils utils, int offsetX, int offsetY)
    {
        if (!isActive)
            return;
        utils.MoveCursor(x - direction, y); // Borrar anterior
        std::wcout << L" ";
        utils.MoveCursor(offsetX + x, offsetY + y);
        std::wcout << GREEN_BRIGHT << (direction == 1 ? L">" : L"<") << RESET;
    }

    void Deactivate()
    {
        isActive = false;
    }
};

#endif

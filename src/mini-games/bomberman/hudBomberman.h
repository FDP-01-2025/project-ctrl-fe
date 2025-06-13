#ifndef HUD_BOMBERMAN_H
#define HUD_BOMBERMAN_H

#include "./src/utils/settings/hudBase.h"
#include "./src/utils/settings/colors.h"
#include "./src/utils/settings/utils.h"
#include <iostream>
#include <string>

class HUDBomberman : public HUDBase
{
public:
    // Obtener ancho del HUD donde el override es porque se reescribió el método de la clase base
    int GetWidth() const override
    {
        return 25;
    }

    // Funcion para dibujar el HUD en la consola
    // Recibe el jugador, el nivel actual y el ancho del mapa (opcional)
    void Draw(const Player &player, int currentLevel, int mapWidth = 0) override
    {
        int hudX = mapWidth + 3;
        int y = 3;

        // Información del jugador
        PrintLine(hudX, y, "Sala - " + std::to_string(currentLevel), WHITE_BRIGHT);
        y++;

        PrintLine(hudX, y, "Vida - " + std::to_string(player.GetLives()), RED);
        std::cout << "\033[" << y << ";" << hudX << "H" << std::string(25, ' ');
        PrintLine(hudX, y, "B - " + std::to_string(player.GetBombs()), ORANGE);
        y++;

        // Objetivo del juego
        PrintLine(hudX, y, "[Objetivo]:", WHITE_BRIGHT);
        PrintLine(hudX, y, "Encuentra la puerta");
        PrintLine(hudX, y, "Destruye % con bombas");
        PrintLine(hudX, y, "Evita morir con fuego");
        y++;

        // Controles
        PrintLine(hudX, y, "Controles:", WHITE_BRIGHT);
        PrintLine(hudX, y, "[WASD]: Moverse");
        PrintLine(hudX, y, "[B]: Bomba");

        std::cout << RESET;
    }

private:
    // Funcion para printar una línea de texto en una posición específica del HUD
    void PrintLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT) const
    {
        std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET;
        y++;
    }
};

#endif // HUD_BOMBERMAN_H

// HUD es un acrónimo de "Heads-Up Display", 
//que se refiere a una interfaz gráfica que 
//muestra información importante al jugador sin interrumpir 
//la experiencia de juego. En este caso, el 
//HUD muestra información sobre el jugador, como su vida, objetos dependiendo del nivel y objetivos del juego.

#ifndef HUD_BASE_H
#define HUD_BASE_H

#include "player.h"
#include "utils/functions/utils.h"

class HUDBase {
public:
    Utils utils; // Instancia de la clase Utils para operaciones comunes
    // Constructor por defecto
    // Esto significa que las clases derivadas deben implementar sus propios destructores
    virtual ~HUDBase() = default;
    //Obtener ancho del HUD donde se declaro con virtual porrque se pueda cambiar en las clases derivadas
    virtual int GetWidth() const = 0;
    // Se dibuja el HUD en la consola 
    virtual void Draw(const Player& player, int currentLevel, int mapWidth = 0) = 0;
};

#endif
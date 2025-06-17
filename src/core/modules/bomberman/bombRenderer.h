#ifndef BOMB_RENDERER_H // Directiva de preprocesador para evitar inclusiones múltiples
#define BOMB_RENDERER_H

//Include de librerías necesarias
#include <iostream> 

// Include de las cabeceras de las clases utilizadas
#include "utils/functions/utils.h" 
#include "core/modules/bomberman/bomb.h" 
#include "utils/screen/colors.h" 

// Clase responsable de dibujar las bombas en la consola
class BombRenderer
{
public:
    Utils utils; // Objeto utilitario para funciones como mover el cursor

    // Método para dibujar todas las bombas en pantalla
    // bombs[]: arreglo de bombas a dibujar
    // count: cantidad de bombas en el arreglo
    // offsetX y offsetY: desplazamiento opcional en la posición de dibujo
    void Draw(Bomb bombs[], int count, int offsetX = 0, int offsetY = 0);
};

// Implementación del método Draw
void BombRenderer::Draw(Bomb bombs[], int count, int offsetX, int offsetY)
{
    // Itera sobre todas las bombas
    for (int i = 0; i < count; ++i) 
    {
        int x = bombs[i].GetX(); // Obtiene la coordenada X de la bomba
        int y = bombs[i].GetY(); // Obtiene la coordenada Y de la bomba
        utils.MoveCursor(offsetX + x, offsetY + y); // Mueve el cursor a la posición correspondiente
        std::cout << ORANGE << "0" << RESET; // Dibuja la bomba como '0' en color naranja y luego reinicia el color
    }
}

#endif // Fin del include guard

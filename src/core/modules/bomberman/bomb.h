#ifndef BOMB_H
#define BOMB_H

#include <chrono>

class Bomb
{
public:
    // Declaracion del constructor por defecto y con coordenadas
    Bomb();
    Bomb(int x, int y);

    // Getters para las coordenadas
    int GetX() const;
    int GetY() const;
    // Funcion que verifica si la bomba ha explotado
    bool HasExploded() const;

private:
    // Declaracion de las coordenadas y el tiempo en que se colocó la bomba
    int x, y;
    std::chrono::steady_clock::time_point placedAt;
};

// Implementación de Bomb
// Constructor por defecto y con coordenadas
Bomb::Bomb() : x(0), y(0), placedAt(std::chrono::steady_clock::now()) {}
Bomb::Bomb(int x, int y) : x(x), y(y), placedAt(std::chrono::steady_clock::now()) {}

// Getters para las coordenadas
int Bomb::GetX() const { return x; }
int Bomb::GetY() const { return y; }

// Verifica si la bomba ha explotado (3 segundos después de ser colocada)
bool Bomb::HasExploded() const
{
    // Compara el tiempo actual con el tiempo en que se colocó la bomba
    // donde el steady_clock es más preciso que el system_clock
    // de ahi placedAt es el tiempo en que se colocó la bomba y el .count() convierte la duración a segundos
    // y se verifica si han pasado al menos 3 segundos
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - placedAt).count() >= 3;
}

#endif

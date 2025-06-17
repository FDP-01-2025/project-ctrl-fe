#ifndef PLAYER_H
#define PLAYER_H

#pragma once
#include <string>

class Map;

// Clase que representa al jugador en el juego
class Player
{
public:
    // Dificultad del juego
    enum Difficulty
    {
        EASY,
        NORMAL,
        HARD
    };

    // Getters
    int GetX() const;
    int GetY() const;
    int GetLives() const;
    int GetBombs() const;
    int GetMaxBombs() const;
    bool CanPlaceBomb() const;
    Difficulty GetDifficulty() const;
    // Obtiene si el control de la tecla B está activo
    bool IsControlBActive() const;

    // Setters
    // Establece la posición del jugador
    void SetPosition(int x, int y);
    // Establece la dificultad del juego
    void SetDifficulty(Difficulty dif);
    // SE pone si esta activa o desactiva el control de la tecla B
    void ActivateControlB(bool state);
    void SetLives(int lives);
    void SetBombs(int bombs);

    // Acciones del jugador
    // Intenta mover al jugador en base a un input y el mapa
    bool TryMove(char input, Map &map);
    // Movimiento básico si la casilla es caminable
    void Move(int dx, int dy, char nextTile);
    // Coloca una bomba si hay bombas disponibles
    void PlaceBomb();
    void LoseLife();
    void IncrementBombs();
    void IncrementLife();

    // Evitar copia y asignación
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

    // Método estático para obtener la instancia única
    static Player &GetInstance()
    {
        static Player instance; // instancia única (lazy initialization)
        return instance;
    }

private:
    int x, y;
    int lives;
    int bombsAvailable;
    int maxBombs = 25;
    Difficulty difficulty;
    bool controlB;

    // Constructor privado para evitar instancias externas
    Player() : x(0), y(0), lives(3), difficulty(EASY), controlB(false), bombsAvailable(1) {}
};

#include "maping.h"

int Player::GetX() const { return x; }
int Player::GetY() const { return y; }
int Player::GetLives() const { return lives; }
int Player::GetBombs() const { return bombsAvailable; }
int Player::GetMaxBombs() const { return maxBombs; }
Player::Difficulty Player::GetDifficulty() const { return difficulty; }
bool Player::IsControlBActive() const { return controlB; }

//! Setters
// Establece la posición del jugador
void Player::SetPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

// Establece la dificultad del juego
void Player::SetDifficulty(Difficulty dif)
{
    difficulty = dif;
}

// Establece el número de vidas del jugador
void Player::SetLives(int newLives)
{
    lives = newLives;
}

// Establece el número de bombas disponibles
void Player::SetBombs(int newBombs)
{
    if (newBombs > maxBombs)
    bombsAvailable = maxBombs;
    else
        bombsAvailable = newBombs;
}

//! Activa o desactiva el control de la tecla B
void Player::ActivateControlB(bool state)
{
    controlB = state;
}

//! Acciones del jugador
// Verifica si el jugador puede colocar una bomba
bool Player::CanPlaceBomb() const
{
    return bombsAvailable > 0;
}

// Coloca una bomba si hay bombas disponibles
// Si hay bombas disponibles, decrementa el contador de bombas
void Player::PlaceBomb()
{
    if (bombsAvailable > 0)
        --bombsAvailable;
}

// Incrementa el número de bombas disponibles si no se ha alcanzado el máximo
void Player::IncrementBombs()
{
    if (bombsAvailable < maxBombs)
        ++bombsAvailable;
}

// Incrementa la cantidad de vidas del jugador
void Player::IncrementLife()
{
    ++lives;
}

// Decrementa la cantidad de vidas del jugador
void Player::LoseLife()
{
    --lives;
}

// Movimiento del jugador
// Mueve al jugador en la dirección especificada si la casilla es caminable
void Player::Move(int dx, int dy, char nextTile)
{
    if (nextTile == ' ' || nextTile == '/' || nextTile == 'B')
    {
        x += dx;
        y += dy;
    }
}

// Intenta mover al jugador en base a un input y el mapa
bool Player::TryMove(char input, Map &map)
{
    // Define las direcciones de movimiento
    int dx = 0, dy = 0;

    // Determina la dirección del movimiento según el input
    switch (input)
    {
    case 'w':
        // Mueve hacia arriba
        dy = -1;
        break;
    case 's':
        // Mueve hacia abajo
        dy = 1;
        break;
    case 'a':
        // Mueve hacia la izquierda
        dx = -1;
        break;
    case 'd':
        // Mueve hacia la derecha
        dx = 1;
        break;
    default:
        // Si el input no es válido, no se mueve
        return false;
    }

    // Calcula las nuevas coordenadas del jugador
    int newX = x + dx;
    int newY = y + dy;
    // Verifica si las nuevas coordenadas están dentro de los límites del mapa
    char tile = map.GetTile(newX, newY);

    // Si la casilla no es caminable, no se mueve
    if (tile == '#' || tile == '0' || tile == '~' || tile == 'H' || tile == 'A')
        return false;

    // Mueve al jugador a la nueva posición
    Move(dx, dy, tile);

    // Si la casilla es una bomba, incrementa el contador de bombas y la elimina del mapa
    if (tile == 'B')
    {
        IncrementBombs();
        map.SetTile(newX, newY, ' ');
    }

    // Si pisa puerta '/', retorna true para indicar cambio de nivel
    return tile == '/';
}

#endif

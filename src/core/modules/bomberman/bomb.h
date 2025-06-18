#ifndef BOMB_H
#define BOMB_H

// Include high-resolution timer utilities
#include <chrono>
#include "utils/player/player.h"

// Bomb class represents a bomb placed by the player in the game.
// It stores its position, the time it was placed, and the delay before it explodes.
class Bomb
{
public:
    // Constructors: default and with position + difficulty
    Bomb();
    Bomb(int x, int y, Player::Difficulty difficulty);

    // Get the X coordinate of the bomb
    int GetX() const;

    // Get the Y coordinate of the bomb
    int GetY() const;

    // Check if the bomb has exploded based on the elapsed time since placement
    bool HasExploded() const;

private:
    int x, y;                                       // Coordinates where the bomb was placed
    int explosionDelay;                             // Time in milliseconds before the bomb explodes
    std::chrono::steady_clock::time_point placedAt; // Timestamp when the bomb was placed
};

// Default constructor: places a bomb at (0, 0) with default delay (Normal difficulty)
Bomb::Bomb() : x(0), y(0), placedAt(std::chrono::steady_clock::now()), explosionDelay(2500) {}

// Constructor with position and difficulty: sets explosion delay based on difficulty level
Bomb::Bomb(int x, int y, Player::Difficulty difficulty)
    : x(x), y(y), placedAt(std::chrono::steady_clock::now())
{
    switch (difficulty)
    {
    case Player::EASY:
        explosionDelay = 3000; // 3 seconds delay
        break;
    case Player::NORMAL:
        explosionDelay = 2500; // 2.5 seconds delay
        break;
    case Player::HARD:
        explosionDelay = 2000; // 2 seconds delay
        break;
    }
}

// Getter for X position
int Bomb::GetX() const { return x; }

// Getter for Y position
int Bomb::GetY() const { return y; }

// Determines whether the bomb has exploded based on current time and delay
bool Bomb::HasExploded() const
{
    // Calculate the time elapsed since the bomb was placed
    // and check if it is greater than or equal to the configured delay
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - placedAt)
               .count() >= explosionDelay;
}

#endif

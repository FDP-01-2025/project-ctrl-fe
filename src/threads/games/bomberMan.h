#ifndef MAIN_BOMBERMAN_H
#define MAIN_BOMBERMAN_H

// Include required header files
#include "utils/functions/utils.h"
#include "core/modules/bomberman/hudBomberman.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/modules/bomberman/bombRenderer.h"
#include "core/modules/bomberman/bomb.h"

// Include neccessary libraries
#include <fstream>
#include <iostream>
#include <conio.h> // for _kbhit() and _getch() that allow non-blocking keyboard input
#include <limits>  // for std::numeric_limits that allows us to clear input buffer
#include <tuple>   // for std::pair used in player movement direction
// Required to play sounds using Windows Multimedia API
#include <mmsystem.h>
// Link the program with the Windows Multimedia library
#pragma comment(lib, "winmm.lib")

#ifdef _WIN32
#include <windows.h> // Only needed for sound effects on Windows
#endif

const int MAX_BOMBS = 25; // Maximum number of bombs allowed

// Main class controlling the Bomberman-style game
class MainBomberman
{
public:
    MainBomberman(); // Constructor initializes state
    bool Run();      // Main game loop, returns true if all levels are passed

private:
    // Game components
    Map map;
    Player player;
    HUDBomberman hud;
    Utils utils;
    BombRenderer bombRenderer;
    std::string difficultyFolder;

    Bomb bombs[MAX_BOMBS]; // Active bomb list, we use a fixed-size array for simplicity because
    // the maximum number of bombs is known and limited
    int bombCount = 0; // Current active bombs

    int currentLevel; // Current game level
    bool isRunning;   // Game running state

    int offsetX = 1, offsetY = 1; // Map drawing offset
    std::string currentMapName;   // Current map file name

    // Internal helper methods
    void ProcessInput(char input);                       // Handle player input
    void LoadLevel(int level);                           // Load level from file
    void HandleExplosion(int i);                         // Handle explosion logic for bomb i
    void ApplyExplosionAt(int x, int y, int dx, int dy); // Apply explosion at direction
    void DetermineDifficultyFolder();                    // Set folder based on difficulty
};

// Constructor: initialize level, bomb count and running flag
MainBomberman::MainBomberman() : currentLevel(1), isRunning(true), bombCount(0) {}

// Main game loop logic
bool MainBomberman::Run()
{
    utils.ClearScreen();
    player.ActivateControlB(true); // Enable 'B' control for placing bombs
    DetermineDifficultyFolder();   // Set level folder based on difficulty
    LoadLevel(currentLevel);       // Load first level
    isRunning = true;

    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\bomberman.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play background music

    while (isRunning)
    {
        utils.ClearScreen();

        // Render map, bombs, player, and HUD
        map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), player.GetX(), player.GetY(), offsetX, offsetY);
        bombRenderer.Draw(bombs, bombCount, offsetX, offsetY);
        hud.Draw(player, currentLevel, map.GetWidth());

        // Check bombs that should explode
        for (int i = 0; i < bombCount;)
        {
            if (bombs[i].HasExploded())
            {
                HandleExplosion(i); // Apply explosion effects

                // Shift bombs to remove exploded one
                for (int j = i; j < bombCount - 1; ++j)
                    bombs[j] = bombs[j + 1];
                --bombCount;
            }
            else
            {
                ++i;
            }
        }

        // Read user input if available
        if (_kbhit())
            ProcessInput(_getch());

        // If player has no more lives, end game
        if (player.GetLives() <= 0)
        {
            utils.ClearScreen();
            isRunning = false;
            return false;
        }

        utils.Sleep(15); // Short delay between frames
    }

    return true; // Game completed successfully
}

// Handle player key input
void MainBomberman::ProcessInput(char input)
{
    // Place a bomb if 'b' is pressed and allowed
    if (input == 'b' && player.IsControlBActive())
    {
        // Check if player can place a bomb and if the maximum limit is not reached
        if (player.CanPlaceBomb() && (bombCount < MAX_BOMBS))
        {
            // Place a bomb at player's current position
            bombs[bombCount++] = Bomb(player.GetX(), player.GetY(), player.GetDifficulty());
            map.SetTile(player.GetX(), player.GetY(), L'¤'); // Mark bomb on map
            // Save player state after placing a bomb
            player.PlaceBomb();
        }
        return;
    }

    // Handle movement input
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    // Allow movement only to walkable tiles
    if (tile == ' ' || tile == 'B' || tile == '/')
    {
        // Pick up bomb power-up
        if (tile == 'B')
        {
            player.IncrementBombs();
            map.SetTile(newX, newY, ' ');
            utils.MoveCursor(offsetX + newX, offsetY + newY);
            return;
        }

        // Move to next level
        if (tile == '/')
        {
            // If player reaches the door, load next level
            int nextLevel = currentLevel + 1;
            // Determine the difficulty folder based on player state
            DetermineDifficultyFolder();
            // Build the next level map path
            std::string nextMapPath = utils.GetAssetsPath() + "maps\\bomberman\\" + difficultyFolder + "\\level" + std::to_string(nextLevel) + ".txt";
            // Check if the next level file exists
            std::ifstream file(nextMapPath, std::ios::binary);

            if (!file.is_open())
            {
                utils.ClearScreen();
                isRunning = false;
                return;
            }
            // Close the file after checking
            file.close();
            currentLevel = nextLevel;
            // Load the next level
            LoadLevel(currentLevel);
            return;
        }

        // Move player if tile is valid
        player.Move(dx, dy, tile);
    }
}

// Load a specific level from file
void MainBomberman::LoadLevel(int level)
{
    // Call the function to set the difficulty folder based on player state
    DetermineDifficultyFolder();

    // Build full path to level file
    std::string currentMapName = utils.GetAssetsPath() + "maps\\bomberman\\" + difficultyFolder + "\\level" + std::to_string(level) + ".txt";

    // Clear screen and load map if found
    utils.ClearScreenComplety();
    // We call ReadMap from the Map class to load the map
    map.ReadMap(currentMapName, map.GetWidth(), map.GetHeight());
    bombCount = 0;                                        // Reset bombs
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY()); // Set spawn point

    // Centrar horizontalmente el mapa
    int consoleWidth = utils.GetConsoleWidth();
    offsetX = (consoleWidth - map.GetWidth()) / 4;
    if (offsetX < 0)
        offsetX = 0;

    hud.SetCenteredOffset(offsetX);

    int consoleHeight = utils.GetConsoleHeight();
    offsetY = (consoleHeight - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}

// Handle the explosion effects of a bomb
void MainBomberman::HandleExplosion(int i)
{
    // We declare the coordinates of the bomb
    int x = bombs[i].GetX();
    int y = bombs[i].GetY();

    // Explode in 5 directions: center + up/down/left/right
    ApplyExplosionAt(x, y, 0, 0);  // Center
    ApplyExplosionAt(x, y, 0, -1); // Up
    ApplyExplosionAt(x, y, 0, 1);  // Down
    ApplyExplosionAt(x, y, -1, 0); // Left
    ApplyExplosionAt(x, y, 1, 0);  // Right

#ifdef _WIN32
    // Play explosion sound (Windows only)
    Beep(800, 50);
    Beep(600, 50);
    Beep(300, 100);
#endif

    // Redraw game state after explosion
    utils.ClearScreen();
    hud.Draw(player, currentLevel, map.GetWidth());
    map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), player.GetX(), player.GetY(), offsetX, offsetY);
    bombRenderer.Draw(bombs, bombCount, offsetX, offsetY);
    utils.Sleep(30);

    // Clean up explosion fire tiles, we use a 3x3 area around the bomb
    // This will remove the explosion fire tiles in cardinal directions
    // We iterate over a 3x3 area centered on the bomb
    // and remove any fire tiles ('*' or '0') in cardinal directions
    // This will remove the explosion fire tiles in cardinal directions
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0)) // Only cardinal directions
            {
                int nx = x + dx, ny = y + dy;
                wchar_t tile = map.GetTile(nx, ny);
                if (tile == '*' || tile == '0')
                    map.SetTile(nx, ny, ' ');
            }
        }
    }
}

// Apply the effect of an explosion at a given tile (center or direction)
void MainBomberman::ApplyExplosionAt(int x, int y, int dx, int dy)
{
    // Calculate the coordinates of the tile to apply explosion
    // We calculate the new coordinates based on the current position and direction
    // dx and dy are the direction offsets, e.g. (0, -1) for up
    // We calculate the new coordinates based on the current position and direction
    int nx = x + dx;
    int ny = y + dy;
    wchar_t tile = map.GetTile(nx, ny);

    // Player hit by explosion
    if (nx == player.GetX() && ny == player.GetY())
        player.LoseLife();

    // If the tile is destructible, destroy it or spawn power-up
    if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0')
    {
        // If the tile is destructible, we can destroy it
        if (tile == '%')
        {
            switch (player.GetDifficulty())
            {
            // Probability of spawning bombs based on difficulty
            case Player::Difficulty::EASY:
                map.SetTile(nx, ny, (rand() % 2 == 0) ? 'B' : '*'); // 50% chance to spawn a bomb power-up
                break;
            case Player::Difficulty::NORMAL:
                map.SetTile(nx, ny, (rand() % 3 == 0) ? 'B' : '*'); // 33% chance
                break;
            case Player::Difficulty::HARD:
                map.SetTile(nx, ny, (rand() % 5 == 0) ? 'B' : '*'); // 20% chance
                break;
            }
        }
        else
            map.SetTile(nx, ny, '*');
    }
}

// Set difficulty folder string based on selected difficulty
void MainBomberman::DetermineDifficultyFolder()
{
    // First get the difficulty from player state and set the folder accordingly
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        // Set the folder for easy levels
        difficultyFolder = "easy-levels";
        break;
    case Player::Difficulty::NORMAL:
        // Set the folder for medium levels
        difficultyFolder = "medium-levels";
        break;
    case Player::Difficulty::HARD:
        // Set the folder for hard levels
        difficultyFolder = "hard-levels";
        break;
    }
}

#endif

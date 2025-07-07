#ifndef MAIN_BOSS_FIGHT_ZELDA_H
#define MAIN_BOSS_FIGHT_ZELDA_H

// Standard headers
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <cstdlib>

// Project headers
#include "utils/functions/utils.h"
#include "utils/screen/colors.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/modules/bomberman/bomb.h"
#include "core/modules/bomberman/bombRenderer.h"
#include "core/modules/hud/hudBossZelda.h"
// Required to play sounds using Windows Multimedia API
#include <mmsystem.h>
// Link the program with the Windows Multimedia library
#pragma comment(lib, "winmm.lib")
// Main class for the Zelda-style boss fight
class MainBossFightZelda
{
public:
    MainBossFightZelda() {}

    // Main loop to run the boss fight sequence
    bool Run()
    {
        // Load the Zelda temple map from file
        std::string currentMapName = utils.GetAssetsPath() + "maps\\zelda\\temple.txt";
        map.ReadMap(currentMapName, map.GetWidth(), map.GetHeight());

        // Calculate the starting X position to center the map and HUD in the console
        int consoleWidth = utils.GetConsoleWidth();
        int mapWidth = map.GetWidth();
        int mapStartX = (consoleWidth - mapWidth - 25 - 3) / 2;
        if (mapStartX < 0)
            mapStartX = 0;

        // Set the player's initial spawn position based on the map data
        player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

        // Set boss and gameplay parameters based on selected difficulty
        int difficulty = player.GetDifficulty();
        int fireInterval = 20; // Time between boss projectile attacks
        int moveInterval = 10; // Time between statue moves
        switch (difficulty)
        {
        case Player::EASY:
            maxHealth = 3;
            fireInterval = 20;
            moveInterval = 15;
            break;
        case Player::NORMAL:
            maxHealth = 5;
            fireInterval = 15;
            moveInterval = 10;
            break;
        case Player::HARD:
            maxHealth = 7;
            fireInterval = 15;
            moveInterval = 8;
            break;
        }

        // Initialize statue position, bomb state, timers, and other variables
        statueX = 15;
        statueY = 3;
        damageTaken = 0;
        bombPlaced = false;
        projectileCooldown = 0;
        offsetX = 24; // X offset for rendering the scene
        offsetY = 10; // Y offset for rendering the scene
        lastMoveTime = std::chrono::steady_clock::now();
        lastFireTime = std::chrono::steady_clock::now();
        lastBombSpawnTime = std::chrono::steady_clock::now();
        isRunning = true;
        projectileCount = 0;
        randomBombAvailable = false;
        bombX = bombY = -1;
        bossDefeated = false;

        utils.ClearScreenComplety(); // Clear the console before starting

        std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\bossZelda.wav";
        PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play background music

        // Main game loop: runs until the player wins, loses, or exits
        while (isRunning)
        {
            utils.ClearScreen();

            // Save current projectile positions to erase them in the next frame
            std::pair<int, int> previousProjectiles[MAX_PROJECTILES];
            for (int i = 0; i < projectileCount; ++i)
                previousProjectiles[i] = projectiles[i];

            // Draw the current game scene, including map, player, and boss
            DrawScene();

            // Draw the boss HUD, showing health and defeat status
            hud.SetCenteredOffset(mapStartX);
            int consoleHeight = utils.GetConsoleHeight();
            if (!bossDefeated)
                hud.DrawDefeat(player, mapWidth);
            else
            {
                hud.ClearHUDArea(mapWidth, consoleHeight);
                hud.DrawGoAway(player, mapWidth);
            }
            // Draw the bomb if it has been placed by the player
            if (bombPlaced)
                bombRenderer.Draw(&bomb, 1, offsetX, offsetY);

            auto now = std::chrono::steady_clock::now();

            // Move the statue randomly every moveInterval seconds
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastMoveTime).count() >= moveInterval)
            {
                int dx = (rand() % 3) - 1;
                int dy = (rand() % 3) - 1;
                // Only move if the new position is empty
                if (map.GetTile(statueX + dx, statueY + dy) == ' ')
                {
                    statueX += dx;
                    statueY += dy;
                }
                lastMoveTime = now;
            }

            // Fire projectiles from the statue if cooldown is over
            if (projectileCooldown == 0)
            {
                projectileCount = 0;
                // Add projectiles in four cardinal directions
                AddProjectile(statueX, statueY - 1); // Up
                AddProjectile(statueX, statueY + 1); // Down
                AddProjectile(statueX - 1, statueY); // Left
                AddProjectile(statueX + 1, statueY); // Right
                // On hard difficulty, add diagonal projectiles
                if (difficulty == Player::HARD)
                {
                    AddProjectile(statueX - 1, statueY - 1); // Diagonals
                    AddProjectile(statueX + 1, statueY - 1);
                    AddProjectile(statueX - 1, statueY + 1);
                    AddProjectile(statueX + 1, statueY + 1);
                }
                projectileCooldown = fireInterval;
            }
            else
            {
                // Move each projectile in its direction and check for collisions
                std::pair<int, int> updated[MAX_PROJECTILES];
                int updatedCount = 0;
                for (int i = 0; i < projectileCount; ++i)
                {
                    auto p = projectiles[i];
                    int dx = p.first - statueX;
                    int dy = p.second - statueY;

                    // Update projectile position based on its direction
                    if (dx == 0 && dy < 0)
                        p.second--;
                    else if (dx == 0 && dy > 0)
                        p.second++;
                    else if (dy == 0 && dx < 0)
                        p.first--;
                    else if (dy == 0 && dx > 0)
                        p.first++;
                    else if (dx < 0 && dy < 0)
                    {
                        p.first--;
                        p.second--;
                    }
                    else if (dx > 0 && dy < 0)
                    {
                        p.first++;
                        p.second--;
                    }
                    else if (dx < 0 && dy > 0)
                    {
                        p.first--;
                        p.second++;
                    }
                    else if (dx > 0 && dy > 0)
                    {
                        p.first++;
                        p.second++;
                    }

                    // Only keep projectiles within map bounds and not hitting walls
                    if (p.first >= 0 && p.second >= 0 && p.first < map.GetWidth() && p.second < map.GetHeight())
                    {
                        wchar_t tile = map.GetTile(p.first, p.second);
                        if (tile != '#' && tile != '~' && tile != ']')
                            updated[updatedCount++] = p;
                    }

                    // If projectile hits the player or statue, player loses a life
                    if ((p.first == player.GetX() && p.second == player.GetY()))
                        player.LoseLife();
                }
                // Update the projectile list for the next frame
                for (int i = 0; i < updatedCount; ++i)
                    projectiles[i] = updated[i];
                projectileCount = updatedCount;
                projectileCooldown--;
            }

            // Erase old projectiles from the previous frame
            for (int i = 0; i < projectileCount; ++i)
            {
                auto p = previousProjectiles[i];
                if (p.first >= 0 && p.second >= 0 && p.first < map.GetWidth() && p.second < map.GetHeight())
                {
                    utils.MoveCursor(offsetX + p.first, offsetY + p.second);
                    wchar_t tile = map.GetTile(p.first, p.second);
                    if (tile == ' ')
                        std::wcout << L' ';
                }
            }

            // Draw projectiles with direction symbols for visual feedback
            for (int i = 0; i < projectileCount; ++i)
            {
                auto &p = projectiles[i];
                utils.MoveCursor(offsetX + p.first, offsetY + p.second);
                int dx = p.first - statueX;
                int dy = p.second - statueY;
                // Choose the correct arrow symbol based on direction
                if (dx > 0 && dy == 0)
                    std::wcout << RED << L"→" << RESET;
                else if (dx < 0 && dy == 0)
                    std::wcout << RED << L"←" << RESET;
                else if (dx == 0 && dy > 0)
                    std::wcout << RED << L"↓" << RESET;
                else if (dx == 0 && dy < 0)
                    std::wcout << RED << L"↑" << RESET;
                else if (dx < 0 && dy < 0)
                    std::wcout << RED << L"↖" << RESET;
                else if (dx > 0 && dy < 0)
                    std::wcout << RED << L"↗" << RESET;
                else if (dx < 0 && dy > 0)
                    std::wcout << RED << L"↙" << RESET;
                else if (dx > 0 && dy > 0)
                    std::wcout << RED << L"↘" << RESET;
            }

            // Handle bomb explosion and apply damage to the boss if in range
            if (bombPlaced && bomb.HasExploded())
            {
                int bx = bomb.GetX();
                int by = bomb.GetY();
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        // Only cross-shaped explosion (no diagonals)
                        if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0))
                        {
                            int nx = bx + dx;
                            int ny = by + dy;
                            if (nx == statueX && ny == statueY)
                                damageTaken++;
                            wchar_t tile = map.GetTile(nx, ny);
                            if (tile != '#' && tile != '~' && tile != ']')
                                map.SetTile(nx, ny, '*');
                        }
                    }
                }

                bombPlaced = false;
                utils.ClearScreen();
                DrawScene();
                utils.Sleep(250);
#ifdef _WIN32
                // Play explosion sound (Windows only)
                Beep(800, 50);
                Beep(600, 50);
                Beep(300, 100);
#endif

                // Clear explosion marks from the map
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0))
                        {
                            int nx = bx + dx;
                            int ny = by + dy;
                            if (map.GetTile(nx, ny) == '*')
                                map.SetTile(nx, ny, ' ');
                        }
                    }
                }
            }

            // Spawn a random bomb on the map every 7 seconds if none is available
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastBombSpawnTime).count() >= 7 && !randomBombAvailable)
            {
                for (int attempt = 0; attempt < 50; ++attempt)
                {
                    int rx = rand() % map.GetWidth();
                    int ry = rand() % map.GetHeight();
                    if (map.GetTile(rx, ry) == ' ')
                    {
                        bombX = rx;
                        bombY = ry;
                        map.SetTile(rx, ry, L'B');
                        randomBombAvailable = true;
                        break;
                    }
                }
                lastBombSpawnTime = now;
            }

            // Allow player to pick up the random bomb if standing on it
            if (player.GetX() == bombX && player.GetY() == bombY && randomBombAvailable)
            {
                player.IncrementBombs();
                map.SetTile(bombX, bombY, ' ');
                randomBombAvailable = false;
            }

            // Handle player input for movement and bomb placement
            if (_kbhit())
            {
                char input = _getch();
                if (input == 'b' && !bombPlaced)
                {
                    // Place a bomb at the player's current position
                    bomb = Bomb(player.GetX(), player.GetY(), player.GetDifficulty());
                    map.SetTile(player.GetX(), player.GetY(), L'¤');
                    bombPlaced = true;
                    player.PlaceBomb();
                }
                else
                {
                    // Move the player based on input direction
                    auto dir = player.GetInputDirection(input);
                    int nx = player.GetX() + dir.first;
                    int ny = player.GetY() + dir.second;
                    wchar_t tile = map.GetTile(nx, ny);
                    if ((nx != statueX || ny != statueY) && (tile == ' ' || tile == '/' || tile == 'B'))
                    {
                        player.Move(dir.first, dir.second, tile);
                        // If player reaches the exit, end the fight
                        if (tile == '/')
                        {
                            utils.ClearScreen();
                            isRunning = false;
                            return true;
                        }
                    }
                }
            }

            // Check for game over condition (player out of lives)
            if (player.GetLives() <= 0)
            {
                utils.ClearScreen();
                isRunning = false;
                return false;
            }

            // Check if boss is defeated and open the exit
            if (damageTaken >= maxHealth && !bossDefeated)
            {
                map.SetTile(statueX, statueY, ' '); // Remove statue from map
                map.SetTile(statueX, statueY, '/'); // Place exit door
                bossDefeated = true;
            }

            // Sleep briefly to reduce CPU usage and control game speed
            utils.Sleep(30);
        }
    }

private:
    // Maximum number of projectiles on screen at once
    static const int MAX_PROJECTILES = 100;

    // Core game components and utilities
    Utils utils;
    Map map;
    Player player;
    Bomb bomb;
    BombRenderer bombRenderer;
    HUDBossZelda hud;

    // Boss and gameplay state variables
    int statueX, statueY;
    int damageTaken, maxHealth;
    int offsetX, offsetY;
    bool isRunning, bombPlaced;
    int projectileCooldown;
    std::pair<int, int> projectiles[MAX_PROJECTILES];
    int projectileCount;
    bool bossDefeated;

    // Timers and random bomb state
    std::chrono::steady_clock::time_point lastMoveTime, lastFireTime, lastBombSpawnTime;
    int bombX, bombY;
    bool randomBombAvailable;

    // Add a new projectile to the list if under the max limit
    void AddProjectile(int x, int y)
    {
        if (projectileCount < MAX_PROJECTILES)
            projectiles[projectileCount++] = {x, y};
    }

    // Draws the entire scene: map, player, statue, and HUD
    void DrawScene()
    {
        // Draw boss health bar and label
        utils.MoveCursor(offsetX, offsetY - 2);
        std::wcout << L"Statue \u03A9 ";
        int barLength = 20;
        int filled = (int)((float)(maxHealth - damageTaken) / maxHealth * barLength);
        std::wcout << CYAN << L"[";
        for (int i = 0; i < barLength; ++i)
            std::wcout << (i < filled ? L"█" : L" ");
        std::wcout << L"]" << RESET;

        utils.MoveCursor(offsetX, offsetY - 1);
        std::wcout << L"HP: " << (maxHealth - damageTaken) << L"/" << maxHealth;

        // Draw the map and all entities (player, statue, bombs, etc.)
        for (int y = 0; y < map.GetHeight(); ++y)
        {
            for (int x = 0; x < map.GetWidth(); ++x)
            {
                utils.MoveCursor(offsetX + x, offsetY + y);
                if (x == player.GetX() && y == player.GetY())
                    std::wcout << PINK << L"இ" << RESET;
                else if (x == statueX && y == statueY && damageTaken < maxHealth)
                    std::wcout << CYAN << L"Ω" << RESET;
                else
                {
                    wchar_t tile = map.GetTile(x, y);
                    // Draw each tile with appropriate color and symbol
                    switch (tile)
                    {
                    case '#':
                        std::wcout << GRAY << L"#" << RESET;
                        break;
                    case ']':
                        std::wcout << GRAY << L"]" << RESET;
                        break;
                    case 'B':
                        std::wcout << ORANGE << L"¤" << RESET;
                        break;
                    case 'A':
                        std::wcout << GREEN << L"♣" << RESET;
                        break;
                    case '~':
                        std::wcout << BLUE << L"≈" << RESET;
                        break;
                    case '*':
                        std::wcout << YELLOW_BRIGHT << L"✹" << RESET;
                        break;
                    case '/':
                        std::wcout << GREEN << L"/" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }
};

#endif

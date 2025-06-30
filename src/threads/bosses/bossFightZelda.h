#ifndef MAIN_BOSS_FIGHT_ZELDA_H
#define MAIN_BOSS_FIGHT_ZELDA_H

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <cstdlib>

#include "utils/functions/utils.h"
#include "utils/screen/colors.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/modules/bomberman/bomb.h"
#include "core/modules/bomberman/bombRenderer.h"
#include "core/modules/hud/hudBossZelda.h"

class MainBossFightZelda
{
public:
    MainBossFightZelda() {}

    void Run()
    {
        std::string currentMapName = utils.GetAssetsPath() + "maps\\zelda\\temple.txt";
        map.ReadMap(currentMapName, map.GetWidth(), map.GetHeight());
        int consoleWidth = utils.GetConsoleWidth();
        int mapWidth = map.GetWidth();                          // Asumiendo que tienes una función así
        int mapStartX = (consoleWidth - mapWidth - 25 - 3) / 2; // 25 es el ancho del HUD, 3 es espacio extra

        if (mapStartX < 0)
            mapStartX = 0; // Por si la consola es muy pequeña

        player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

        int difficulty = player.GetDifficulty();
        int fireInterval = 20;
        int moveInterval = 10;
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

        statueX = 15;
        statueY = 3;
        damageTaken = 0;
        bombPlaced = false;
        projectileCooldown = 0;
        offsetX = 24;
        offsetY = 10;
        lastMoveTime = std::chrono::steady_clock::now();
        lastFireTime = std::chrono::steady_clock::now();
        lastBombSpawnTime = std::chrono::steady_clock::now();
        isRunning = true;
        projectileCount = 0;
        randomBombAvailable = false;
        bombX = bombY = -1;
        bossDefeated = false;

        while (isRunning)
        {
            utils.ClearScreen();
            std::pair<int, int> previousProjectiles[MAX_PROJECTILES];
            for (int i = 0; i < projectileCount; ++i)
                previousProjectiles[i] = projectiles[i];

            DrawScene();

            hud.SetCenteredOffset(mapStartX); // Para que empiece donde termina el mapa
            if (!bossDefeated)
            {
                hud.DrawDefeat(player, mapWidth);
            }
            else
            {
                hud.DrawGoAway(player, mapWidth);
            }

            if (bombPlaced)
                bombRenderer.Draw(&bomb, 1, offsetX, offsetY);

            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastMoveTime).count() >= moveInterval)
            {
                int dx = (rand() % 3) - 1;
                int dy = (rand() % 3) - 1;
                if (map.GetTile(statueX + dx, statueY + dy) == ' ')
                {
                    statueX += dx;
                    statueY += dy;
                }
                lastMoveTime = now;
            }

            if (projectileCooldown == 0)
            {
                projectileCount = 0;
                AddProjectile(statueX, statueY - 1);
                AddProjectile(statueX, statueY + 1);
                AddProjectile(statueX - 1, statueY);
                AddProjectile(statueX + 1, statueY);
                if (difficulty == Player::HARD)
                {
                    AddProjectile(statueX - 1, statueY - 1);
                    AddProjectile(statueX + 1, statueY - 1);
                    AddProjectile(statueX - 1, statueY + 1);
                    AddProjectile(statueX + 1, statueY + 1);
                }
                projectileCooldown = fireInterval;
            }
            else
            {
                std::pair<int, int> updated[MAX_PROJECTILES];
                int updatedCount = 0;
                for (int i = 0; i < projectileCount; ++i)
                {
                    auto p = projectiles[i];
                    int dx = p.first - statueX;
                    int dy = p.second - statueY;
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

                    if (p.first >= 0 && p.second >= 0 && p.first < map.GetWidth() && p.second < map.GetHeight())
                    {
                        wchar_t tile = map.GetTile(p.first, p.second);
                        if (tile != '#' && tile != '~' && tile != ']')
                        {
                            updated[updatedCount++] = p;
                        }
                    }

                    if ((p.first == player.GetX() && p.second == player.GetY()) || (player.GetX() == statueX && player.GetY() == statueY))
                    {
                        player.LoseLife();
                    }
                }
                for (int i = 0; i < updatedCount; ++i)
                    projectiles[i] = updated[i];
                projectileCount = updatedCount;
                projectileCooldown--;
            }

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

            for (int i = 0; i < projectileCount; ++i)
            {
                auto &p = projectiles[i];
                utils.MoveCursor(offsetX + p.first, offsetY + p.second);
                int dx = p.first - statueX;
                int dy = p.second - statueY;
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

            if (bombPlaced && bomb.HasExploded())
            {
                int bx = bomb.GetX();
                int by = bomb.GetY();
                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
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

            if (player.GetX() == bombX && player.GetY() == bombY && randomBombAvailable)
            {
                player.IncrementBombs();
                map.SetTile(bombX, bombY, ' ');
                randomBombAvailable = false;
            }

            if (_kbhit())
            {
                char input = _getch();
                if (input == 'b' && !bombPlaced)
                {
                    bomb = Bomb(player.GetX(), player.GetY(), player.GetDifficulty());
                    map.SetTile(player.GetX(), player.GetY(), L'¤');
                    bombPlaced = true;
                    player.PlaceBomb();
                }
                else
                {
                    auto dir = player.GetInputDirection(input);
                    int nx = player.GetX() + dir.first;
                    int ny = player.GetY() + dir.second;
                    wchar_t tile = map.GetTile(nx, ny);
                    if (tile == ' ' || tile == '/' || tile == 'B')
                    {
                        player.Move(dir.first, dir.second, tile);
                        if (tile == '/')
                        {
                            utils.ClearScreen();
                            isRunning = false;
                            return;
                        }
                    }
                }
            }

            if (player.GetLives() <= 0)
            {
                utils.ClearScreen();
                std::wcout << L"\nYou have been defeated. Game Over.\n";
                isRunning = false;
                return;
            }

            if (damageTaken >= maxHealth && !bossDefeated)
            {
                map.SetTile(statueX, statueY, ' '); // Eliminar estatua
                map.SetTile(statueX, statueY, '/'); // Colocar puerta de salida
                bossDefeated = true;
            }

            utils.Sleep(30);
        }
    }

private:
    static const int MAX_PROJECTILES = 100;
    Utils utils;
    Map map;
    Player player;
    Bomb bomb;
    BombRenderer bombRenderer;
    HUDBossZelda hud; // HUD for the boss fight

    int statueX, statueY;
    int damageTaken, maxHealth;
    int offsetX, offsetY;
    bool isRunning, bombPlaced;
    int projectileCooldown;
    std::pair<int, int> projectiles[MAX_PROJECTILES];
    int projectileCount;
    bool bossDefeated = false;

    std::chrono::steady_clock::time_point lastMoveTime, lastFireTime, lastBombSpawnTime;
    int bombX, bombY;
    bool randomBombAvailable;

    void AddProjectile(int x, int y)
    {
        if (projectileCount < MAX_PROJECTILES)
        {
            projectiles[projectileCount++] = {x, y};
        }
    }

    void DrawScene()
    {

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

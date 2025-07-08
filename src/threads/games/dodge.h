#ifndef MAIN_DODGE_EVENT_H
#define MAIN_DODGE_EVENT_H

#include "utils/functions/utils.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/engine/settings/console.h"
#include "core/engine/tool/dialogues.h"
#include "core/modules/hud/hudDodge.h"
#include "threads/startup/gameOver.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <random>
#include <algorithm>

// Required to play sounds using Windows Multimedia API
#include <mmsystem.h>
// Link the program with the Windows Multimedia library
#pragma comment(lib, "winmm.lib")

#ifdef _WIN32
#include <windows.h>
#endif

class DodgeEvent
{
public:
    DodgeEvent();
    bool Run();

private:
    bool isRunning = true;

    const int mapWidth = 35; //
    const int mapHeight = 10;

    int offsetX = 0;
    int offsetY = 0;
    char input;

    int tickCounter = 0;
    static const int MAX_PROJECTILES = 20; // the max in showed screen
    int projectileCount = 0;               // current projectiles
    int projX[MAX_PROJECTILES];
    int projY[MAX_PROJECTILES];
    int projDX[MAX_PROJECTILES];
    int projDY[MAX_PROJECTILES];
    int remainingProjectiles = 30; // total projectiles during the game

    bool finished = false;

    Utils utils;
    Map map;
    Player player;
    HUDDodge hud;
    GameOverScreen gameOver;

    void LoadMap();
    void DrawStaticMap();
    void SpawnProjectile();
    void UpdateProjectiles();
    void DrawProjectiles();
    bool CheckCollision();
    void ProcessInput(char input);
};

inline DodgeEvent::DodgeEvent() {}

inline void DodgeEvent::LoadMap()
{
    std::string path = utils.GetAssetsPath() + "maps\\dodge\\dodgeRoom.txt";
    map.ReadMap(path, mapWidth, mapHeight);

    offsetX = (utils.GetConsoleWidth() - map.GetWidth()) / 2;
    if (offsetX < 0)
        offsetX = 0;
    offsetY = (utils.GetConsoleHeight() - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}

inline void DodgeEvent::DrawStaticMap()
{
    for (int y = 0; y < map.GetHeight(); ++y)
    {
        for (int x = 0; x < map.GetWidth(); ++x)
        {
            utils.MoveCursor(offsetX + x, offsetY + y);
            wchar_t tile = map.GetTile(x, y);

            switch (tile)
            {
            case '#':
                std::wcout << GRAY << L"#" << RESET;
                break;
            case '/':
                std::wcout << GRAY << L"/" << RESET;
                break;
            default:
                std::wcout << tile;
                break;
            }
        }
    }
}

inline void DodgeEvent::SpawnProjectile()
{
    if (projectileCount >= MAX_PROJECTILES)
        return;

    int x, y, dx, dy;
    int edge = rand() % 4;

    switch (edge)
    {
    case 0:
        x = 0;
        y = rand() % mapHeight;
        dx = 1;
        dy = 0;
        break;
    case 1:
        x = rand() % mapWidth;
        y = 0;
        dx = 0;
        dy = 1;
        break;
    case 2:
        x = mapWidth - 1;
        y = rand() % mapHeight;
        dx = -1;
        dy = 0;
        break;
    case 3:
        x = rand() % mapWidth;
        y = mapHeight - 1;
        dx = 0;
        dy = -1;
        break;
    }

    projX[projectileCount] = x;
    projY[projectileCount] = y;
    projDX[projectileCount] = dx;
    projDY[projectileCount] = dy;
    projectileCount++;
    remainingProjectiles--;
}

inline void DodgeEvent::UpdateProjectiles()
{
    for (int i = 0; i < projectileCount; ++i)
    {
        // Borrar la posición anterior
        utils.MoveCursor(offsetX + projX[i], offsetY + projY[i]);
        wchar_t originalTile = map.GetTile(projX[i], projY[i]);
        switch (originalTile)
        {
        case '#':
            std::wcout << GRAY << L"#" << RESET;
            break;
        case '/':
            std::wcout << GRAY << L"/" << RESET;
            break;
        default:
            std::wcout << originalTile;
            break;
        }

        // Actualizar posición
        projX[i] += projDX[i];
        projY[i] += projDY[i];
    }

    // Filtrar proyectiles fuera o que chocan
    int writeIndex = 0;
    for (int i = 0; i < projectileCount; ++i)
    {
        bool insideMap = (projX[i] >= 0 && projX[i] < mapWidth && projY[i] >= 0 && projY[i] < mapHeight);
        wchar_t tile = insideMap ? map.GetTile(projX[i], projY[i]) : '#';

        if (insideMap && tile != '#' && tile != '/')
        {
            projX[writeIndex] = projX[i];
            projY[writeIndex] = projY[i];
            projDX[writeIndex] = projDX[i];
            projDY[writeIndex] = projDY[i];
            writeIndex++;
        }
    }
    projectileCount = writeIndex;
}

inline void DodgeEvent::DrawProjectiles()
{
    for (int i = 0; i < projectileCount; ++i)
    {
        utils.MoveCursor(offsetX + projX[i], offsetY + projY[i]);
        std::wcout << RED << L"X" << RESET;
    }
}

inline bool DodgeEvent::CheckCollision()
{
    for (int i = 0; i < projectileCount; ++i)
    {
        if (projX[i] == player.GetX() && projY[i] == player.GetY())
            return true;
    }
    return false;
}

inline void DodgeEvent::ProcessInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == '/')
    {
        if (tile == '/') // to leave the room
        {
            utils.ClearScreen();
            isRunning = false;
        }
        utils.MoveCursor(offsetX + player.GetX(), offsetY + player.GetY());
        std::wcout << L" ";

        // move the player
        player.Move(dx, dy, tile);
        utils.MoveCursor(offsetX + player.GetX(), offsetY + player.GetY());
        std::wcout << PINK << L"இ" << RESET;
    }
}

inline bool DodgeEvent::Run()
{
    LoadMap();
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    DrawStaticMap();

    // Dibuja al jugador por primera vez
    utils.MoveCursor(offsetX + player.GetX(), offsetY + player.GetY());
    std::wcout << PINK << L"இ" << RESET;
    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\dodgeRoom.wav"; //music
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (isRunning)
    {

        UpdateProjectiles();
        DrawProjectiles();
        utils.MoveCursor(offsetX + player.GetX(), offsetY + player.GetY());
        std::wcout << PINK << L"இ" << RESET;

        hud.SetCenteredOffset(offsetX);
        hud.DrawDodgeEvent(player, player.GetRoom(), map.GetWidth(), remainingProjectiles);

        if (remainingProjectiles != 0) // the logic of the projectiles
        {
            if (tickCounter % 10 == 0)
            {
                SpawnProjectile();
            }
        }

        if (_kbhit())
        {
            input = _getch();
            if (input == 27)
                break; // ESC

            ProcessInput(input);
        }

        if (CheckCollision())
        {
            player.DecrementLives(1);
            if (player.GetLives() <= 0)
            {
                // gameOver.Show(utils);
                isRunning = false;
            }
        }

        tickCounter++;
        Sleep(30);

        if (remainingProjectiles == 0 && projectileCount == 0)
        {
            DrawDialogue(L"It seems the door has been opened", offsetX, offsetY - 4);
            map.SetTile(33, 8, L'/'); // spawns the door
            utils.MoveCursor(offsetX + 33, offsetY + 8);
            std::wcout << YELLOW << L"/" << RESET;
        }
    }

    return true;
}

#endif

#ifndef MAIN_CHEST_EVENT_H
#define MAIN_CHEST_EVENT_H

#include "utils/functions/utils.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/engine/settings/console.h"
#include "core/engine/tool/dialogues.h"

#include <iostream>
#include <conio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

class ChestEvent
{
public:
    ChestEvent();
    bool Run();

private:
    bool isRunning = true;
    bool chestOpened = false;

    const int mapWidth = 35;
    const int mapHeight = 10;

    // Chest location
    const int chestX[5] = {15, 16, 17, 18, 19};
    const int chestY[2] = {1, 2};

    int offsetX = 1;
    int offsetY = 1;

    Utils utils;
    Map map;
    Player player;
    // Console console;

    char input;

    bool IsOverChest(int x, int y);
    void ProcessInput(char input);
    void ChestInteraction();
    void WaitKey();
    void LoadMap();
    void DrawPrompt(const std::wstring &message, int delay = 0);
};

inline ChestEvent::ChestEvent() {}

inline bool ChestEvent::IsOverChest(int x, int y)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 5; j++)
            if (x == chestX[j] && y == chestY[i])
                return true;
    return false;
}

inline void ChestEvent::WaitKey()
{
    utils.MoveCursor(0, mapHeight + 4);
    std::wcout << L"\nPress any key to continue...";
    _getch();
}

inline void ChestEvent::DrawPrompt(const std::wstring &message, int delay)
{
    utils.ClearScreen();
    DrawDialogue(message);
    if (delay > 0)
    {
#ifdef _WIN32
        Sleep(delay * 1000);
#else
        std::this_thread::sleep_for(std::chrono::seconds(delay));
#endif
    }
}

inline void ChestEvent::ChestInteraction()
{
    DrawPrompt(L"Do you want to open the chest? (y/n)");

    char answer;
    do
    {
        answer = tolower(_getch());
    } while (answer != 'y' && answer != 'n');

    if (answer == 'y')
    {
        chestOpened = true;
        // Limpiar zona del cofre (quitar cofre)
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 5; j++)
                map.SetTile(chestX[j], chestY[i], ' ');

        DrawPrompt(L"You opened the chest! You found a treasure!", 3);
    }
    else
    {
        DrawPrompt(L"You didn't open the chest...", 2);
    }

    WaitKey();
}

inline void ChestEvent::ProcessInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == '/' || tile == '-' || tile == '_')
    {
        if (tile == '/')
        {
            isRunning = false;
        }
        else
        {
            player.Move(dx, dy, tile);
        }
    }
}

inline void ChestEvent::LoadMap()
{
    std::string path = utils.GetAssetsPath() + "maps\\chest\\goodChestRoom.txt";
    map.ReadMap(path, mapWidth, mapHeight);

    // To center the map
    offsetX = (utils.GetConsoleWidth() - map.GetWidth()) / 4;
    if (offsetX < 0)
        offsetX = 0;

    offsetY = (utils.GetConsoleHeight() - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}

inline bool ChestEvent::Run()
{
    player.ResetState(Player::NORMAL);
    LoadMap();
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    while (isRunning)
    {
        utils.ClearScreen();

        map.DrawWithPlayer(mapWidth, mapHeight, player.GetX(), player.GetY(), offsetX, offsetY); // for the map

        if (_kbhit())
        {
            input = _getch();
            ProcessInput(input);

            if (!chestOpened && IsOverChest(player.GetX(), player.GetY()))
            {
                ChestInteraction();
            }

            if (input == 27) // ESC para salir
                isRunning = false;
        }

#ifdef _WIN32
        Sleep(20);
#endif
    }

    return true;
}

#endif

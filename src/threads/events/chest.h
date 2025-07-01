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
    const int chestLockX = 17;
    const int chestLockY = 3;

    int offsetX = 1;
    int offsetY = 1;

    Utils utils;
    Map map;
    Player player;
    // Console console;

    char input;

    void ProcessInput(char input);
    void ChestInteraction();
    void WaitKey();
    void LoadMap();
    void DrawPrompt(const std::wstring &message, int delay = 0);
};

inline ChestEvent::ChestEvent() {};

inline void ChestEvent::WaitKey()
{
    int msgX = std::max(0, map.GetWidth() / 2 - 20);
    utils.MoveCursor(msgX, offsetY + 11);
    std::wcout << L"Press any key to continue...";
    Sleep(2000);
    _getch();
    utils.MoveCursor(msgX, offsetY + 11);
    std::wcout << L"                            "; // for cleaning
}

inline void ChestEvent::ChestInteraction() // asking the player to open the chest
{
    int msgX = std::max(0, (map.GetWidth() / 2) + 3); // horizontal position
    utils.MoveCursor(msgX, offsetY - 3);
    std::wcout << L"Do you want to open the chest?";
    utils.MoveCursor(msgX, offsetY - 2);
    std::wcout << L"(Press 'Y' to open it)";

    char answer;

    answer = tolower(_getch());
    if (answer == 'y')
    {
        chestOpened = true;

        int msgX = std::max(0, (map.GetWidth() / 2) + 3);
        utils.MoveCursor(msgX, offsetY - 3);
        std::wcout << L"You opened the chest and gained an extra life!!";
        utils.MoveCursor(msgX, offsetY - 2);
        std::wcout << L"                      ";
        DrawDialogue(L"1+ life", 13, 2);
        Sleep(2000);

        WaitKey(); // this is for waiting

        // for cleaning the chest area after it's opened
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 5; j++)
                map.SetTile(chestX[j], chestY[i], ' ');

        utils.MoveCursor(msgX, offsetY - 2); // cleaning the messages

        utils.MoveCursor(msgX, offsetY + 2);
        std::wcout << L"                                                   ";
    }
    else
    {
        utils.MoveCursor(msgX, offsetY - 3);
        std::wcout << L"                              ";
        utils.MoveCursor(msgX, offsetY - 2);
        std::wcout << L"                      ";
    }
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

    if (tile == ' ' || tile == '/' || tile == '-' || chestLockX || chestLockY)
    {
        if (tile == '/') // to leave the room
        {
            utils.ClearScreen();
            isRunning = false;
        }
        else
        {
            player.Move(dx, dy, tile);
        }
    }
    // if the player is front the chest
    if (!chestOpened && player.GetX() == chestLockX && player.GetY() == chestLockY)
    {
        ChestInteraction();
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

        // if the player is front the chest
        if (!chestOpened && player.GetX() == chestLockX && player.GetY() == chestLockY)
        {
            ChestInteraction();
        }

        map.DrawWithPlayer(mapWidth, mapHeight, player.GetX(), player.GetY(), offsetX, offsetY); // for the map

        if (_kbhit())
        {
            input = _getch();
            ProcessInput(input); // the movement

            if (input == 27) // ESC para salir
                isRunning = false;
        }


        // #ifdef _WIN32
        Sleep(20);
        // #endif
    }

    return true;
}

#endif

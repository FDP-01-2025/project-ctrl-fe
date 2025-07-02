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
    bool beginningEvent = true; // for the first message
    bool moveAllow = false;

    const int mapWidth = 35;
    const int mapHeight = 10;
    const int chestX[5] = {15, 16, 17, 18, 19}; // Chest location
    const int chestY[2] = {1, 2};
    const int chestLockX = 17; // front the chest
    const int chestLockY = 3;

    int offsetX = 17; // to indicate where messages begin, comes from the half of the map width
    int offsetY = 1;

    char input;

    Utils utils;
    Map map;
    Player player;

    void ProcessInput(char input);
    void ChestInteraction();
    void WaitKey();
    void LoadMap();
    void Beginning();
};

inline ChestEvent::ChestEvent() {};

inline void ChestEvent::WaitKey()
{
    utils.MoveCursor(offsetX, offsetY + 11);
    std::wcout << L"Press any key to continue...";
    _getch();
    utils.MoveCursor(offsetX, offsetY + 11);
    std::wcout << L"                             "; // for cleaning
}

void ClearKeyboardBuffer() // to avoid saving inputs when moveAllow is false
{
    while (_kbhit())
        _getch();
}

inline void ChestEvent::ChestInteraction() // asking the player to open the chest
{
    chestOpened = true; // changes the condition
    moveAllow = false;

    utils.MoveCursor(offsetX + 3, offsetY - 2);
    std::wcout << L"                      "; // for cleaning (Press 'Y'...)
    utils.MoveCursor(offsetX - 5, offsetY - 3);
    std::wcout << L"You opened the chest and gained an extra life!!";
    DrawDialogue(L"+1 life", offsetX + 12, offsetY + 1);
    Sleep(2000);
    ClearKeyboardBuffer();
    WaitKey(); // this is for waiting

    utils.MoveCursor(offsetX - 5, offsetY - 3); // Clear the message
    std::wcout << L"                                               ";
    ClearDialogue(offsetX + 12, offsetY + 1, 7); // clear the item dialogue

    // for cleaning the chest area after it's opened
    for (int i = 0; i < 2; i++)
    {

        for (int j = 0; j < 5; j++)
        {
            map.SetTile(chestX[j], chestY[i], ' ');
        }
    }
    DrawDialogue(L"It seems the chest has disappeared...", offsetX, offsetY + 1);
    Sleep(2000);
    ClearKeyboardBuffer();
    WaitKey();
    ClearDialogue(offsetX, offsetY + 1, 37);

    moveAllow = true;
}

inline void ChestEvent::Beginning() // the introduction to the room
{
    DrawDialogue(L"There's a chest over that kind of altar. Perhaps something lies within... ", offsetX, offsetY + 10);
    DrawDialogue(L"Shall I break its seal and look inside?", offsetX, offsetY + 12);
    Sleep(3000);
    ClearDialogue(offsetX, offsetY + 10, 74);
    ClearDialogue(offsetX, offsetY + 12, 39);
    ClearKeyboardBuffer();
    moveAllow = true;
}

inline void ChestEvent::ProcessInput(char input)
{
    if (moveAllow)
    {
        std::pair<int, int> dir = player.GetInputDirection(input);
        int dx = dir.first;
        int dy = dir.second;

        if (dx == 0 && dy == 0)
            return;

        int newX = player.GetX() + dx;
        int newY = player.GetY() + dy;

        wchar_t tile = map.GetTile(newX, newY);

        if (tile == ' ' || tile == '/' || tile == '-')
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

            // if the player is front the chest
            if (!chestOpened && player.GetX() == chestLockX && player.GetY() == chestLockY)
            {
                utils.MoveCursor(offsetX + 3, offsetY - 3);
                std::wcout << L"Do you want to open the chest?";
                utils.MoveCursor(offsetX + 3, offsetY - 2);
                std::wcout << L"(Press 'Y' to open it)";
            }
            else
            {
                utils.MoveCursor(offsetX + 3, offsetY - 3);
                std::wcout << L"                              ";
                utils.MoveCursor(offsetX + 3, offsetY - 2);
                std::wcout << L"                      ";
            }
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

inline bool ChestEvent::Run() //call this in main.cpp 
{
    player.ResetState(Player::NORMAL);
    LoadMap();
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithPlayer(mapWidth, mapHeight, player.GetX(), player.GetY(), offsetX, offsetY); // for the map

        if (beginningEvent) // for the first dialogue
        {
            Beginning();
            beginningEvent = false;
        }

        if (_kbhit())
        {
            input = _getch();
            ProcessInput(input); // the movement

            if (input == 27) // ESC para salir
                isRunning = false;
        }

        if (!chestOpened && input == 'y' && player.GetX() == chestLockX && player.GetY() == chestLockY)
        {
            ChestInteraction();
        }
    }

    return true;
}

#endif

#ifndef MAIN_GENIOUS_LAMP_H
#define MAIN_GENIOUS_LAMP_H

#include "utils/functions/utils.h"
#include "core/modules/hud/hudGenius.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"

#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

class MainGenious
{
public:
    MainGenious();
    bool Run();

private:
    bool isRunning, lampEventActive = true;
    int activeMap;
    std::string mapGeniusArchive;
    int offsetX = 1, offsetY = 1;
    bool geniusActivated = false;
    const int lampX = 17, lampY = 6;

    Utils utils;
    Map map;
    Player player;
    HUDGenie hud;

    void ActiveMapGenius();
    int GenerateGeniusType(int difficulty);
    void processInput(char input);
};

inline MainGenious::MainGenious() : isRunning(true), activeMap(0) {}

inline void MainGenious::ActiveMapGenius()
{
    std::string basePath = utils.GetAssetsPath() + "maps\\geniuos-lamp\\";
    mapGeniusArchive = basePath + ((activeMap == 0) ? "geniuosLamp.txt" : "lamp.txt");
}

// Returns 1 if Genie is good, 0 if evil based on difficulty
inline int MainGenious::GenerateGeniusType(int difficulty)
{
    int random = rand() % 100;
    return random < (difficulty == 0 ? 70 : (difficulty == 1 ? 50 : 30)) ? 1 : 0;
}

// Handles player input to move
inline void MainGenious::processInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx, newY = player.GetY() + dy;
    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == '/')
    {
        if (tile == '/')
        {
            utils.ClearScreen();
            isRunning = false;
            lampEventActive = false;
        }
        else
            player.Move(dx, dy, tile);
    }
}

// Main execution logic of the Genie event
// This function handles both the lamp interaction and the genie appearance
// We declare it as inline to suggest that it can be optimized by the compiler
// This is a performance optimization hint, not a strict requirement
inline bool MainGenious::Run()
{
    srand(time(0));
    bool insideGeniusRoom = player.GetEvent() == 1;

    ActiveMapGenius();
    if (!insideGeniusRoom)
        mapGeniusArchive = utils.GetAssetsPath() + "maps\\geniuos-lamp\\lamp.txt";

    map.ReadMap(mapGeniusArchive, 60, 50);
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Calculate map draw offset
    offsetX = std::max(0, (utils.GetConsoleWidth() - map.GetWidth()) / 4);
    offsetY = std::max(0, (utils.GetConsoleHeight() - map.GetHeight()) / 2);

    // First phase: interact with the lamp
    while (lampEventActive)
    {
        utils.ClearScreen();
        if (player.GetX() == lampX && player.GetY() == lampY)
        {
            int msgX = std::max(0, offsetX + map.GetWidth() / 2 - 20);
            utils.MoveCursor(msgX, offsetY - 2);
            std::wcout << L"Do you want to rub the lamp? Press [i] to interact";
        }
        else
        {
            int msgX = std::max(0, offsetX + map.GetWidth() / 2 - 20);
            utils.MoveCursor(msgX, offsetY - 2);
            std::wcout << L"                                                   ";
        }

        map.DrawWithPlayerLamp(60, 50, player.GetX(), player.GetY(), offsetX, offsetY);
        hud.SetCenteredOffset(offsetX); // Center based on the map offset
        hud.DrawLampEvent(player, player.GetRoom(), map.GetWidth());

        if (_kbhit())
        {
            char input = _getch();
            if (input == 'i' && player.GetX() == lampX && player.GetY() == lampY)
            {
                lampEventActive = false; // End the lamp event
                geniusActivated = true;
                break;
            }
            processInput(input);
        }

        Sleep(20);
    }

    utils.ClearScreenComplety();

    // Second phase: Genie appears
    if (geniusActivated)
    {
        ActiveMapGenius();
        map.ReadMap(mapGeniusArchive, 60, 50);

        bool interacting = true;
        while (interacting)
        {
            utils.ClearScreen();
            map.DrawWithPlayerGenius(60, 50, player.GetX(), player.GetY(), offsetX, offsetY);
            hud.SetCenteredOffset(offsetX); // Central based on the map offset
            hud.DrawGeniusEvent(player, player.GetRoom(), map.GetWidth());

            int msgX = std::max(0, offsetX + map.GetWidth() / 2 - 20);
            utils.MoveCursor(msgX, offsetY - 2);
            std::wcout << L"You have awakened the genie!";

            utils.MoveCursor(offsetX, offsetY + map.GetHeight() + 1);
            std::wcout << L"Press [e] to choose a wish or walk away...";

            if (_kbhit())
            {
                char input = _getch();
                if (input == 'e')
                {
                    utils.ClearScreen();
                    map.DrawWithPlayerGenius(60, 50, player.GetX(), player.GetY(), offsetX, offsetY);
                    hud.SetCenteredOffset(offsetX);
                    hud.DrawGeniusEvent(player, player.GetRoom(), map.GetWidth());

                    std::wstring options[] = {
                        L"1. Skip 2 levels",
                        L"2. Gain 3 extra lives",
                        L"3. Gain 3 extra bombs"};

                    int wishY = offsetY + map.GetHeight() + 2;
                    // Loop through each element in the 'options' container using a range-based for loop
                    // Process each 'opt' element here
                    for (const auto &opt : options)
                        utils.MoveCursor(offsetX, wishY++), std::wcout << opt;

                    utils.MoveCursor(offsetX, wishY++);
                    std::wcout << L"Choose your wish (1-3): ";
                    int choice = 0;
                    std::cin >> choice;

                    if (choice < 1 || choice > 3)
                    {
                        utils.MoveCursor(offsetX, wishY++);
                        std::wcout << L"The genie got angry because you didn't choose any wish.";
                        utils.MoveCursor(offsetX, wishY++);
                        std::wcout << L"Press any key to continue...";
                        _getch();
                        break;
                    }

                    utils.ClearScreenComplety();

                    int geniusType = GenerateGeniusType((int)player.GetDifficulty());
                    std::wstring result = (geniusType == 0)
                                              ? L"The genie laughs... He was evil!"
                                              : L"The genie nods with a smile. Your wish is granted.";

                    switch (choice)
                    {
                    case 1:
                        player.SetRoom(player.GetRoom() + (geniusType ? 2 : -2));
                        break;
                    case 2:
                        geniusType ? player.IncrementLives(3) : player.DecrementLives(3);
                        break;
                    case 3:
                        geniusType ? player.IncrementBombs(3) : player.DecrementBombs(3);
                        break;
                    }

                    // Show final result
                    bool waiting = true;
                    while (waiting)
                    {
                        utils.ClearScreen();
                        map.DrawWithPlayerGenius(60, 50, player.GetX(), player.GetY(), offsetX, offsetY);
                        hud.SetCenteredOffset(offsetX);
                        hud.DrawGeniusEvent(player, player.GetRoom(), map.GetWidth());

                        int msgY = offsetY + map.GetHeight() + 2;
                        utils.MoveCursor(offsetX, msgY++);
                        std::wcout << L"You chose: " << options[choice - 1];
                        utils.MoveCursor(offsetX, msgY++);
                        std::wcout << result;
                        utils.MoveCursor(offsetX, msgY++);
                        std::wcout << L"Press any key to continue...";

                        if (_kbhit())
                            _getch(), waiting = false;
                        Sleep(20);
                    }

                    interacting = false;
                }
                else
                    processInput(input);
            }

            Sleep(20);
        }
    }

    utils.ClearScreenComplety();

    // Final phase: empty room, exit through the door
    mapGeniusArchive = utils.GetAssetsPath() + "maps\\geniuos-lamp\\emptyRoom.txt";
    map.ReadMap(mapGeniusArchive, 60, 50);

    while (isRunning)
    {

        int msgX = std::max(0, offsetX + map.GetWidth() / 2 - 20);
        utils.MoveCursor(msgX, offsetY - 2);
        std::wcout << L"The genie disappeared, leaving you in an empty room.";

        utils.ClearScreen();
        map.DrawWithPlayer(60, 50, player.GetX(), player.GetY(), offsetX, offsetY);
        hud.SetCenteredOffset(offsetX);
        hud.DrawRoomEmptyEvent(player, player.GetRoom(), map.GetWidth());

        if (_kbhit())
        {
            char input = _getch();
            processInput(input);

            if (map.GetTile(player.GetX(), player.GetY()) == '/')
            {
                utils.ClearScreen();
                isRunning = false;
            }
        }

        Sleep(20);
    }

    return true;
}

#endif

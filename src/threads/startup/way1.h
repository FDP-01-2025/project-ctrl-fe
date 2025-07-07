#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/hud/hudWay.h"
#include <mmsystem.h>             // Play sound
#pragma comment(lib, "winmm.lib") // Link with sound library

class Way1
{
public:
    Way1();
    bool Run(Console consoleSettings);

private:
    Map map;
    Utils utils;
    Player player;
    HudWay hud;

    int viewW;
    int consoleW;
    int playerX;
    int playerY;

    int offsetX = 1, offsetY = 1; // Offset for drawing the map

    bool isRunning;

    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
    void SetGoodStyle(Console consoleSettings);
    void ShowLoadingAnimation(Console consoleSettings, int cycles = 3, int delay = 300);
    std::string GetPath();
};

Way1::Way1() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth(); // Get console width at initialization
}

bool Way1::Run(Console consoleSettings)
{
    system("cls");  // Clear console screen
    Sleep(10);
    SetGoodStyle(consoleSettings);  // Set font and show loading animation

    std::string key = utils.GetAssetsPath() + "maps\\main\\way1.txt";
    LoadLevel(key);  // Load the level/map from file

    player.SetPosition(3, 5); // Set initial player position

    // Play music in loop asynchronously
    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\SilentHill2.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (isRunning)
    {
        viewW = 34;             // Width of viewport to draw map slice
        utils.ClearScreen();    // Clear the console

        // Draw map with a window view centered on player, with offsets
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY, FrstWay);

        hud.Draw(player, 1, viewW); // Draw HUD with player info and viewport width

        // If a key is pressed, process it
        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(15);  // Small delay to reduce CPU load
    }

    PlaySoundW(NULL, NULL, 0); // Stop sound playback

    return true;
}

void Way1::ProceesInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key pressed, do nothing

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == L'-')
    {
        map.SetTile(newX, newY, L'|'); // Automatically "open" the door by changing tile
        // Note: need logic to revert tile when moving away later
    }

    // Only allow moving into empty space ' ' or 's' tile
    if (tile == ' ' || tile == 's')
    {
        player.SetPosition(newX, newY);
    }

    // If player reaches tile 'P' (presumably exit)
    if (tile == 'P')
    {
        system("cls");   // Clear console
        utils.ClearScreen();
        isRunning = false;  // End the game loop
        return;
    }
}

void Way1::LoadLevel(std::string key)
{
    utils.ClearScreenComplety();  // Clear entire console

    // Read the map file into the map object
    map.ReadMap(key, map.GetWidth(), map.GetHeight());

    // Set player's initial position to the spawn point defined in the map
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Sync player's position variables
    playerX = player.GetX();
    playerY = player.GetY();

    // Set viewport width based on console width and map width
    viewW = utils.GetConsoleWidth();
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();

    // Center vertically the map on console if possible
    int consoleHeight = utils.GetConsoleHeight();
    offsetY = (consoleHeight - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}

void Way1::ShowLoadingAnimation(Console consoleSettings, int cycles, int delay)
{
    consoleSettings.SetConsoleFont(19, 25, L"Lucida Console");  // Set font size for loading animation
    consoleSettings.SetColor(FOREGROUND_RED);

    std::wstring baseText = L"Loading";
    std::wstring dots[] = {L".", L"..", L"..."};  // Different dot animations

    for (int i = 0; i < cycles * 3; ++i)
    {
        std::wstring text = baseText + dots[i % 3];

        utils.ClearScreen();
        utils.PrintCentered(text);  // Print the loading text centered on screen

        Sleep(delay);
    }

    system("cls");
}

void Way1::SetGoodStyle(Console consoleSettings)
{
    ShowLoadingAnimation(consoleSettings); // Show loading animation first
    Sleep(300);
    consoleSettings.SetConsoleFont();      // Reset font to default
    Sleep(300);
    consoleSettings.SetConsoleFont(22, 25, L"Lucida Console"); // Set preferred game font size
    Sleep(100);
}

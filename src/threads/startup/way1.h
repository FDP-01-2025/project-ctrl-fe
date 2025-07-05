#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"

class Way1
{
public:
    Way1();
    bool Run(Console consoleSettings);

private:
    Map map;
    Utils utils;
    Player player;

    int viewW;
    int consoleW;
    int playerX;
    int playerY;

    int offsetX = 1, offsetY = 1; // Map drawing offset

    bool isRunning;
    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
    void SetGoodStyle(Console consoleSettings);
    void ShowLoadingAnimation(Console consoleSettings, int c, int d);
    std::string GetPath();
};

Way1::Way1() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

bool Way1::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings);
    std::string key = utils.GetAssetsPath() + "maps\\main\\way1.txt";
    LoadLevel(key);
    player.SetPosition(3, 5);
    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY, FrstWay);

        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(15);
    }
    return true;
}

void Way1::ProceesInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == L'-')
    {
        map.SetTile(newX, newY, L'|'); // Abrir automáticamente
    } // Si yo se, falta que al alejarse regrese a como antes, paciencia

    // Solo permitir moverse a espacios vacíos o puerta
    if (tile == ' ' || tile == 's')
    {
        player.SetPosition(newX, newY);
    }

    if (tile == 'P')
    {
        system("cls");
        utils.ClearScreen();
        isRunning = false;
        return;
    }
}

void Way1::LoadLevel(std::string key)
{
    // Clear screen and load map if found
    utils.ClearScreenComplety();
    // We call ReadMap from the Map class to load the map
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    playerX = player.GetX(); // sincronizar por si acaso
    playerY = player.GetY();

    viewW = utils.GetConsoleWidth();
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();

    int consoleHeight = utils.GetConsoleHeight();
    offsetY = (consoleHeight - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}

void Way1::ShowLoadingAnimation(Console consoleSettings, int cycles = 3, int delay = 300)
{
    consoleSettings.SetConsoleFont(19, 25, L"Lucida Console");
    consoleSettings.SetColor(FOREGROUND_RED);

    std::wstring baseText = L"Loading";
    std::wstring dots[] = {L".", L"..", L"..."};

    for (int i = 0; i < cycles * 3; ++i)
    {
        std::wstring text = baseText + dots[i % 3];

        utils.ClearScreen();
        utils.PrintCentered(text);

        Sleep(delay);
    }

    system("cls");
}

void Way1::SetGoodStyle(Console consoleSettings)
{
    ShowLoadingAnimation(consoleSettings);
    Sleep(300);
    consoleSettings.SetConsoleFont();
    Sleep(300);
    consoleSettings.SetConsoleFont(25, 25, L"Lucida console");
    Sleep(100);
}

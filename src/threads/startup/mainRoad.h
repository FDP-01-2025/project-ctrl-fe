#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"

class MainRoadGame
{
public:
    MainRoadGame();
    bool Run();

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
    void ProceesInput(char input);
    void LoadLevel(std::string key);
};

MainRoadGame::MainRoadGame() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW / 2;
}

bool MainRoadGame::Run()
{
    std::string key = utils.GetAssetsPath() + "maps\\main\\mainRoad.txt";
    LoadLevel(key);
    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY);

        if (_kbhit())
            ProceesInput(_getch());

        Sleep(50);
    }

    return true;
}

void MainRoadGame::ProceesInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    // Solo permitir moverse a espacios vacíos o puerta
    if (tile == ' ' || tile == '/')
    {
        if (tile == '/')
        {
            utils.ClearScreen();
            std::wcout << L"\n¡Has llegado a la puerta!\n";
            isRunning = false;
            return;
        }

        player.SetPosition(newX, newY);
    }
}

void MainRoadGame::LoadLevel(std::string key)
{
    // Clear screen and load map if found
    utils.ClearScreenComplety();
    // We call ReadMap from the Map class to load the map
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    playerX = player.GetX(); // sincronizar por si acaso
    playerY = player.GetY();

    // Centrar horizontalmente el mapa
    int consoleWidth = utils.GetConsoleWidth();
    offsetX = (consoleWidth - map.GetWidth()) / 4;
    if (offsetX < 0)
        offsetX = 0;

    int consoleHeight = utils.GetConsoleHeight();
    offsetY = (consoleHeight - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;
}
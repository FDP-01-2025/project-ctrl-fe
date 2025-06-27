#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"

class MainRoadGame
{
public:
    MainRoadGame();
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
    void AnimateAutomaticDoor(int x, int y);
};

MainRoadGame::MainRoadGame() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

bool MainRoadGame::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings);
    std::string key = utils.GetAssetsPath() + "maps\\main\\mainRoad.txt";
    LoadLevel(key);
    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY);

        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(50);
    }
    consoleSettings.SetConsoleFont();
    return true;
}

void MainRoadGame::ProceesInput(char input, Console consoleSettings)
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
    if (tile == ' ' || tile == '|')
    {
        if (tile == '|')
        {
            system("cls");
            Sleep(300);
            consoleSettings.SetConsoleFont();
            Sleep(300);
            consoleSettings.SetConsoleFont(14, 20, L"Lucida Console");
            utils.PrintCentered(L"Seguro que quieres entrar? S = si, N = no");
            // Leer tecla
            while (true)
            {
                char tecla = _getch();
                if (tecla == 's' || tecla == 'S')
                {
                    system("cls");
                    Sleep(100);
                    utils.PrintCentered(L"Has decidido entrar.");
                    Sleep(1000);
                    system("cls");
                    Sleep(500);
                    isRunning = false;
                    return;
                }
                else if (tecla == 'n' || tecla == 'N')
                {
                    system("cls");
                    Sleep(100);
                    utils.PrintCentered(L"Has decidido no entrar.");
                    Sleep(1000);
                    system("cls");
                    consoleSettings.SetConsoleFont();
                    Sleep(100);
                    consoleSettings.SetConsoleFont(23, 28, L"Lucida console");
                    Sleep(100);
                    break;
                    // poner logica de volver a recargar el mapa con su posición anterior
                }
            }
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

    viewW = utils.GetConsoleWidth();
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();
}

void MainRoadGame::ShowLoadingAnimation(Console consoleSettings, int cycles = 3, int delay = 300)
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

void MainRoadGame::SetGoodStyle(Console consoleSettings)
{
    ShowLoadingAnimation(consoleSettings);
    Sleep(300);
    consoleSettings.SetConsoleFont();
    Sleep(300);
    consoleSettings.SetConsoleFont(23, 28, L"Lucida console");
    Sleep(100);
}

void MainRoadGame::AnimateAutomaticDoor(int x, int y)
{
    for (int i = 0; i < 2; ++i)
    {
        map.SetTile(x, y + i, L' ');
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY);
        Sleep(100);
    }
}
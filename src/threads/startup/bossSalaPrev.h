#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"

class BossSalaPrev
{
public:
    BossSalaPrev();
    MapId Run(Console consoleSettings, MapId opciones[3]);

private:
    Map map;
    Utils utils;
    Player player;

    int viewW;
    int consoleW;
    int playerX;
    int playerY;
    MapId selection;
    MapId opcionesBoses[2];

    int offsetX = 1, offsetY = 1; // Map drawing offset

    bool isRunning;
    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
};

BossSalaPrev::BossSalaPrev() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

MapId BossSalaPrev::Run(Console consoleSettings, MapId opciones[2])
{
    for (int i = 0; i < 3; ++i)
    {
        opcionesBoses[i] = opciones[i];
    }
    std::string key = utils.GetAssetsPath() + "maps\\main\\boosSalaPrev.txt";
    LoadLevel(key);
    player.SetPosition(3, 7);
    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY, MapId::MainRoom);

        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(15);
    }
    return selection;
}

void BossSalaPrev::ProceesInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == L'1' || tile == L'2')
    {
        if (tile == L'1')
        {
            selection = opcionesBoses[0];
        }
        if (tile == L'2')
        {
            selection = opcionesBoses[1];
        }

        map.SetTile(newX, newY, L'O'); // Abrir automáticamente
        Sleep(500);
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
                player.SetPosition(player.GetX() - 3, player.GetY());
                Sleep(100);
                map.SetTile(newX, newY, L'D');
                break;
                // poner logica de volver a recargar el mapa con su posición anterior
            }
        }
    }

    // Solo permitir moverse a espacios vacíos o puerta
    if (tile == ' ' || tile == '-')
    {
        player.SetPosition(newX, newY);
    }
}

void BossSalaPrev::LoadLevel(std::string key)
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

#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "utils/functions/toWstring.h"

class MainRoomGame
{
public:
    MainRoomGame();
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
    MapId opcionesGames[3];
    int totalOpciones = 0;

    int offsetX = 1, offsetY = 1; // Map drawing offset

    bool isRunning;
    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
    std::wstring GetMapName(MapId id);
    void ReplaceDoorsName(MapId op[3]);
    void WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text);
};

MainRoomGame::MainRoomGame() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

MapId MainRoomGame::Run(Console consoleSettings, MapId opciones[3])
{
    totalOpciones = 0;
    for (int i = 0; i < 3; ++i)
    {
        opcionesGames[i] = opciones[i];
        if (opciones[i] != MapId::None)
            totalOpciones++;
    }

    std::string key = utils.GetAssetsPath() + "maps\\main\\mainRoom.txt";
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
    consoleSettings.SetConsoleFont();
    return selection;
}

void MainRoomGame::ReplaceDoorsName(MapId op[3])
{
    for (int i = 0; i < totalOpciones; ++i)
    {
        if (opcionesGames[i] == MapId::None)
            continue;

        std::wstring nombre = GetMapName(opcionesGames[i]);

        int fila;
        switch (i)
        {
        case 0:
            fila = 3;
            break;
        case 1:
            fila = 6;
            break;
        case 2:
            fila = 9;
            break;
        }

        WriteTextOnMap(map, fila, 44, nombre);
    }

    // Si no hay tercera opción, tapar puerta 3 (fila 11 y 12)
    if (totalOpciones < 3)
    {
        map.SetTile(60, 10, L'|');
        map.SetTile(60, 11, L'|');
    }

    // Si no hay segunda opción, tapar puerta 2 (fila 8 y 9)
    if (totalOpciones < 2)
    {
        map.SetTile(60, 7, L'|');
        map.SetTile(60, 8, L'|');
    }
}

void MainRoomGame::WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        map.SetTile(colStart + i, row, text[i]);
    }
}

void MainRoomGame::ProceesInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == L'1' || tile == L'2' || tile == L'3')
    {
        if (tile == L'1')
            selection = opcionesGames[0];
        if (tile == L'2')
            selection = opcionesGames[1];
        if (tile == L'3')
            selection = opcionesGames[2];

        Sleep(500);
        system("cls");
        consoleSettings.SetConsoleFont();
        Sleep(300);
        consoleSettings.SetConsoleFont(14, 20, L"Lucida Console");
        Sleep(500);
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
                Sleep(300);
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
                map.SetTile(newX, newY, tile);
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

void MainRoomGame::LoadLevel(std::string key)
{
    // Clear screen and load map if found
    utils.ClearScreenComplety();
    // We call ReadMap from the Map class to load the map
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    ReplaceDoorsName(opcionesGames);
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    playerX = player.GetX(); // sincronizar por si acaso
    playerY = player.GetY();

    viewW = utils.GetConsoleWidth();
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();
}

std::wstring MainRoomGame::GetMapName(MapId id)
{
    // MapId allGames[] = {BomberManGame, MazeGame, GeniusGame, WormGame, ElevatorGame};
    switch (id)
    {
    case BomberManGame:
        return L"Bomberman";
    case MazeGame:
        return L"Laberinto";
    case GeniusGame:
        return L"Genio";
    // case ChestGame: return L"Cofres";
    case WormGame:
        return L"Gusano";
    case ElevatorGame:
        return L"Ascensor";
    case BoosMario:
        return L"Jefe Mario";
    case BoosZelda:
        return L"Jefe Zelda";
    default:
        return L"Desconocido";
    }
}
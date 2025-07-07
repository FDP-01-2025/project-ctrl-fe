#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/hud/hudBossSala.h"
#include <mmsystem.h>             // Reproducir sonido
#pragma comment(lib, "winmm.lib") // Enlace con la librería de sonido

class BossSalaPrev
{
public:
    BossSalaPrev();
    MapId Run(Console consoleSettings, MapId opciones[3]);

private:
    Map map;
    Utils utils;
    Player player;
    HudBossSala hud;

    int viewW;
    int consoleW;
    int playerX;
    int playerY;
    MapId selection;
    MapId opcionesBoses[2];
    int totalOpciones = 0;

    int offsetX = 1, offsetY = 1; // Map drawing offset

    bool isRunning;
    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
    void ReplaceDoorsName(MapId op[2]);
    std::wstring GetMapName(MapId id);
    void WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text);
};

BossSalaPrev::BossSalaPrev() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

MapId BossSalaPrev::Run(Console consoleSettings, MapId opciones[2])
{
    isRunning = true;
    totalOpciones = 0;
    for (int i = 0; i < 3; ++i)
    {
        opcionesBoses[i] = opciones[i];
        if (opciones[i] != MapId::None)
            totalOpciones++;
    }
    Sleep(100);
    consoleSettings.SetConsoleFont(25, 25, L"Lucida console");
    Sleep(100);
    std::string key = utils.GetAssetsPath() + "maps\\main\\boosSalaPrev.txt";
    LoadLevel(key);
    player.SetPosition(3, 7);

    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\LookOut.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    while (isRunning)
    {
        viewW = 30;
        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY, MapId::MainRoom);
        hud.Draw(player, 1, viewW);

        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(15);
    }
    PlaySoundW(NULL, NULL, 0);
    consoleSettings.SetConsoleFont();
    return selection;
}

void BossSalaPrev::ReplaceDoorsName(MapId op[3])
{
    for (int i = 0; i < totalOpciones; ++i)
    {
        if (opcionesBoses[i] == MapId::None)
            continue;

        std::wstring nombre = GetMapName(opcionesBoses[i]);

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
}

void BossSalaPrev::WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        map.SetTile(colStart + i, row, text[i]);
    }
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

std::wstring BossSalaPrev::GetMapName(MapId id)
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
    case ChestGame:
        return L"Cofres";
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
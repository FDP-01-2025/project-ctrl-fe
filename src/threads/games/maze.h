#ifndef MAIN_MAZE_H
#define MAIN_MAZE_H

#include <vector>
#include "utils/functions/utils.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/modules/Maze/hudMaze.h"

#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>
#include <tuple>

#ifdef _WIN32
#include <windows.h>
#endif

class MainMaze
{
public:
    MainMaze();
    bool Run();

private:
    Map map;
    Player player;
    HUDMaze hud;
    Utils utils;
    std::string difficultyFolder;

    bool hasKey = false;
    bool isRunning = true;

    int offsetX = 2, offsetY = 1;
    std::string currentMapPath;
    int openedBoxes = 0;

    int VIEW_WIDTH = 30;
    int VIEW_HEIGHT = 20;

    void processInput(char input);
    void LoadLevel();
    void DetermineDifficultyFolder();
};

MainMaze::MainMaze() {}

bool MainMaze::Run()
{
    utils.ClearScreen();
    DetermineDifficultyFolder();
    LoadLevel();

    while (isRunning)
    {
        utils.ClearScreen();
        hud.Draw(player, hasKey, openedBoxes, VIEW_WIDTH);
        map.DrawViewportAroundPlayerMaze(
            player.GetX(), player.GetY(),
            VIEW_WIDTH, VIEW_HEIGHT,
            offsetX, offsetY);

        if (_kbhit())
            processInput(_getch());

        if (player.GetLives() <= 0)
        {
            utils.ClearScreen();
            std::wcout << L"\nYou have lost all your lives. Game Over!\n";
            isRunning = false;
            return false;
        }

        utils.Sleep(15);
    }

    return true;
}

void MainMaze::processInput(char input)
{
    // ✅ Apertura de cofre con tecla Q
    if (input == 'q' || input == 'Q')
    {
        wchar_t tile = map.GetTile(player.GetX(), player.GetY());

        if (tile == ']' || tile == '#') // Cofre cerrado
        {
            map.SetTile(player.GetX(), player.GetY(), '/'); // '/' = cofre abierto
            openedBoxes++;                                  // ← cuenta la caja abierta
            if (tile == '#')
            {
                hasKey = true;
                std::wcout << L"¡Has encontrado la llave dentro del cofre!\n";
            }
            else
            {
                std::wcout << L"El cofre estaba vacío.\n";
            }
            utils.Sleep(600);
            return;
        }

        else
        {
            std::wcout << L"No estás sobre un cofre.\n";
        }

        utils.Sleep(600);
        return;
    }

    // Movimiento
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == 'K' || tile == '/' || tile == ']' || tile == '#')
    {
        if (tile == 'K') // En caso sigas usando 'K'
        {
            hasKey = true;
            map.SetTile(newX, newY, ' ');
            utils.MoveCursor(offsetX + newX, offsetY + newY);
        }
        else if (tile == '/')
        {
            if (hasKey)
            {
                utils.ClearScreen();
                std::wcout << L"\n¡Felicidades! Encontraste la llave y saliste del laberinto.\n";
                isRunning = false;
                return;
            }
            else
            {
                std::wcout << L"\n¡Necesitas la llave para salir!\n";
                utils.Sleep(500);
            }
        }

        player.Move(dx, dy, tile);
    }
}

void MainMaze::LoadLevel()
{
    DetermineDifficultyFolder();

    currentMapPath = utils.GetAssetsPath() + "maps\\maze\\" + difficultyFolder;

    utils.ClearScreenComplety();
    map.ReadMap(currentMapPath, map.GetWidth(), map.GetHeight());
    player.SetPosition(1, 3);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int consoleWidth = 80;
    int consoleHeight = 25;

#ifdef _WIN32
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#endif

    offsetX = std::max(0, (consoleWidth - VIEW_WIDTH) / 2 - 10);
    offsetY = std::max(0, (consoleHeight - VIEW_HEIGHT) / 2);

    hud.SetCenteredOffset(offsetX);

    // ✅ Insertar cofres aleatorios
    int totalCofres = 5; // Cambia este número según la cantidad que quieras
    std::vector<std::pair<int, int>> position;

    for (int y = 0; y < map.GetHeight(); ++y)
    {
        for (int x = 0; x < map.GetWidth(); ++x)
        {
            if (map.GetTile(x, y) == ' ') // Solo lugares vacíos
                position.emplace_back(x, y);
        }
    }

    std::random_shuffle(position.begin(), position.end());

    for (int i = 0; i < totalCofres && i < position.size(); ++i)
    {
        int x = position[i].first;
        int y = position[i].second;
        map.SetTile(x, y, ']'); // ']': caja cerrada
    }

    if (!position.empty())
    {
        int keyX = position[0].first;
        int keyY = position[0].second;
        map.SetTile(keyX, keyY, '#'); // '#' = caja cerrada con llave
    }
}

void MainMaze::DetermineDifficultyFolder()
{
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 20;
        difficultyFolder = "maze1.txt";
        break;
    case Player::Difficulty::NORMAL:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 16;
        difficultyFolder = "maze2.txt";
        break;
    case Player::Difficulty::HARD:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 20;
        difficultyFolder = "maze.txt";
        break;
    }
}

#endif

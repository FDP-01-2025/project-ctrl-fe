#ifndef MAIN_MAZE_H
#define MAIN_MAZE_H

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
        hud.Draw(player, hasKey, VIEW_WIDTH);
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
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == 'K' || tile == '/' || tile == ']')
    {
        if (tile == 'K')
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

    hud.SetCenteredOffset(offsetX);
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

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
    // HUDMaze hud;
    Utils utils;
    std::string difficultyFolder;

    bool hasKey = false;
    bool isRunning = true;

    int offsetX = 1, offsetY = 1;
    std::string currentMapPath;

    void processInput(char input);
    void LoadLevel();
    void DetermineDifficultyFolder(); // Set folder based on difficulty
};
MainMaze::MainMaze() {}

bool MainMaze::Run()
{
    utils.ClearScreen();
    player.ActivateControlB(true); // Enable 'B' control for placing bombs
    DetermineDifficultyFolder();   // Set level folder based on difficulty
    LoadLevel();

    while (isRunning)
    {
        utils.ClearScreen();
        map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), player.GetX(), player.GetY(), offsetX, offsetY);
        // hud.DrawMaze(player, hasKey);

        if (_kbhit())
            processInput(_getch());

        // If player has no more lives, end game
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

    if (tile == ' ' || tile == 'K' || tile == '/')
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
    // Call the function to set the difficulty folder based on player state
    DetermineDifficultyFolder();

    currentMapPath = utils.GetAssetsPath() + "maps\\maze\\" + "maze.txt";

    utils.ClearScreenComplety();
    map.ReadMap(currentMapPath, map.GetWidth(), map.GetHeight());
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    int consoleWidth = utils.GetConsoleWidth();
    offsetX = (consoleWidth - map.GetWidth()) / 4;
    if (offsetX < 0)
        offsetX = 0;

    int consoleHeight = utils.GetConsoleHeight();
    offsetY = (consoleHeight - map.GetHeight()) / 2;
    if (offsetY < 0)
        offsetY = 0;

    // hud.SetCenteredOffset(offsetX);
}

// Set difficulty folder string based on selected difficulty
void MainMaze::DetermineDifficultyFolder()
{
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        difficultyFolder = "easy-levels";
        break;
    case Player::Difficulty::NORMAL:
        difficultyFolder = "medium-levels";
        break;
    case Player::Difficulty::HARD:
        difficultyFolder = "hard-levels";
        break;
    }
}

#endif

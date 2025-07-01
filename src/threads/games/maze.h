#ifndef MAIN_MAZE_H
#define MAIN_MAZE_H

#include "utils/functions/utils.h"
#include "utils/player/maping.h"
#include "utils/player/player.h"
#include "core/modules/Maze/hudMaze.h"
#include "utils/screen/colors.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>
#include <tuple>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_POSITIONS 1000

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

    std::pair<int, int> keyBoxPosition = {-1, -1};

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
    if (input == 'q' || input == 'Q')
    {
        wchar_t tile = map.GetTile(player.GetX(), player.GetY());

        if (tile == L'█')
        {
            map.SetTile(player.GetX(), player.GetY(), L'░');
            openedBoxes++;

            if (player.GetX() == keyBoxPosition.first && player.GetY() == keyBoxPosition.second)
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
            utils.Sleep(600);
            return;
        }
    }

    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);

    if (tile == ' ' || tile == 'K' || tile == L'░' || tile == ']' || tile == L'█')
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

    // ✅ Cofres aleatorios
    int totalCofres = 5;
    int validCount = 0;
    int posX[MAX_POSITIONS];
    int posY[MAX_POSITIONS];

    for (int y = 0; y < map.GetHeight(); ++y)
    {
        for (int x = 0; x < map.GetWidth(); ++x)
        {
            if (map.GetTile(x, y) == ' ' && validCount < MAX_POSITIONS)
            {
                posX[validCount] = x;
                posY[validCount] = y;
                ++validCount;
            }
        }
    }

    bool used[MAX_POSITIONS] = {false};
    int selectedCount = 0;
    int cofreX[totalCofres];
    int cofreY[totalCofres];

    srand(time(nullptr));

    while (selectedCount < totalCofres && selectedCount < validCount)
    {
        int index = rand() % validCount;
        if (!used[index])
        {
            used[index] = true;
            int x = posX[index];
            int y = posY[index];
            map.SetTile(x, y, L'█');

            cofreX[selectedCount] = x;
            cofreY[selectedCount] = y;
            ++selectedCount;
        }
    }

    if (selectedCount > 0)
    {
        // Intentar hasta 10 veces encontrar un cofre cerrado para la llave,
        // o seleccionar uno aleatorio si no hay 5 cofres cerrados disponibles.
        const int MIN_CLOSED_CHEKED = 5; // al menos 5 cofres para revisar
        int attempts = 0;
        int keyIndex = -1;

        while (attempts < 10)
        {
            int randomIndex = rand() % selectedCount;
            int x = cofreX[randomIndex];
            int y = cofreY[randomIndex];
            wchar_t tile = map.GetTile(x, y);

            if (tile == L'█') // cofre cerrado
            {
                keyIndex = randomIndex;
                break;
            }
            attempts++;
        }

        // Si no encontró cofre cerrado tras intentos, asignar cualquiera (puede estar abierto)
        if (keyIndex == -1)
            keyIndex = rand() % selectedCount;

        keyBoxPosition = std::make_pair(cofreX[keyIndex], cofreY[keyIndex]);
    }

    // 🌿 Área verde (esquina inferior derecha)
    for (int y = map.GetHeight() - 4; y < map.GetHeight() - 1; ++y)
    {
        for (int x = map.GetWidth() - 6; x < map.GetWidth() - 2; ++x)
        {
            if (map.GetTile(x, y) == ' ')
                map.SetTile(x, y, L'♣');
        }
    }

    // 🍽️ Mesas decorativas (T)
    int mesaX[] = {3, map.GetWidth() - 4, 3, map.GetWidth() - 4};
    int mesaY[] = {2, 2, map.GetHeight() - 3, map.GetHeight() - 3};

    for (int i = 0; i < 4; ++i)
    {
        if (map.GetTile(mesaX[i], mesaY[i]) == ' ')
            map.SetTile(mesaX[i], mesaY[i], L'T');
    }

    // 🔥 Antorchas (solo si hay espacio a los lados)
    for (int x = 2; x < map.GetWidth() - 2; x += 10)
    {
        for (int y = 2; y < map.GetHeight() - 2; y += 5)
        {
            // Verificar que haya espacio a los lados para evitar solapamientos
            if (map.GetTile(x, y) == ' ' &&
                map.GetTile(x - 1, y) == ' ' &&
                map.GetTile(x + 1, y) == ' ' &&
                map.GetTile(x, y - 1) == ' ' &&
                map.GetTile(x, y + 1) == ' ')
            {
                map.SetTile(x, y, L'*');
            }
        }
    }

    // 🌊 Laguito decorativo (solo sobre zonas vacías y alejadas de muros)
    int lakeX = map.GetWidth() / 4;
    int lakeY = map.GetHeight() / 2;

    bool canPlaceLake = true;
    for (int y = lakeY; y < lakeY + 3 && canPlaceLake; ++y)
    {
        for (int x = lakeX; x < lakeX + 6; ++x)
        {
            wchar_t tile = map.GetTile(x, y);
            if (tile != ' ')
            {
                canPlaceLake = false;
                break;
            }
        }
    }

    if (canPlaceLake)
    {
        for (int y = lakeY; y < lakeY + 3; ++y)
        {
            for (int x = lakeX; x < lakeX + 6; ++x)
            {
                map.SetTile(x, y, L'~');
            }
        }
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

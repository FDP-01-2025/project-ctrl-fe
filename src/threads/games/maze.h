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

#define MAX_POSITIONS 5000

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

    int totalCofres = 10; // default

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

        int messageRow = offsetY + VIEW_HEIGHT + 1; // Justo debajo de la ventana visible

        if (tile == L'█')
        {
            map.SetTile(player.GetX(), player.GetY(), L'░');
            openedBoxes++;

            std::wstring message;
            if (player.GetX() == keyBoxPosition.first && player.GetY() == keyBoxPosition.second)
            {
                hasKey = true;
                message = L"You found the key!";
            }
            else
            {
                message = L"The box is empty.";
            }

            // Limpiar línea antes de imprimir mensaje
            utils.MoveCursor(offsetX, messageRow);
            std::wcout << std::wstring(VIEW_WIDTH, L' ');

            // Imprimir mensaje centrado
            int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
            utils.MoveCursor(msgStartCol, messageRow);
            std::wcout << message << std::flush;

            utils.Sleep(600);
            return;
        }
        else
        {
            std::wstring message = L"You are not on a box.";

            utils.MoveCursor(offsetX, messageRow);
            std::wcout << std::wstring(VIEW_WIDTH, L' ');

            int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
            utils.MoveCursor(msgStartCol, messageRow);
            std::wcout << message << std::flush;

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

    if (tile == ' ' || tile == 'K' || tile == L'░' || tile == ']' || tile == L'█' || tile == 'E')
    {
        if (tile == 'K')
        {
            hasKey = true;
            map.SetTile(newX, newY, ' ');
            utils.MoveCursor(offsetX + newX, offsetY + newY);
        }
        else if (tile == 'E')
        {
            int messageRow = offsetY + VIEW_HEIGHT + 1; // igual que antes

            if (hasKey)
            {
                utils.ClearScreen();
                std::wstring message = L"Congratulations! You escaped the maze!";

                int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
                utils.MoveCursor(msgStartCol, messageRow);
                std::wcout << message << std::flush;

                utils.Sleep(1500);
                isRunning = false;
                return;
            }
            else
            {
                std::wstring message = L"You need the key to escape!";

                utils.MoveCursor(offsetX, messageRow);
                std::wcout << std::wstring(VIEW_WIDTH, L' ');

                int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
                utils.MoveCursor(msgStartCol, messageRow);
                std::wcout << message << std::flush;

                utils.Sleep(800);
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

    // ✅ Cofres garantizados en franjas
    int zones = 3;
    int cofresPorZona = totalCofres / zones;

    int selectedCount = 0;
    int cofreX[MAX_POSITIONS];
    int cofreY[MAX_POSITIONS];

    for (int z = 0; z < zones; ++z)
    {
        int zoneHeight = map.GetHeight() / zones;
        int yStart = z * zoneHeight;
        int yEnd = (z == zones - 1) ? map.GetHeight() : yStart + zoneHeight;

        int validX[MAX_POSITIONS], validY[MAX_POSITIONS], validCount = 0;

        for (int y = yStart + 1; y < yEnd - 1; ++y)
        {
            for (int x = 1; x < map.GetWidth() - 1; ++x)
            {
                if (map.GetTile(x, y) == ' ')
                {
                    validX[validCount] = x;
                    validY[validCount] = y;
                    ++validCount;
                }
            }
        }

        for (int i = validCount - 1; i > 0; --i)
        {
            int j = rand() % (i + 1);
            std::swap(validX[i], validX[j]);
            std::swap(validY[i], validY[j]);
        }

        int n = std::min(cofresPorZona, validCount);
        for (int i = 0; i < n && selectedCount < totalCofres; ++i)
        {
            map.SetTile(validX[i], validY[i], L'█');
            cofreX[selectedCount] = validX[i];
            cofreY[selectedCount] = validY[i];
            ++selectedCount;
        }
    }

    // Rellenar si faltan
    if (selectedCount < totalCofres)
    {
        int allX[MAX_POSITIONS], allY[MAX_POSITIONS], allCount = 0;
        for (int y = 1; y < map.GetHeight() - 1; ++y)
        {
            for (int x = 1; x < map.GetWidth() - 1; ++x)
            {
                if (map.GetTile(x, y) == ' ')
                {
                    allX[allCount] = x;
                    allY[allCount] = y;
                    ++allCount;
                }
            }
        }
        for (int i = 0; i < allCount && selectedCount < totalCofres; ++i)
        {
            map.SetTile(allX[i], allY[i], L'█');
            cofreX[selectedCount] = allX[i];
            cofreY[selectedCount] = allY[i];
            ++selectedCount;
        }
    }

    if (selectedCount > 0)
    {
        int keyIndex = 0;

        if (player.GetDifficulty() == Player::Difficulty::EASY)
        {
            // En fácil: totalmente aleatorio.
            keyIndex = rand() % selectedCount;
        }
        else
        {
            // En normal y difícil: favorece cofres de la mitad o zona baja.
            int biasAttempts = 10; // Más intentos para encontrar uno abajo
            for (int attempt = 0; attempt < biasAttempts; ++attempt)
            {
                int idx = rand() % selectedCount;
                int y = cofreY[idx];

                // Checa si está en la mitad baja
                if (y > map.GetHeight() / 2)
                {
                    keyIndex = idx;
                    break;
                }
            }

            // Si no encontró, elige cualquiera (fallback)
            if (keyIndex == 0)
            {
                keyIndex = rand() % selectedCount;
            }
        }

        keyBoxPosition = std::make_pair(cofreX[keyIndex], cofreY[keyIndex]);
    }

    // 🎋 Área verde, mesas, antorchas, laguito igual que antes
    for (int y = map.GetHeight() - 4; y < map.GetHeight() - 1; ++y)
        for (int x = map.GetWidth() - 6; x < map.GetWidth() - 2; ++x)
            if (map.GetTile(x, y) == ' ')
                map.SetTile(x, y, L'♣');

    int mesaX[] = {3, map.GetWidth() - 4, 3, map.GetWidth() - 4};
    int mesaY[] = {2, 2, map.GetHeight() - 3, map.GetHeight() - 3};
    for (int i = 0; i < 4; ++i)
        if (map.GetTile(mesaX[i], mesaY[i]) == ' ')
            map.SetTile(mesaX[i], mesaY[i], L'T');

    for (int x = 2; x < map.GetWidth() - 2; x += 10)
        for (int y = 2; y < map.GetHeight() - 2; y += 5)
            if (map.GetTile(x, y) == ' ' &&
                map.GetTile(x - 1, y) == ' ' &&
                map.GetTile(x + 1, y) == ' ' &&
                map.GetTile(x, y - 1) == ' ' &&
                map.GetTile(x, y + 1) == ' ')
                map.SetTile(x, y, L'*');

    int lakeX = map.GetWidth() / 4;
    int lakeY = map.GetHeight() / 2;
    bool canPlaceLake = true;
    for (int y = lakeY; y < lakeY + 3 && canPlaceLake; ++y)
        for (int x = lakeX; x < lakeX + 6; ++x)
            if (map.GetTile(x, y) != ' ')
                canPlaceLake = false;
    if (canPlaceLake)
        for (int y = lakeY; y < lakeY + 3; ++y)
            for (int x = lakeX; x < lakeX + 6; ++x)
                map.SetTile(x, y, L'~');
}

void MainMaze::DetermineDifficultyFolder()
{
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 20;
        difficultyFolder = "maze1.txt";
        totalCofres = 10;
        break;
    case Player::Difficulty::NORMAL:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 16;
        difficultyFolder = "maze2.txt";
        totalCofres = 20;
        break;
    case Player::Difficulty::HARD:
        VIEW_WIDTH = 30;
        VIEW_HEIGHT = 20;
        difficultyFolder = "maze.txt";
        totalCofres = 30;
        break;
    }
}

#endif

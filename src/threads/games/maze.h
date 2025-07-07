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

class MainMaze {
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

    int totalCofres = 10; // default number of chests

    std::pair<int, int> keyBoxPosition = {-1, -1}; // where the key will be

    void processInput(char input);
    void LoadLevel();
    void DetermineDifficultyFolder();
};

// Constructor
MainMaze::MainMaze() {}

// Main loop of the game
bool MainMaze::Run() {
    utils.ClearScreen();
    srand(static_cast<unsigned>(time(nullptr))); // Seed for randomness
    DetermineDifficultyFolder(); // Set map and difficulty settings
    LoadLevel(); // Load the map and decorate it

    while (isRunning) {
        utils.ClearScreen();
        hud.Draw(player, hasKey, openedBoxes, VIEW_WIDTH);
        map.DrawViewportAroundPlayerMaze(
            player.GetX(), player.GetY(),
            VIEW_WIDTH, VIEW_HEIGHT,
            offsetX, offsetY);

        if (_kbhit())
            processInput(_getch()); // Handle key press

        if (player.GetLives() <= 0) {
            utils.ClearScreen();
            std::wcout << L"\nYou have lost all your lives. Game Over!\n";
            isRunning = false;
            return false;
        }

        utils.Sleep(15);
    }

    return true;
}

// Handles user input
void MainMaze::processInput(char input) {
    if (input == 'q' || input == 'Q') {
        wchar_t tile = map.GetTile(player.GetX(), player.GetY());

        int messageRow = offsetY + VIEW_HEIGHT + 1; // Below the view

        if (tile == L'█') { // Box tile
            map.SetTile(player.GetX(), player.GetY(), L'░'); // Open it
            openedBoxes++;

            std::wstring message;
            if (player.GetX() == keyBoxPosition.first && player.GetY() == keyBoxPosition.second) {
                hasKey = true;
                message = L"You found the key!";
            } else {
                message = L"The box is empty.";
            }

            // Show message
            utils.MoveCursor(offsetX, messageRow);
            std::wcout << std::wstring(VIEW_WIDTH, L' ');

            int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
            utils.MoveCursor(msgStartCol, messageRow);
            std::wcout << message << std::flush;

            utils.Sleep(600);
            return;
        } else {
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

    // Allow movement if the tile is walkable
    if (tile == ' ' || tile == 'K' || tile == L'░' || tile == ']' || tile == L'█' || tile == 'E') {
        if (tile == 'K') {
            hasKey = true;
            map.SetTile(newX, newY, ' ');
        } else if (tile == 'E') {
            int messageRow = offsetY + VIEW_HEIGHT + 1;

            if (hasKey) {
                utils.ClearScreen();
                std::wstring message = L"Congratulations! You escaped the maze!";

                int msgStartCol = offsetX + (VIEW_WIDTH - (int)message.size()) / 2;
                utils.MoveCursor(msgStartCol, messageRow);
                std::wcout << message << std::flush;

                utils.Sleep(1500);
                isRunning = false;
                return;
            } else {
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

// Load and generate the level
void MainMaze::LoadLevel() {
    DetermineDifficultyFolder();
    currentMapPath = utils.GetAssetsPath() + "maps\\maze\\" + difficultyFolder;

    utils.ClearScreenComplety();
    map.ReadMap(currentMapPath, map.GetWidth(), map.GetHeight());
    player.SetPosition(1, 3);

    // Get current console size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int consoleWidth = 80;
    int consoleHeight = 25;

#ifdef _WIN32
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#endif

    // Center the viewport
    offsetX = std::max(0, (consoleWidth - VIEW_WIDTH) / 2 - 10);
    offsetY = std::max(0, (consoleHeight - VIEW_HEIGHT) / 2);
    hud.SetCenteredOffset(offsetX);

    // Get bounds of the walkable area (ignore walls)
    int minX = map.GetWidth(), maxX = 0;
    int minY = map.GetHeight(), maxY = 0;

    for (int y = 0; y < map.GetHeight(); ++y) {
        for (int x = 0; x < map.GetWidth(); ++x) {
            wchar_t tile = map.GetTile(x, y);
            if (tile != '#' && tile && tile != 0) {
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }

    minX = std::max(1, minX);
    minY = std::max(1, minY);
    maxX = std::min(map.GetWidth() - 2, maxX);
    maxY = std::min(map.GetHeight() - 2, maxY);

    // Helper to check if a point is inside the maze
    auto isInsideMaze = [&](int x, int y) {
        return x >= minX && x <= maxX && y >= minY && y <= maxY;
    };

    // Helper to check if we can place a chest
    auto isValidCofreSpot = [&](int x, int y) {
        if (!isInsideMaze(x, y)) return false;
        if (map.GetTile(x, y) != ' ') return false;

        int wallCount = 0;
        if (map.GetTile(x + 1, y) == '#') wallCount++;
        if (map.GetTile(x - 1, y) == '#') wallCount++;
        if (map.GetTile(x, y + 1) == '#') wallCount++;
        if (map.GetTile(x, y - 1) == '#') wallCount++;

        return wallCount <= 1;
    };

    // Place chests in different areas
    int zones = 3;
    int cofresPorZona = totalCofres / zones;
    int selectedCount = 0;
    int cofreX[MAX_POSITIONS], cofreY[MAX_POSITIONS];

    for (int z = 0; z < zones; ++z) {
        int zoneHeight = (maxY - minY) / zones;
        int yStart = minY + z * zoneHeight;
        int yEnd = (z == zones - 1) ? maxY : yStart + zoneHeight;

        int validX[MAX_POSITIONS], validY[MAX_POSITIONS], validCount = 0;

        for (int y = yStart; y <= yEnd; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                if (isValidCofreSpot(x, y)) {
                    validX[validCount] = x;
                    validY[validCount] = y;
                    ++validCount;
                }
            }
        }

        // Shuffle valid positions and pick a few
        for (int i = validCount - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(validX[i], validX[j]);
            std::swap(validY[i], validY[j]);
        }

        int n = std::min(cofresPorZona, validCount);
        for (int i = 0; i < n && selectedCount < totalCofres; ++i) {
            map.SetTile(validX[i], validY[i], L'█');
            cofreX[selectedCount] = validX[i];
            cofreY[selectedCount] = validY[i];
            ++selectedCount;
        }
    }

    // Fill any missing chests
    if (selectedCount < totalCofres) {
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                if (selectedCount >= totalCofres) break;
                if (isValidCofreSpot(x, y)) {
                    map.SetTile(x, y, L'█');
                    cofreX[selectedCount] = x;
                    cofreY[selectedCount] = y;
                    ++selectedCount;
                }
            }
        }
    }

    // Choose one chest to hide the key
    if (selectedCount > 0) {
        int keyIndex = 0;
        if (player.GetDifficulty() == Player::Difficulty::EASY) {
            keyIndex = rand() % selectedCount;
        } else {
            bool found = false;
            for (int i = 0; i < 10; ++i) {
                int idx = rand() % selectedCount;
                if (cofreY[idx] > (minY + maxY) / 2) {
                    keyIndex = idx;
                    found = true;
                    break;
                }
            }
            if (!found)
                keyIndex = rand() % selectedCount;
        }

        keyBoxPosition = std::make_pair(cofreX[keyIndex], cofreY[keyIndex]);
    }

    // Place decoration: tables
    int mesaX[] = {minX + 2, maxX - 2, minX + 2, maxX - 2};
    int mesaY[] = {minY + 1, minY + 1, maxY - 1, maxY - 1};
    for (int i = 0; i < 4; ++i)
        if (isValidCofreSpot(mesaX[i], mesaY[i]))
            map.SetTile(mesaX[i], mesaY[i], L'T');

    // Place decoration: torches
    for (int x = minX + 2; x < maxX - 2; x += 10)
        for (int y = minY + 2; y < maxY - 2; y += 5)
            if (isValidCofreSpot(x, y))
                map.SetTile(x, y, L'*');

    // Place decoration: lake
    int lakeX = (minX + maxX) / 2 - 2;
    int lakeY = (minY + maxY) / 2 - 1;
    bool canPlaceLake = true;
    for (int y = lakeY; y < lakeY + 3 && canPlaceLake; ++y)
        for (int x = lakeX; x < lakeX + 6; ++x)
            if (!isValidCofreSpot(x, y))
                canPlaceLake = false;

    if (canPlaceLake)
        for (int y = lakeY; y < lakeY + 3; ++y)
            for (int x = lakeX; x < lakeX + 6; ++x)
                map.SetTile(x, y, L'~');
}

// Set difficulty based folder and settings
void MainMaze::DetermineDifficultyFolder() {
    switch (player.GetDifficulty()) {
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

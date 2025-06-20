#ifndef MAPING_H
#define MAPING_H

// Include necessary libraries
#include <fstream> // For file handling
#include <iostream>
#include <string>
#include <conio.h> // For functions like _kbhit() and _getch()
#include <limits>  // For std::numeric_limits
#ifdef _WIN32      // Only include on Windows systems
#include <windows.h>
#endif

// Include headers of used classes
#include "utils/screen/colors.h"
#include "utils/functions/utils.h"

// Global constants for the maximum map size
const int MAP_HEIGHT = 95;
const int MAP_WIDTH = 60;

//*Purpose: Logic to draw maps; you just need to specify which map to draw and it will render
class Map
{
protected:
    //* Initial values
    // Note: Even though one of the goals was to make this variable (based on console size),
    // it's not feasible — vectors are required for that
    const int mH = 50; // Fixed internal height
    const int mW = 30; // Fixed internal width

    // Character matrix representing the map
    wchar_t grid[MAP_HEIGHT][MAP_WIDTH]{};

    // Dimensions of the loaded map
    int width = 0;
    int height = 0;

    // Player spawn position
    int spawnX = 1;
    int spawnY = 1;

public:
    // Getters for dimensions and spawn position
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetSpawnX() const { return spawnX; }
    int GetSpawnY() const { return spawnY; }

    Utils utils; // Utility object for printing/moving the cursor

    // --- Deprecated map reading method from composed path ---
    // std::ifstream IdentifierMap(std::string map)
    // {
    //     std::string path = "/maps/";
    //     std::string filename = path + map;
    //     return std::ifstream(filename);
    // }

    // void ReadMap(std::string key, int mapW, int mapH)
    // {
    //     std::ifstream file = IdentifierMap(key);
    //     if (!file.is_open())
    //     {
    //         std::cerr << "Error opening file: " << strerror(errno) << "\n";
    //     }

    //     std::string line;
    //     int y = 0;
    //     while (std::getline(file, line) && y < mapH)
    //     {
    //         int lineLength = std::min((int)line.size(), mapW);
    //         for (int x = 0; x < lineLength; ++x)
    //         {
    //             grid[y][x] = line[x];
    //             if (line[x] == ']')
    //             {
    //                 spawnX = x + 1;
    //                 spawnY = y;
    //             }
    //         }
    //         for (int x = lineLength; x < mW; ++x)
    //         {
    //             grid[y][x] = ' ';
    //         }
    //         ++y;
    //     }

    //     height = y;
    //     width = mW;
    // }

    // Reads the map from a plain text file
    void ReadMap(std::string key, int mapW, int mapH)
    {
        std::wifstream file(key, std::ios::binary); // Wide input stream
        file.imbue(std::locale(""));

        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << key << " -> " << strerror(errno) << "\n";
            return;
        }
        std::wstring line;
        int y = 0;
        int maxWidth = 0;

        while (std::getline(file, line) && y < MAP_HEIGHT)
        {
            int lineLength = std::min((int)line.size(), MAP_WIDTH);
            for (int x = 0; x < lineLength; ++x)
            {
                grid[y][x] = line[x];
                if (line[x] == ']') // Spawn marker
                {
                    spawnX = x + 1;
                    spawnY = y;
                }
            }
            for (int x = lineLength; x < MAP_WIDTH; ++x)
            {
                grid[y][x] = L' '; // Fill with spaces if line is short
            }

            if (lineLength > maxWidth)
                maxWidth = lineLength;

            ++y;
        }

        height = y;
        width = maxWidth;
    }

    // Draws the map on screen, optionally with player
    void DrawMap(std::string key, int mapW, int mapH, int playerX = 1, int playerY = 1)
    {
        ReadMap(key, mapW, mapH); // Load the map

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                utils.MoveCursor(playerX + x, playerY + y);
                if (x == playerX && y == playerY)
                {
                    std::wcout << PINK << L"o" << RESET; // Draw player
                }
                else
                {
                    std::wstring txt = L"";
                    txt += grid[y][x];
                    std::wcout << txt; // Draw map tile
                }
            }
        }
    }

    // Draws map and player, with configurable offset
    void DrawWithPlayer(int mapW, int mapH, int playerX = 1, int playerY = 1, int offsetX = 0, int offsetY = 0)
    {

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                utils.MoveCursor(offsetX + x, offsetY + y);

                if (x == playerX && y == playerY)
                {
                    std::wcout << PINK << L"இ" << RESET;
                }
                else
                {
                    wchar_t tile = grid[y][x];
                    switch (tile)
                    {
                    case '#':
                        std::wcout << GRAY << L"#" << RESET;
                        break;
                    case ']':
                        std::wcout << GRAY << L"]" << RESET;
                        break;
                    case 'B':
                        std::wcout << ORANGE << L"¤" << RESET;
                        break;
                    case 'A':
                        std::wcout << GREEN << L"♣" << RESET;
                        break;
                    case '~':
                        std::wcout << BLUE << L"≈" << RESET;
                        break;
                    case '*':
                        std::wcout << YELLOW_BRIGHT << L"✹" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }

    // Returns the character at position (x, y)
    wchar_t GetTile(int x, int y) const
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return grid[y][x];
        }
        return ' '; // Return space if out of bounds
    }

    // Sets the character at position (x, y)
    void SetTile(int x, int y, wchar_t value)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            grid[y][x] = value;
        }
    }
};

#endif
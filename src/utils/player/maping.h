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
const int MAP_WIDTH = 150;

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

    Utils utils;
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
                    case '0':
                        std::wcout << GRAY << L"░" << RESET;
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

    void DrawWithPlayerSphinx(int mapW, int mapH, int playerX = 1, int playerY = 1, int offsetX = 0, int offsetY = 0)
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
                    case 'A':
                        std::wcout << GREEN << L"║" << RESET;
                        break;
                    case 'B':
                        std::wcout << GREEN << L"═" << RESET;
                        break;
                    case 'v':
                        std::wcout << YELLOW << L"°" << RESET;
                        break;
                    case 'n':
                        std::wcout << YELLOW << L"¥" << RESET;
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

    // Draw the map of the genius
    //  Draws map and player, with configurable offset
    void DrawWithPlayerLamp(int mapW, int mapH, int playerX = 1, int playerY = 1, int offsetX = 0, int offsetY = 0)
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
                    case 'A':
                        std::wcout << GREEN << L"♣" << RESET;
                        break;
                    case '|':
                        std::wcout << YELLOW_BRIGHT << L"|" << RESET;
                        break;
                    case '-':
                        std::wcout << YELLOW_BRIGHT << L"-" << RESET;
                        break;
                    case '\\':
                        std::wcout << YELLOW_BRIGHT << L"\\" << RESET;
                        break;
                    case '.':
                        std::wcout << YELLOW_BRIGHT << L"." << RESET;
                        break;
                    case 'g':
                        std::wcout << YELLOW_BRIGHT << L"/" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }

    // Draw the map of the genius
    //   Draws map and player, with configurable offset
    void DrawWithPlayerGenius(int mapW, int mapH, int playerX = 1, int playerY = 1, int offsetX = 0, int offsetY = 0)
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
                    case 'A':
                        std::wcout << GREEN << L"♣" << RESET;
                        break;
                    case '|':
                        std::wcout << BLUE << L"|" << RESET;
                        break;
                    case '-':
                        std::wcout << BLUE << L"-" << RESET;
                        break;
                    case '_':
                        std::wcout << BLUE << L"_" << RESET;
                        break;
                    case '(':
                        std::wcout << BLUE << L"(" << RESET;
                        break;
                    case ')':
                        std::wcout << BLUE << L")" << RESET;
                        break;
                    case 'g':
                        std::wcout << BLUE << L"/" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }
    void DrawViewportAroundPlayerMaze(int playerX, int playerY, int viewWidth, int viewHeight, int offsetX = 0, int offsetY = 0)
    {
        int startX = playerX - viewWidth / 2;
        int startY = playerY - viewHeight / 2;

        if (startX < 0)
            startX = 0;
        if (startY < 0)
            startY = 0;
        if (startX + viewWidth > width)
            startX = width - viewWidth;
        if (startY + viewHeight > height)
            startY = height - viewHeight;
        if (startX < 0)
            startX = 0;
        if (startY < 0)
            startY = 0;

        for (int y = 0; y < viewHeight; ++y)
        {
            for (int x = 0; x < viewWidth; ++x)
            {
                int mapX = startX + x;
                int mapY = startY + y;
                utils.MoveCursor(offsetX + x, offsetY + y);

                if (mapX == playerX && mapY == playerY)
                {
                    std::wcout << PINK << L"இ" << RESET;
                }
                else
                {
                    wchar_t tile = GetTile(mapX, mapY);
                    switch (tile)
                    {
                    case '#':
                    case ']':
                        std::wcout << GRAY << tile << RESET;
                        break;
                    case '|':
                    case '-':
                        std::wcout << RESET << tile << RESET;
                        break;
                    case 'A':
                    case L'♣':
                        std::wcout << GREEN_BRIGHT << L"♣" << RESET;
                        break;
                    case '~':
                        std::wcout << BLUE << L"≈" << RESET;
                        break;
                    case '*':
                        std::wcout << YELLOW_BRIGHT << L"✹" << RESET;
                        break;
                    case L'T':
                        std::wcout << BROWN << L"T" << RESET;
                        break;
                    case L'█':
                        std::wcout << ORANGE << L"█" << RESET;
                        break;
                    case L'░':
                        std::wcout << GRAY_BRIGHT << L"░" << RESET;
                        break;
                    case L'K':
                        std::wcout << YELLOW << L"K" << RESET;
                        break;
                    case L'/':
                        std::wcout << CYAN << L"/" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }

    void DrawWithWindowView(int viewWidth, int playerX = 3, int playerY = 3, int offsetX = 0, int offsetY = 0, MapId mapId = MapId::FrstWay)
    {
        // Ensure the view width does not exceed the map width
        if (viewWidth > width)
            viewWidth = width;

        // Calculate starting X position for the viewport so player is centered
        int startX = playerX - viewWidth / 2;

        // Clamp startX to minimum 0 (left edge)
        if (startX < 0)
            startX = 0;

        // Clamp startX to maximum so viewport fits within map width
        if (startX + viewWidth > width)
            startX = width - viewWidth;

        // Just in case startX went below zero again after adjustment
        if (startX < 0)
            startX = 0;

        // Loop through each visible row (vertical)
        for (int y = 0; y < MAP_HEIGHT && y < height; ++y)
        {
            // Loop through each visible column in the viewport (horizontal)
            for (int x = 0; x < viewWidth; ++x)
            {
                int mapX = startX + x; // Actual map X coordinate corresponding to viewport column

                // Move console cursor to correct screen position plus offsets
                utils.MoveCursor(offsetX + x, offsetY + y);

                // Draw player if current tile matches player's position
                if (mapX == playerX && y == playerY)
                {
                    std::wcout << PINK << L"இ" << RESET; // Print player symbol in pink color
                }
                else
                {
                    wchar_t tile = GetTile(mapX, y); // Get the map tile character at this position
                    bool drawn = false;              // Flag to check if tile was drawn

                    // Draw tile differently depending on current map id (area/level)
                    switch (mapId)
                    {
                    case FrstWay:
                        // Tiles specific to FrstWay map
                        switch (tile)
                        {
                        case 'D':
                            std::wcout << GREEN << L"◇" << RESET;
                            drawn = true;
                            break;
                        case 'P':
                            std::wcout << GREEN << L" " << RESET;
                            drawn = true;
                            break;
                        case 'b':
                            std::wcout << BROWN << L"║" << RESET;
                            drawn = true;
                            break;
                        case 'a':
                            std::wcout << BROWN << L"═" << RESET;
                            drawn = true;
                            break;
                        }
                        break;

                    case MainRoom:
                        // Tiles specific to MainRoom map
                        switch (tile)
                        {
                        case '1':
                        case '2':
                        case '3':
                            std::wcout << GREEN << L"║" << RESET;
                            drawn = true;
                            break;
                        case '*':
                            std::wcout << BROWN << L"┬" << RESET;
                            drawn = true;
                            break;
                        case '-':
                            std::wcout << BROWN << L"│" << RESET;
                            drawn = true;
                            break;
                        case '=':
                            std::wcout << BROWN << L"┴" << RESET;
                            drawn = true;
                            break;
                        case 'f':
                            std::wcout << RED << L"→" << RESET;
                            drawn = true;
                            break;
                        case 'l':
                            std::wcout << RED << L"─" << RESET;
                            drawn = true;
                            break;
                        case 'b':
                            std::wcout << BROWN << L"║" << RESET;
                            drawn = true;
                            break;
                        case 'a':
                            std::wcout << BROWN << L"═" << RESET;
                            drawn = true;
                            break;
                        }
                        break;

                    case ElevatorGame:
                        // Tiles specific to ElevatorGame map
                        switch (tile)
                        {
                        case L'z':
                            std::wcout << BROWN << L"═" << RESET;
                            drawn = true;
                            break;
                        case L'j':
                            std::wcout << BROWN << L"║" << RESET;
                            drawn = true;
                            break;
                        case L'k':
                            std::wcout << YELLOW_BRIGHT << L"░" << RESET;
                            drawn = true;
                            break;
                        }
                        break;

                    default:
                        break;
                    }

                    // If tile was not drawn by any above case, draw default tiles
                    if (!drawn)
                    {
                        switch (tile)
                        {
                        case '#':
                            std::wcout << GRAY << L" " << RESET; // Draw gray space for wall or similar
                            break;
                        case '|':
                        case '_':
                            std::wcout << GRAY << L"░" << RESET; // Draw shaded gray block
                            break;
                        default:
                            std::wcout << tile; // Just print the tile character as is
                            break;
                        }
                    }
                }
            }
        }
    }

    void DrawWithSphinx(int viewWidth, int playerX = 3, int playerY = 3, int offsetX = 0, int offsetY = 0)
    {
        if (viewWidth > width)
            viewWidth = width;

        int startX = playerX - viewWidth / 2;
        if (startX < 0)
        {
            startX = 0;
        }
        if (startX + viewWidth > width)
        {
            startX = width - viewWidth;
        }
        if (startX < 0)
        {
            startX = 0;
        }

        for (int y = 0; y < MAP_HEIGHT && y < height; ++y)
        {
            for (int x = 0; x < viewWidth; ++x)
            {
                int mapX = startX + x;
                utils.MoveCursor(offsetX + x, offsetY + y);

                if (mapX == playerX && y == playerY)
                {
                    std::wcout << PINK << L"இ" << RESET;
                }
                else
                {
                    wchar_t tile = GetTile(mapX, y);
                    switch (tile)
                    {
                    case '#':
                        std::wcout << GRAY << L"#" << RESET;
                        break;
                    case 'A':
                        std::wcout << GREEN << L"║" << RESET;
                        break;
                    case 'B':
                        std::wcout << GREEN << L"═" << RESET;
                        break;
                    default:
                        std::wcout << tile;
                        break;
                    }
                }
            }
        }
    }
};

#endif
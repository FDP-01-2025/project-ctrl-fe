#include <fstream>
#include <iostream>
#include <string>
#include "Utils.h"
#include "colors.h"

#include <conio.h>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

//*Objetivo: Logica para dibujar los mapas, de manera que, simplemente hay que indicar que mapa queremos dibujar y ya
class Map
{
protected:
    //* Valores iniciales
    // PD: a pesar de que un objetivo era hacerlo variable, osea, que se relacionen junto al tamaño de consola pues, resumen, no se puede, se ocupa vectores para eso
    static const int mH = 50;
    static const int mW = 30;

    char grid[mH][mW]{};

    int width = 0;
    int height = 0;
    int spawnX = 1;
    int spawnY = 1;

public:
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    Utils utils;

    std::ifstream IdentifierMap(std::string map)
    {
        std::string path = "./src/maps/";
        std::string filename = path + map;

        return std::ifstream(filename);
    }

    void ReadMap(std::string key, int mapW, int mapH)
    {
        std::ifstream file = IdentifierMap(key);
        if (!file.is_open())
        {
            std::cerr << "Error al abrir el archivo: " << strerror(errno) << "\n";
        }

        std::string line;
        int y = 0;
        while (std::getline(file, line) && y < mapH)
        {
            int lineLength = std::min((int)line.size(), mapW);
            for (int x = 0; x < lineLength; ++x)
            {
                grid[y][x] = line[x];
                if (line[x] == ']')
                {
                    spawnX = x + 1;
                    spawnY = y;
                }
            }
            for (int x = lineLength; x < mW; ++x)
            {
                grid[y][x] = ' ';
            }
            ++y;
        }

        height = y;
        width = mW;
    }

    void DrawMap(std::string key, int mapW, int mapH, int playerX = 1, int playerY = 1)
    {
        ReadMap(key, mapW, mapH);

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                utils.MoveCursor(playerX + x, playerY + y);
                if (x == playerX && y == playerY)
                {
                    std::cout << PINK << "o" << RESET;
                }
                else
                {
                    std::cout << grid[y][x];
                }
            }
        }
    }

    char GetTile(int x, int y) const
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return grid[y][x];
        }
        return ' ';
    }

    void SetTile(int x, int y, char value)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            grid[y][x] = value;
        }
    }
};

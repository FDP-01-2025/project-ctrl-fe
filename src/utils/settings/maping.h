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

public:
    //* Valores iniciales
    // PD: a pesar de que un objetivo era hacerlo variable, osea, que se relacionen junto al tamaño de consola pues, resumen, no se puede, se ocupa vectores para eso
    static const int mH = 50;
    static const int mW = 30;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    std::ifstream identifierMap(int identifier)
    {
        std::string filename;
        switch (identifier)
        {
        case 0:
            filename = "./src/maps/mainMaps/main.txt";
            break;
        default:
            return std::ifstream();
        }

        return std::ifstream(filename);
    }

    void readMap(int key, int mapW, int mapH)
    {
        std::ifstream file = identifierMap(key);
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

    void drawMap(int key, int mapW, int mapH, int playerX = 1, int playerY = 1)
    {
        readMap(key, mapW, mapH);
        // drawBorders();

        for (int y = 0; y < getHeight(); ++y)
        {
            for (int x = 0; x < getWidth(); ++x)
            {
                Utils::moveCursor(playerX + x, playerY + y);
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

    void drawBorders()
    {
        for (int y = 0; y < mH; y++)
        {
            for (int x = 0; x < mW; x++)
            {
                // goToXY(x, y);
                if (y == 0 || y == mH - 1 || x == 0 || x == mW - 1)
                {
                    if ((y == mH / 2 && (x == 0 || x == mW - 1)) ||
                        (x == mW / 2 && (y == 0 || y == mH - 1)))
                    {
                        std::cout << " ";
                    }
                    else
                    {
                        std::cout << "#";
                    }
                }
                else
                {
                    std::cout << " ";
                }
            }
        }
    }

protected:
    char grid[mH][mW]{};

    int width = 0;
    int height = 0;
    int spawnX = 1;
    int spawnY = 1;
};

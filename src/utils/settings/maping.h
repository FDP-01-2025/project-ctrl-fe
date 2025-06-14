#ifndef MAPING_H
#define MAPING_H

// Include de librerías necesarias
#include <fstream>  // Para manejo de archivos
#include <iostream> 
#include <string>   
#include <conio.h> // Para funciones como _kbhit() y _getch()
#include <limits>  // Para std::numeric_limits
#ifdef _WIN32 // Solo incluir en sistemas Windows
#include <windows.h>
#endif

// Include de las cabeceras de las clases utilizadas
#include "Utils.h" 
#include "colors.h" 

// Constantes globales para el tamaño máximo del mapa
const int MAP_HEIGHT = 95;
const int MAP_WIDTH = 60;

//*Objetivo: Logica para dibujar los mapas, de manera que, simplemente hay que indicar que mapa queremos dibujar y ya
class Map
{
protected:
    //* Valores iniciales
    // PD: a pesar de que un objetivo era hacerlo variable, osea, que se relacionen junto al tamaño de consola pues, resumen, no se puede, se ocupa vectores para eso
    const int mH = 50; // Altura fija "interna"
    const int mW = 30; // Ancho fijo "interno"

    // Matriz de caracteres que representa el mapa
    char grid[MAP_HEIGHT][MAP_WIDTH]{};

    // Dimensiones del mapa cargado
    int width = 0;
    int height = 0;

    // Posición de aparición del jugador
    int spawnX = 1;
    int spawnY = 1;

public:
    // Getters de dimensiones y posición de aparición
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetSpawnX() const { return spawnX; }
    int GetSpawnY() const { return spawnY; }

    Utils utils; // Objeto utilitario para impresión/movimiento de cursor

    // Código comentado para leer mapas desde una ruta compuesta
    // std::ifstream IdentifierMap(std::string map)
    //  {
    //      std::string path = "/maps/";
    //      std::string filename = path + map;

    //     return std::ifstream(filename);
    // }

    // void ReadMap(std::string key, int mapW, int mapH)
    // {
    //     std::ifstream file = IdentifierMap(key);
    //     if (!file.is_open())
    //     {
    //         std::cerr << "Error al abrir el archivo: " << strerror(errno) << "\n";
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

     // Lee el mapa desde un archivo de texto plano
    void ReadMap(std::string key, int mapW, int mapH)
    {
        std::ifstream file(key); // Abre el archivo especificado por `key`
        
        if (!file.is_open())
        {
            std::cerr << "Error al abrir el archivo: " << key << " -> " << strerror(errno) << "\n";
            return;
        }

        std::string line;
        int y = 0;
        int maxWidth = 0;

        while (std::getline(file, line) && y < MAP_HEIGHT)
        {
            int lineLength = std::min((int)line.size(), MAP_WIDTH);
            for (int x = 0; x < lineLength; ++x)
            {
                grid[y][x] = line[x];
                if (line[x] == ']') // Marca de punto de aparición
                {
                    spawnX = x + 1;
                    spawnY = y;
                }
            }
            for (int x = lineLength; x < MAP_WIDTH; ++x)
            {
                grid[y][x] = ' '; // Rellena con espacios si la línea es corta
            }

            if (lineLength > maxWidth)
                maxWidth = lineLength;

            ++y;
        }

        height = y;
        width = maxWidth;
    }

    // Dibuja el mapa en consola con el jugador (opcional)
    void DrawMap(std::string key, int mapW, int mapH, int playerX = 1, int playerY = 1)
    {
        ReadMap(key, mapW, mapH); // Carga el mapa

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                utils.MoveCursor(playerX + x, playerY + y);
                if (x == playerX && y == playerY)
                {
                    std::cout << PINK << "o" << RESET; // Dibuja el jugador
                }
                else
                {
                    std::cout << grid[y][x]; // Dibuja el mapa
                }
            }
        }
    }

    // Dibuja el mapa con jugador y con desplazamiento (offset)
    void DrawWithPlayer(std::string key, int mapW, int mapH, int playerX = 1, int playerY = 1, int offsetX = 0, int offsetY = 0)
    {
        ReadMap(key, mapW, mapH); // Carga el mapa

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                utils.MoveCursor(offsetX + x, offsetY + y);

                if (x == playerX && y == playerY)
                {
                    std::cout << PINK << "o" << RESET; // Dibuja jugador
                }
                else
                {
                    char tile = grid[y][x]; // Caracter del mapa
                    switch (tile)
                    {
                    case '#':
                        std::cout << GRAY << "#" << RESET;
                        break;
                    case ']':
                        std::cout << GRAY << "]" << RESET;
                        break;
                    case 'B':
                        std::cout << ORANGE << "B" << RESET;
                        break;
                    case 'A':
                        std::cout << GREEN << "#" << RESET;
                        break;
                    case 'H':
                        std::cout << GREEN << "O" << RESET;
                        break;
                    case 'T':
                        std::cout << BROWN << "W" << RESET;
                        break;
                    case '~':
                        std::cout << BLUE << "~" << RESET;
                        break;
                    case '*':
                        std::cout << YELLOW_BRIGHT << "*" << RESET;
                        break;
                    default:
                        std::cout << tile; // Imprime sin color si no es especial
                        break;
                    }
                }
            }
        }
    }

    // Retorna el carácter que hay en la posición (x, y)
    char GetTile(int x, int y) const
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return grid[y][x];
        }
        return ' '; // Retorna espacio si está fuera de rango
    }

    // Cambia el carácter en la posición (x, y)
    void SetTile(int x, int y, char value)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            grid[y][x] = value;
        }
    }
};

#endif
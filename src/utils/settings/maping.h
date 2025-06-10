#include <iostream>
using namespace std;

void putMap(int key)
{
    drawBorders();

    switch (key)
    {
    case 1:
        drawMainMap();
        break;
    }
}

void drawBorders()
{
    for (int y = 0; y < ROOM_HEIGHT; y++)
    {
        for (int x = 0; x < ROOM_WIDTH; x++)
        {
            if (y == 0 || y == ROOM_HEIGHT - 1 || x == 0 || x == ROOM_WIDTH - 1)
            {
                if ((y == ROOM_HEIGHT / 2 && (x == 0 || x == ROOM_WIDTH - 1)) ||
                    (x == ROOM_WIDTH / 2 && (y == 0 || y == ROOM_HEIGHT - 1)))
                {
                    cout << " ";
                }
                else
                {
                    cout << "#";
                }
            }
            else
            {
                cout << " ";
            }
        }
    }
}

const int viewW = 40;
const int viewH = 25;

void drawMainMap()
{

    static const int mapW = 81;
    static const int mapH = 21;
    char mainMapa[mapH][mapW + 1] = {
        "################################################################################",
        "#                                                                              #",
        "# _____________________________________________________________________________#",
        "#                                                                               #",
        "#                                                                           ____#",
        "# _______________________________                                               ",
        "#                                |                                --->       1  ",
        "#                                |                                          _____",
        "#                                |                                              #",
        "#                                |                                              #",
        "#                                |                                          ____#",
        "#                                |                                              ",
        "#                                |                                ---->      2  ",
        "#                                |                                          _____",
        "#                                |                                              #",
        "#                                |                                              #",
        "#                                |                                          ____#",
        "#                                |                                              ",
        "#                                |                                ---->      3  ",
        "#                                |______________________________________________",
        "################################################################################"};

    system("cls");

    int startX = 3 - viewW / 2;

    if (startX < 0)
        startX = 0;
    if (startX + viewW > ROOM_WIDTH)
        startX = 50 - viewW;

    for (int y = 0; y < ROOM_HEIGHT; y++)
    {
        for (int x = 0; x < viewW; x++)
        {
            int mapX = startX + x;
            if (mapX == 2 && y == 3)
                std::cout << "/o";
            else
                std::cout << mainMapa[y][mapX];
        }
        std::cout << "\n";
    }
}

#ifndef ROOM_H
#define ROOM_H

const int ROOM_WIDTH = 40;
const int ROOM_HEIGHT = 15;

enum TileType
{
    EMPTY,
    KEY,
    EXIT
};

struct Room
{
    TileType tile;
};

void drawBorders();
#endif

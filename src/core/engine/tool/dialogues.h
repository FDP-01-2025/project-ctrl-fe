#pragma once
#include <iostream>
#include <string>
#include <windows.h>

const int widht = 35; // estas medidas son del mapa
const int height = 10;

void MoveCursor(int x, int y) // para el cursor
{
    COORD pos = {(SHORT)x, (SHORT)y};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawDialogue(const std::string &text, int offsetY = 1) // offsetY para indicar donde inicia
{
    int widthDialogue = text.length() + 4; // la longitud del texto
    int startX = (widht - widthDialogue) / 2;
    int startY = offsetY;

    MoveCursor(startX, startY);
    std::cout << char(201);
    for (int i = 0; i < widthDialogue - 2; i++)
        std::cout << char(205);
    std::cout << char(187);
    MoveCursor(startX, startY + 1);
    std::cout << char(186) << " " << text << " " << char(186);
    MoveCursor(startX, startY + 2);
    std::cout << char(200);
    for (int i = 0; i < widthDialogue - 2; i++)
        std::cout << char(205);
    std::cout << char(188);
}

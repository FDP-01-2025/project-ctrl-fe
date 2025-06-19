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

void DrawDialogue(const std::wstring &text, int offsetY = 1) // offsetY para indicar donde inicia
{
    int widthDialogue = text.length() + 4; // la longitud del texto
    int startX = (widht - widthDialogue) / 2;
    int startY = offsetY;

    MoveCursor(startX, startY);
    std::wcout << wchar_t(201);
    for (int i = 0; i < widthDialogue - 2; i++)
        std::wcout << wchar_t(205);
    std::wcout << wchar_t(187);
    MoveCursor(startX, startY + 1);
    std::wcout << wchar_t(186) << L" " << text << L" " << wchar_t(186);
    MoveCursor(startX, startY + 2);
    std::wcout << wchar_t(200);
    for (int i = 0; i < widthDialogue - 2; i++)
        std::wcout << wchar_t(205);
    std::wcout << wchar_t(188);
}

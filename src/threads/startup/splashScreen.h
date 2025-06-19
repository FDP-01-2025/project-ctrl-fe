#pragma once
#include <iostream>
#include <windows.h>
#include <thread>
#include "utils/screen/multiColors.h"

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Animación con retorno
bool InitializerThread(wchar_t chars[100][100], int colors[100][100], int width = 100, int height = 45, int durationSeconds = 4)
{
    /*const int delayMs = 50;
    DWORD start = GetTickCount();

    while ((GetTickCount() - start) < durationSeconds * 1000)
    {
        GenerateColor(chars, colors, width, height); // Generar nuevo frame

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                SetColor(colors[y][x]);
                std::cout << chars[y][x];
            }
            std::cout << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        system("cls");
    }

    SetColor(7); // Restaurar blanco
    system("cls");*/
    return true;
}

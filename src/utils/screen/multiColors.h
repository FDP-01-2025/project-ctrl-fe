#pragma once

void GenerateColor(char chars[100][100], int colors[100][100], int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            chars[y][x] = (rand() % 2) ? 219 : 176;     // ▓ o ░
            colors[y][x] = rand() % 16;                // Color entre 0 y 15
        }
    }
}

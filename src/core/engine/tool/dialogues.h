#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "utils/functions/utils.h" // for the cursor funtion

Utils utils;

void DrawDialogue(std::wstring text, int startX, int startY) // offsetY is to indicate where it begins
{
    int widthDialogue = text.length() + 4; // text lenght

    utils.MoveCursor(startX, startY);
    std::wcout << L"o";                         // left-up corner
    for (int i = 0; i < widthDialogue - 2; i++) // the 2 is by the cornenrs
        std::wcout << L"=";                     // up line
    std::wcout << L"o";                         // righ-up corner

    utils.MoveCursor(startX, startY + 1);
    std::wcout << L"| " << text << L" |"; // Where the text goes

    utils.MoveCursor(startX, startY + 2);
    std::wcout << L"o"; // left-down corner
    for (int i = 0; i < widthDialogue - 2; i++)
        std::wcout << L"="; // down line
    std::wcout << L"o";     // right down corner;
}

void ClearDialogue(int startX, int startY, int textLength)
{
    int widthDialogue = textLength + 4; // now text widthDialogue is manual input

    for (int i = 0; i < 3; i++) // son 3 líneas
    {
        utils.MoveCursor(startX, startY + i);
        for (int j = 0; j < widthDialogue; j++)
            std::wcout << L" ";
    }
}

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
    std::wcout << L"+"; // left-up corner   
    for (int i = 0; i < widthDialogue - 2; i++) //the 2 is by the cornenrs
        std::wcout << L"-"; // up line
    std::wcout << L"+";     // righ-up corner

    utils.MoveCursor(startX, startY + 1);
    std::wcout << L"| " << text << L" |"; // Where the text goes

    utils.MoveCursor(startX, startY + 2);
    std::wcout << L"+"; // left-down corner
    for (int i = 0; i < widthDialogue - 2; i++)
        std::wcout << L"-"; // down line
    std::wcout << L"+";     // right down corner;
}

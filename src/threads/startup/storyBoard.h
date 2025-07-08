#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"
#include <sstream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

bool ShowStoryBoard(int width, int height, Console consoleSettings, Utils utils)
{
    Sleep(500);
    utils.SetUtf8(); // Enable UTF-8 output in the console

    // ASCII art that will be revealed column by column
    const std::wstring frstW[] = {
        L" ___                                                                                      ",
        L"  |  ._     _.   |_      ._   _ _|_ |_   _ _|_ o  _  _. |   |    |    | _  ._ |  _|       ",
        L" _|_ | |   (_|   | | |_| |_) (_) |_ | | (/_ |_ | (_ (_| |    '--' '--' (_) |  | (_| o o o ",
        L"                     /   |                                                                "};
    const int titleLines = sizeof(frstW) / sizeof(frstW[0]); // Total number of lines in the ASCII art

    consoleSettings.SetConsoleFont(10, 17, L"Lucida Console"); // Set a small readable font
    system("cls"); // Clear screen

    // Get current console size
    int consoleW = utils.GetConsoleWidth();
    int consoleH = utils.GetConsoleHeight();
    int verticalPadding = std::max(0, (consoleH - titleLines) / 2); // Vertical centering

    // Calculate the longest line for column-based animation
    int maxCols = 0;
    for (int i = 0; i < titleLines; ++i)
    {
        if (frstW[i].length() > static_cast<size_t>(maxCols))
            maxCols = static_cast<int>(frstW[i].length());
    }

    // Reveal ASCII art column by column
    for (int col = 0; col < maxCols; ++col)
    {
        for (int row = 0; row < titleLines; ++row)
        {
            int paddingLeft = std::max(0, (consoleW - maxCols) / 2); // Horizontal centering
            if (col < static_cast<int>(frstW[row].length()))
            {
                wchar_t ch = frstW[row][col]; // Character at current column/row
                utils.PrintAtPosition(paddingLeft + col, verticalPadding + row, RED + std::wstring(1, ch) + RESET);
            }
        }
        Sleep(10); // Delay between columns for animation
    }

    Sleep(3000); // Hold final title for 3 seconds
    system("cls");
    int posY = consoleH / 2;

    // Paragraphs for the story
    const std::wstring storyParagraphs[] = {
        L"On an ordinary day in the village of Kirby, we find our heroine, the valiant knight Arlak, enjoying a peaceful moment with her beloved, Prince Onailiem.",
        L"Suddenly, without warning, the jealous master of a nearby dungeon—furious at the sight of their love—abducts the handsome prince and drags him to his lair.",
        L"Burning with rage, Arlak does not hesitate. She sets off on a daring quest to help the wicked Leugim get closer to God... all to rescue her beloved and live happily ever after."};

    const int numParagraphs = sizeof(storyParagraphs) / sizeof(storyParagraphs[0]);
    const int maxLines = 20;   // Max number of lines allowed per paragraph
    const int maxWidth = 100;  // Max line width in characters

    for (int p = 0; p < numParagraphs; ++p)
    {
        system("cls");

        // Line wrapping preparation
        std::wstring lines[maxLines];
        int lineCount = 0;
        std::wistringstream iss(storyParagraphs[p]);
        std::wstring word, currentLine;

        // Wrap paragraph into multiple lines based on maxWidth
        while (iss >> word)
        {
            if (currentLine.length() + word.length() + 1 > static_cast<size_t>(maxWidth))
            {
                if (lineCount < maxLines)
                {
                    lines[lineCount++] = currentLine;
                    currentLine = word;
                }
                else
                    break;
            }
            else
            {
                if (!currentLine.empty())
                    currentLine += L" ";
                currentLine += word;
            }
        }

        // Add last line if not empty
        if (!currentLine.empty() && lineCount < maxLines)
            lines[lineCount++] = currentLine;

        // Calculate vertical centering
        int topPad = std::max(0, (consoleH - lineCount) / 2);
        std::wcout << std::wstring(topPad, L'\n'); // Print vertical padding

        // Calculate total number of characters for timing
        int totalChars = 0;
        for (int i = 0; i < lineCount; ++i)
            totalChars += static_cast<int>(lines[i].length());

        // Distribute 7 seconds (7000 ms) over all characters
        int delayPerChar = totalChars > 0 ? (7000 / totalChars) : 0;

        // Animated line-by-line reveal from left to right
        for (int i = 0; i < lineCount; ++i)
        {
            const std::wstring line = lines[i];
            int len = static_cast<int>(line.length());
            int startX = std::max(0, (consoleW - len) / 2); // Horizontal centering
            int y = topPad + i;

            // Animate each character from left to right
            for (int j = 0; j < len; ++j)
            {
                utils.PrintAtPosition(startX + j, y, YELLOW + std::wstring(1, line[j]) + RESET);
                Sleep(delayPerChar);
            }
        }

        Sleep(1000); // Short pause after each paragraph
    }

    system("cls");
    consoleSettings.SetConsoleFont(); // Reset font to default
    Sleep(300); // Final short delay

    return true; // Function complete
}

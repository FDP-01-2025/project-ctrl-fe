#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"
#include <sstream> // for std::wistringstream
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

bool ShowStoryBoard(int width, int height, Console consoleSettings, Utils utils)
{
    Sleep(500);
    utils.SetUtf8(); // Set console output to UTF-8 encoding

    // ASCII art for the initial title screen
    const std::wstring frstW[] = {
        L" ___                                                                                      ",
        L"  |  ._     _.   |_      ._   _ _|_ |_   _ _|_ o  _  _. |   |    |    | _  ._ |  _|       ",
        L" _|_ | |   (_|   | | |_| |_) (_) |_ | | (/_ |_ | (_ (_| |    '--' '--' (_) |  | (_| o o o ",
        L"                     /   |                                                                "};
    const int titleLines = sizeof(frstW) / sizeof(frstW[0]); // Number of lines in the ASCII art

    // Set a smaller console font for the title screen to fit the ASCII art
    consoleSettings.SetConsoleFont(10, 17, L"Lucida Console");
    system("cls"); // Clear the console screen

    // Get the current console width and height
    int consoleW = utils.GetConsoleWidth();
    int consoleH = utils.GetConsoleHeight();

    // Calculate vertical padding to center the ASCII art vertically
    int verticalPadding = std::max(0, (consoleH - titleLines) / 2);
    std::wcout << std::wstring(verticalPadding, L'\n'); // Print newlines as top padding

    // Print each line of the ASCII art centered horizontally, in red color
    for (int i = 0; i < titleLines; ++i)
    {
        int padding = std::max(0, (consoleW - static_cast<int>(frstW[i].length())) / 2);
        std::wcout << std::wstring(padding, L' ') << RED << frstW[i] << RESET << L"\n";
    }

    Sleep(3000); // Pause for 3 seconds to let the user see the title
    system("cls"); // Clear the screen

    // Array of story paragraphs to be displayed one by one
    const std::wstring storyParagraphs[] = {
        L"On an ordinary day in the village of Kirby, we find our heroine, the valiant knight Arlak, enjoying a peaceful moment with her beloved, Prince Onailiem.",
        L"Suddenly, without warning, the jealous master of a nearby dungeon—furious at the sight of their love—abducts the handsome prince and drags him to his lair.",
        L"Burning with rage, Arlak does not hesitate. She sets off on a daring quest to help the wicked Leugim get closer to God... all to rescue her beloved and live happily ever after."};

    const int numParagraphs = sizeof(storyParagraphs) / sizeof(storyParagraphs[0]); // Number of paragraphs
    const int maxLines = 20;  // Maximum number of lines to display per paragraph
    const int maxWidth = 100; // Maximum width (characters) for each line when wrapping text

    // Loop through each paragraph to display it
    for (int p = 0; p < numParagraphs; ++p)
    {
        system("cls"); // Clear screen before showing the next paragraph

        std::wstring lines[maxLines]; // Array to store wrapped lines for current paragraph
        int lineCount = 0;            // Counter for how many lines are used

        std::wistringstream iss(storyParagraphs[p]); // Create stream from paragraph string
        std::wstring word, currentLine;              // Variables for word processing and current line

        // Read words one by one and build lines that fit maxWidth
        while (iss >> word)
        {
            // Check if adding the next word would exceed maxWidth
            if (currentLine.length() + word.length() + 1 > static_cast<size_t>(maxWidth))
            {
                if (lineCount < maxLines)
                {
                    lines[lineCount++] = currentLine; // Store current line
                    currentLine = word;                // Start a new line with the new word
                }
                else
                {
                    break; // If maxLines reached, stop adding more lines
                }
            }
            else
            {
                if (!currentLine.empty())
                    currentLine += L" "; // Add space before next word if line not empty
                currentLine += word;     // Append word to current line
            }
        }

        // Add the last line if it is not empty and we have space
        if (!currentLine.empty() && lineCount < maxLines)
        {
            lines[lineCount++] = currentLine;
        }

        // Calculate vertical padding to center the paragraph vertically
        int topPad = std::max(0, (consoleH - lineCount) / 2);
        std::wcout << std::wstring(topPad, L'\n'); // Print top padding lines

        // Print each wrapped line centered horizontally in yellow color
        for (int i = 0; i < lineCount; ++i)
        {
            int leftPad = std::max(0, (consoleW - static_cast<int>(lines[i].length())) / 2);
            std::wcout << std::wstring(leftPad, L' ') << YELLOW << lines[i] << RESET << L"\n";
        }

        Sleep(7000); // Pause for 7 seconds so user can read the paragraph
    }

    system("cls");           // Clear screen after all paragraphs shown
    consoleSettings.SetConsoleFont(); // Reset console font to default
    Sleep(300);              // Small delay before exiting

    return true;             // Indicate success
}

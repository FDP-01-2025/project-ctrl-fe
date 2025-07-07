#pragma once

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/worm/hudWorm.h"

class Worm
{
public:
    Worm();
    bool Run(Console consoleSettings); // Starts the worm game loop

private:
    std::deque<wchar_t> worm;   // Worm body segments
    int maxLength = 10;         // Maximum length of the worm
    int lives;                  // Player lives
    int correctAnswers = 0;     // Correct inputs collected
    int maxCorrectAnswers = 10; // Required correct inputs to win
    int startX = 7;             // Initial X position of worm
    int startY = 2;             // Initial Y position of worm
    bool result = false;        // Game result (true if player wins)
    int wormWidth;
    int currentWidth;

    std::vector<wchar_t> animationFrames = {L'@', L'o', L'O', L'0'}; // Worm animation characters

    Map map;            // Game map
    Utils utils;        // Utility functions
    Player player;      // Player entity
    HudWorm hud;        // HUD for worm game
    int viewWidth;      // Width of the view/camera
    int playerX;        // Player X position
    int playerY;        // Player Y position
    int animationFrame; // Current animation frame index

    int offsetX = 1, offsetY = 1; // Map drawing offset
    bool isRunning;               // Is the game running?

    // Initializes console style for this game
    void SetGoodStyle(Console consoleSettings);

    // Shows loading animation before the game starts
    void ShowLoadingAnimation(Console consoleSettings, int c, int d);

    // Processes player keyboard input
    void ProcessInput(char input, Console consoleSettings);

    // Loads the game level from file
    void LoadLevel(std::string key);

    // Generates a random string of keys the player must collect
    std::wstring GenerateKeys();

    // Draws the worm on screen
    void DrawWorm();

    // Clears the worm from the screen
    void ClearWorm();

    int messageX; // X position for displaying messages
    int messageY; // Y position for displaying messages
};

Worm::Worm() : isRunning(true)
{
    srand(time(NULL));
    std::vector<std::wstring> wormArt = {
        L"⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⢀⢴⣖⢤⣀⣼⠛⡚⣳⠢⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⢸⡃⢸⣿⡷⣯⠀⣟⢿⠀⠘⡌⢢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠈⢗⣬⡮⠃⠘⠩⠝⠃⠀⢀⠇⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⡇⠀⠀⠀⢠⡆⡰⠒⡞⠀⡰⠃⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠡⣀⣀⠔⢱⡰⠁⠀⢠⠊⠀⠀⢸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠘⠁⠀⠀⡸⠄⠤⠒⠁⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠉⠒⠒⠒⡟⠀⠀⠀⠀⡠⠊⠉⢲⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⡸⢀⡀⢀⣠⠃⠀⠀⠀⢰⠁⠀⠀⡜⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⠀⠉⡸⠀⠀⠀⠀⢸⣀⠤⢄⡇⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢸⠢⢄⣀⠠⢇⠀⠀⠀⠀⠈⠁⠀⠀⠘⡀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⡘⣄⠀⠀⢀⡴⠉⠉⠓⢤⠃⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠈⢧⠤⠐⠊⠀⡏⠉⠻⡉⠉⠳⡄⢀⠌⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠢⣀⠀⡰⠁⠀⠀⡇⠀⢀⠕⠃⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠐⠒⠒⠒⠓⠉⠁⠀⠀ ⠀⠀ "};

    wormWidth = (int)wormArt[0].size();
    currentWidth = wormWidth;

    for (int i = 0; i < maxLength; ++i)
    {
        worm.push_back(L'@');
    }
}

bool Worm::Run(Console consoleSettings)
{
    isRunning = true;
    lives = player.GetLives(); // Get player's lives

    SetGoodStyle(consoleSettings);
    std::string path = utils.GetAssetsPath() + "maps\\worm\\easy-levels\\level1.txt";
    LoadLevel(path);
    // Draw the map with the player
    map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), 5, 10, offsetX, offsetY);
    int animationFrame = 0;

    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\Solomons.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play background music
    while (isRunning && lives > 0 && correctAnswers < maxCorrectAnswers && !worm.empty())
    {
        std::wstring sequence = GenerateKeys(); // Generate a random letter sequence

        bool inputCompleted = false;
        std::wstring userInput = L"";

        auto startTime = std::chrono::steady_clock::now();
        DrawWorm();
        int previousWidth = -1;
        while (!inputCompleted && isRunning)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            int secondsLeft = 7 - static_cast<int>(elapsed);
            if (secondsLeft < 0)
                secondsLeft = 0;

            if (previousWidth != currentWidth)
            {
                ClearWorm();
                DrawWorm();
                previousWidth = currentWidth;
            }

            // Update player's lives
            player.SetLives(lives);

            // Draw the HUD with the remaining time and sequence
            hud.Draw(player, 1, map.GetWidth(), sequence, correctAnswers, maxCorrectAnswers, userInput, secondsLeft);

            Sleep(100);

            // Check if time is up
            if (elapsed >= 7)
            {
                inputCompleted = true;
                userInput = L""; // Clear input to force failure
            }

            if (_kbhit())
            {
                char c = _getch();
                if (c == '\r') // Enter key
                {
                    inputCompleted = true;
                }
                else if (isalpha(c))
                {
                    if (userInput.size() < 6)
                        userInput += c;
                }
            }
        }

        messageX = 10;
        messageY = offsetY + map.GetHeight() + 1;

        if (userInput == sequence)
        {
            correctAnswers++;
            if (currentWidth > 0)
            {
                currentWidth = currentWidth - 2;
            }
            utils.PrintAtPosition(messageX, messageY, L"Right! Worm loses a part", GREEN);
            Sleep(500);
            utils.PrintAtPosition(messageX, messageY, L"                                             ");
        }
        else
        {
            lives--;
            utils.PrintAtPosition(messageX, messageY, L"Incorrect! Life lost", RED);
            Sleep(500);
            utils.PrintAtPosition(messageX, messageY, L"                                             ");
        }
        Sleep(500);
    }

    // Game end screen
    utils.ClearScreen();
    if (lives == 0)
    {
        utils.PrintAtPosition(messageX, messageY, L"Game Over. No lives left.", RED);
        result = false;
    }
    else if (worm.empty())
    {
        utils.PrintAtPosition(messageX, messageY, L"You won! Worm vanished.", GREEN);
        result = true;
    }
    else
    {
        utils.PrintAtPosition(messageX, messageY, L"You won! All combos done.", GREEN);
        result = true;
    }
    Sleep(2000);
    system("cls");
    PlaySoundW(NULL, NULL, 0);        // Stop background music
    consoleSettings.SetConsoleFont(); // Reset font size
    return result;
}
void Worm::LoadLevel(std::string key)
{
    // Clear the screen completely and load the map from file
    utils.ClearScreenComplety();

    // Load the map using the Map class
    map.ReadMap(key, map.GetWidth(), map.GetHeight());

    // Set player at the map's spawn point
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Synchronize player's position variables (just in case)
    playerX = player.GetX();
    playerY = player.GetY();

    // Set the viewport width based on the console's width
    viewWidth = utils.GetConsoleWidth();
    if (viewWidth > map.GetWidth())
        viewWidth = map.GetWidth();
}

void Worm::SetGoodStyle(Console consoleSettings)
{
    Sleep(300);                                                // Small delay before changing the font
    consoleSettings.SetConsoleFont(19, 20, L"Lucida console"); // Set font for readability
    Sleep(100);                                                // Short delay after font change
}

// Generate a random sequence of 5 lowercase letters (a-z)
std::wstring Worm::GenerateKeys()
{
    std::wstring sequence;
    for (int i = 0; i < 5; ++i)
    {
        sequence += 'a' + rand() % 26;
    }
    return sequence;
}

// Draws the worm vertically on the console with colorized segments
// Draws the worm vertically on the console
void Worm::DrawWorm()
{
    // Static ASCII art representing each line of the worm's body
    const wchar_t *wormArt[] = {
        L"⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⢀⢴⣖⢤⣀⣼⠛⡚⣳⠢⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⢸⡃⢸⣿⡷⣯⠀⣟⢿⠀⠘⡌⢢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠈⢗⣬⡮⠃⠘⠩⠝⠃⠀⢀⠇⠀⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⡇⠀⠀⠀⢠⡆⡰⠒⡞⠀⡰⠃⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠡⣀⣀⠔⢱⡰⠁⠀⢠⠊⠀⠀⢸⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠘⠁⠀⠀⡸⠄⠤⠒⠁⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠉⠒⠒⠒⡟⠀⠀⠀⠀⡠⠊⠉⢲⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⡸⢀⡀⢀⣠⠃⠀⠀⠀⢰⠁⠀⠀⡜⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢀⠇⠀⠀⠉⡸⠀⠀⠀⠀⢸⣀⠤⢄⡇⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢸⠢⢄⣀⠠⢇⠀⠀⠀⠀⠈⠁⠀⠀⠘⡀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⡘⣄⠀⠀⢀⡴⠉⠉⠓⢤⠃⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠈⢧⠤⠐⠊⠀⡏⠉⠻⡉⠉⠳⡄⢀⠌⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠢⣀⠀⡰⠁⠀⠀⡇⠀⢀⠕⠃⠀⠀⠀⠀",
        L"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠐⠒⠒⠒⠓⠉⠁⠀⠀ ⠀⠀ "};

    const int wormArtHeight = sizeof(wormArt) / sizeof(wormArt[0]); // Total lines

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;

    for (int i = 0; i < wormArtHeight; ++i)
    {
        // Set position where the line will be printed
        coord.X = startX;
        coord.Y = startY + i;
        SetConsoleCursorPosition(hConsole, coord);

        // Clip the line to fit within currentWidth (simulate worm shrinking)
        std::wstring lineToPrint = std::wstring(wormArt[i]).substr(0, currentWidth);

        // Choose color for each segment
        const wchar_t *colorCode;

        // Alternate colors for body segments
        switch ((i - 1) % 8)
        {
        case 0:
            colorCode = GREEN;
            break; // First few segments green
        case 1:
            colorCode = YELLOW_BRIGHT;
            break;
        case 2:
            colorCode = CYAN;
            break;
        case 3:
            colorCode = ORANGE;
            break;
        case 4:
            colorCode = PINK;
            break;
        case 5:
            colorCode = PURPLE_BRIGHT;
            break;
        case 6:
            colorCode = BLUE;
            break;
        case 7:
            colorCode = BROWN;
            break;
        default:
            colorCode = WHITE_BRIGHT;
            break;
        }

        // Print the line with color, then reset color
        std::wcout << colorCode << lineToPrint << RESET;
    }
}

// Clears the worm drawing from the console
void Worm::ClearWorm()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    int height = 16;

    for (int i = 0; i < height; ++i)
    {
        coord.X = startX;
        coord.Y = startY + i;
        SetConsoleCursorPosition(hConsole, coord);
        std::wcout << std::wstring(wormWidth, L' ');
    }
}

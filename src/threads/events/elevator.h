#pragma once
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/elevator/hudElevator.h"
// Required to play sounds using Windows Multimedia API
#include <mmsystem.h>
// Link the program with the Windows Multimedia library
#pragma comment(lib, "winmm.lib")

// Structure that defines a math exercise/question for the game
struct Exercise
{
    std::wstring question;   // The question text
    std::wstring options[3]; // Array of 3 possible answer choices
    int correctAnswer;       // Index (0, 1, or 2) of the correct answer
};

Exercise GenerateExercise()
{
    int type = rand() % 9; // Randomly choose one of the 9 exercise types
    Exercise e;

    switch (type)
    {
    case 0:
        e.question = L"What is the result of 3x + 2 if x = 2?";
        e.options[0] = L"8";
        e.options[1] = L"6";
        e.options[2] = L"9";
        e.correctAnswer = 0;
        break;
    case 1:
        e.question = L"Simplify: (2x^2 + 4x) / 2x";
        e.options[0] = L"x + 2";
        e.options[1] = L"x + 1";
        e.options[2] = L"x^2 + 2x";
        e.correctAnswer = 0;
        break;
    case 2:
        e.question = L"What is the result of (x + 3)(x - 2) if x = 1?";
        e.options[0] = L"2";
        e.options[1] = L"-2";
        e.options[2] = L"1";
        e.correctAnswer = 1;
        break;
    case 3:
        e.question = L"What is the result of 2(x - 3) if x = 5?";
        e.options[0] = L"4";
        e.options[1] = L"-4";
        e.options[2] = L"16";
        e.correctAnswer = 0;
        break;
    case 4:
        e.question = L"Simplify: (4x^2 - 2x) / 2x";
        e.options[0] = L"2x - 1";
        e.options[1] = L"2x - x";
        e.options[2] = L"2x - 2";
        e.correctAnswer = 0;
        break;
    case 5:
        e.question = L"What is (x^2 - 4) if x = -2?";
        e.options[0] = L"0";
        e.options[1] = L"-8";
        e.options[2] = L"4";
        e.correctAnswer = 0;
        break;
    case 6:
        e.question = L"Evaluate: (3x + 1)(x - 2) if x = 2";
        e.options[0] = L"0";
        e.options[1] = L"7";
        e.options[2] = L"6";
        e.correctAnswer = 0;
        break;
    case 7:
        e.question = L"What is x^2 + 2x + 1 if x = -1?";
        e.options[0] = L"0";
        e.options[1] = L"4";
        e.options[2] = L"1";
        e.correctAnswer = 2;
        break;
    case 8:
        e.question = L"What is the result of (x^2 - 1)/(x - 1) if x = 3?";
        e.options[0] = L"4";
        e.options[1] = L"5";
        e.options[2] = L"3";
        e.correctAnswer = 0;
        break;
    }

    return e;
}
class Elevator
{
public:
    Elevator();
    bool Run(Console consoleSettings); // Starts the elevator logic and game loop

private:
    Map map;         // Game map
    Utils utils;     // Utility functions
    Player player;   // Player entity
    HudElevator hud; // Elevator HUD manager

    int consoleW;                 // Console width
    int viewW;                    // Viewport width
    int playerX;                  // Player X position
    int playerY;                  // Player Y position
    bool isRunning;               // Is the elevator running?
    bool result;                  // Last exercise result (true = correct)
    int lives;                    // Remaining lives
    int correctAnswers;           // Number of correct answers
    int messageX;                 // Message X position
    int messageY;                 // Message Y position
    Exercise currentExercise;     // Current math exercise
    int offsetX = 1, offsetY = 1; // Map drawing offset

    void LoadLevel(std::string key);                                  // Loads a level by key
    void SetGoodStyle(Console consoleSettings);                       // Sets color/style for correct answer
    void ShowLoadingAnimation(Console consoleSettings, int c, int d); // Displays loading animation
    void ProcessInput(char input, Console consoleSettings);           // Handles user input
    bool SolveExercise(Exercise &e);                                  // Solves the current exercise
    std::chrono::steady_clock::time_point startTime;                  // Start time for time-based mechanics
};

Elevator::Elevator() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;
}

bool Elevator::Run(Console consoleSettings)
{
    startTime = std::chrono::steady_clock::now(); // Start timer
    isRunning = true;
    correctAnswers = 0;
    currentExercise = GenerateExercise(); // Get first exercise

    lives = player.GetLives(); // Load lives from player
    SetGoodStyle(consoleSettings);
    std::string key = utils.GetAssetsPath() + "maps\\elevator\\elevator.txt";
    LoadLevel(key); // Load map file

    int startX = 2;                           // X position to print question
    int spacing = 30;                         // Horizontal spacing between options
    messageY = offsetY + map.GetHeight() + 2; // Y position to show question

    player.SetPosition(3, 6); // Set initial player position

    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\LunarAbyss.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play background music
    lives = 5;

    std::wstring lastQuestion = L"";
    while (isRunning)
    {
        // Calculate time remaining
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        int secondsLeft = 20 - static_cast<int>(elapsed);

        // If time runs out, lose a life
        if (secondsLeft <= 0)
        {
            lives--;
            player.SetLives(lives);

            if (lives <= 0)
            {
                utils.ClearScreen();
                result = false;    // Player lost all lives
                isRunning = false; // End game
            }

            currentExercise = GenerateExercise();         // Load new exercise
            startTime = std::chrono::steady_clock::now(); // Reset timer
            continue;
        }

        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY); // Draw map with camera
        hud.Draw(player, 1, map.GetWidth(), correctAnswers, secondsLeft);              // Draw HUD

        if (currentExercise.question != lastQuestion)
        {
            // Clear question and options lines
            utils.PrintAtPosition(startX, messageY, std::wstring(80, L' '), RESET);
            utils.PrintAtPosition(startX, messageY + 2, std::wstring(80, L' '), RESET);

            // Print new question and options
            utils.PrintAtPosition(startX, messageY, currentExercise.question, PURPLE_BRIGHT);
            for (int i = 0; i < 3; ++i)
            {
                int number = i + 1;
                std::wstring answer = ToWString(number) + L") " + currentExercise.options[i];
                int posX = startX + (i * 20);
                utils.PrintAtPosition(posX, messageY + 2, answer, GRAY_BRIGHT);
            }

            lastQuestion = currentExercise.question;
        }

        // Wait for user input
        if (_kbhit())
            ProcessInput(_getch(), consoleSettings);

        Sleep(50); // Small delay to avoid high CPU usage
    }

    PlaySoundW(NULL, NULL, 0);        // Stop background music
    consoleSettings.SetConsoleFont(); // Reset font size
    return result;                    // Return game result
}

void Elevator::ProcessInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid input key, no movement

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);
    // utils.PrintAtPosition(0, 0, std::wstring(L"Tile: ") + tile, GREEN); // Debug: show tile char

    if (tile == L' ' || tile == L'|')
    {
        player.SetPosition(newX, newY); // Move player to new position if tile is empty or vertical bar
    }

    // int messagePosY is the line to print feedback messages below question options
    int messagePosY = messageY + 4;

    if (tile == L'1' || tile == L'2' || tile == L'3')
    {
        int playerAnswer = tile - L'1'; // Convert '1','2','3' to 0,1,2

        if (playerAnswer == currentExercise.correctAnswer)
        {
            utils.PrintAtPosition(2, messagePosY, L"Correct", GREEN);
            Sleep(500);
            utils.PrintAtPosition(2, messagePosY, L"       ", RESET); // Clear message
            player.SetPosition(3, 6);                                 // Reset player position
            correctAnswers++;
        }
        else
        {
            utils.PrintAtPosition(2, messagePosY, L"Incorrect", RED);
            Sleep(500);
            utils.PrintAtPosition(2, messagePosY, L"         ", RESET); // Clear message
            player.SetPosition(3, 6);
            Sleep(100);
            lives--;
            player.SetLives(lives);
        }

        Sleep(1000);

        if (correctAnswers >= 3)
        {
            result = true;     // Player won
            isRunning = false; // Stop the game
        }
        else if (lives <= 0)
        {
            result = false;    // Player lost
            isRunning = false; // Stop the game
        }

        currentExercise = GenerateExercise();         // Load next exercise
        startTime = std::chrono::steady_clock::now(); // Reset timer
    }
}

void Elevator::LoadLevel(std::string key)
{
    // Clear the console screen completely before loading the map
    utils.ClearScreenComplety();

    // Load the map from the file specified by 'key'
    // Map dimensions are updated internally by ReadMap
    map.ReadMap(key, map.GetWidth(), map.GetHeight());

    // Set the player position to the map's spawn point
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Synchronize player position variables in case they are used separately
    playerX = player.GetX();
    playerY = player.GetY();

    // Get the console width to set the viewport width
    viewW = utils.GetConsoleWidth();

    // Ensure the viewport width does not exceed the map width
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();
}

void Elevator::SetGoodStyle(Console consoleSettings)
{
    Sleep(300); // Small delay before changing font for smooth transition

    // Set console font to Lucida Console with size 16x22 for better readability
    consoleSettings.SetConsoleFont(16, 22, L"Lucida console");

    Sleep(100); // Short delay after font change
}

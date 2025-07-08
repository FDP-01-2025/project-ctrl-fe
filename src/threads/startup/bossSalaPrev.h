#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/hud/hudBossSala.h"
#include <mmsystem.h>             // Play sound
#pragma comment(lib, "winmm.lib") // Link with sound library

class BossRoomPreview
{
public:
    BossRoomPreview();
    MapId Run(Console consoleSettings, MapId options[3]); // Main loop, returns selected MapId

private:
    Map map;               // Game map object
    Utils utils;           // Utility helper functions
    Player player;         // Player object for position and input
    HudBossSala hud;       // HUD display for boss room

    int viewWidth;         // Width of the viewport to display map
    int consoleWidth;      // Console window width
    int playerX;           // Player X coordinate
    int playerY;           // Player Y coordinate
    MapId selection;       // Currently selected boss room option
    MapId bossOptions[2];  // Array holding boss options available
    int totalOptions = 0;  // Number of valid boss options

    int offsetX = 1, offsetY = 1; // Offset for drawing map within console window

    bool isRunning; // Flag to keep the game loop running

    // Handles player input and moves player or selects boss room
    void ProcessInput(char input, Console consoleSettings);

    // Loads the map level from a file
    void LoadLevel(std::string key);

    // Writes the names of the boss doors onto the map (currently commented out)
    void ReplaceDoorNames(MapId op[2]);

    // Returns the display name for a given MapId
    std::wstring GetMapName(MapId id);

    // Helper to write a text string at a specific position on the map grid
    void WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text);
};

BossRoomPreview::BossRoomPreview() : isRunning(true)
{
    consoleWidth = utils.GetConsoleWidth(); // Get console width for view calculation
    viewWidth = consoleWidth;                // Initialize view width to full console width
}

MapId BossRoomPreview::Run(Console consoleSettings, MapId options[2])
{
    isRunning = true;
    totalOptions = 0;

    // Copy options into local array and count valid ones
    for (int i = 0; i < 3; ++i)
    {
        bossOptions[i] = options[i];
        if (options[i] != MapId::None)
            totalOptions++;
    }

    Sleep(100);
    consoleSettings.SetConsoleFont(21, 25, L"Lucida console"); // Set console font size for readability
    Sleep(100);

    // Load boss preview map
    std::string key = utils.GetAssetsPath() + "maps\\main\\boosSalaPrev.txt";
    LoadLevel(key);

    // Place player at fixed starting position for boss preview
    player.SetPosition(3, 7);

    // Play background music in loop asynchronously
    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\LookOut.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // Main game loop
    while (isRunning)
    {
        viewWidth = 30;             // Fixed view width for boss preview screen
        utils.ClearScreen();        // Clear console

        // Draw map portion around player with offset and view width
        map.DrawWithWindowView(viewWidth, player.GetX(), player.GetY(), offsetX, offsetY, MapId::MainRoom);

        // Draw HUD with player info and view width
        hud.Draw(player, 1, viewWidth);

        // Process keyboard input if available
        if (_kbhit())
            ProcessInput(_getch(), consoleSettings);

        Sleep(15); // Small delay to avoid high CPU usage
    }

    // Stop any playing sound
    PlaySoundW(NULL, NULL, 0);

    // Reset console font to default
    consoleSettings.SetConsoleFont();

    // Return the selected boss option
    return selection;
}

void BossRoomPreview::ReplaceDoorNames(MapId op[3])
{
    for (int i = 0; i < totalOptions; ++i)
    {
        if (bossOptions[i] == MapId::None)
            continue;

        // Currently commented out: get the name of the boss and write on map

        //std::wstring name = GetMapName(bossOptions[i]);

        int row;
        switch (i)
        {
        case 0:
            row = 3; // Row to write first door name
            break;
        case 1:
            row = 6; // Row to write second door name
            break;
        case 2:
            row = 9; // Row to write third door name
            break;
        }

        //WriteTextOnMap(map, row, 44, name);
    }
}

// Write a wide string text at a specific row and column on the map grid
void BossRoomPreview::WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        map.SetTile(colStart + i, row, text[i]);
    }
}

// Processes player input for movement and door selection
void BossRoomPreview::ProcessInput(char input, Console consoleSettings)
{
    // Get direction based on input key
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    // Ignore invalid input (no movement)
    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY); // Get tile character at new position

    // Check if player is stepping on a door tile
    if (tile == L'1' || tile == L'2')
    {
        // Assign selected boss option based on door
        if (tile == L'1')
        {
            selection = bossOptions[0];
        }
        if (tile == L'2')
        {
            selection = bossOptions[1];
        }

        map.SetTile(newX, newY, L'O'); // Open the door tile visually
        Sleep(500);
        system("cls");
        Sleep(300);

        // Reset font to default before confirmation message
        consoleSettings.SetConsoleFont();
        Sleep(300);

        // Set font smaller for confirmation message
        consoleSettings.SetConsoleFont(14, 20, L"Lucida Console");

        utils.PrintCentered(L"Are you sure you want to enter? S = yes, N = no");

        // Wait for confirmation input
        while (true)
        {
            char key = _getch();
            if (key == 's' || key == 'S')
            {
                system("cls");
                Sleep(100);
                utils.PrintCentered(L"You decided to enter.");
                Sleep(1000);
                system("cls");
                Sleep(500);
                isRunning = false; // Exit main loop
                return;
            }
            else if (key == 'n' || key == 'N')
            {
                system("cls");
                Sleep(100);
                utils.PrintCentered(L"You decided not to enter.");
                Sleep(1000);
                system("cls");

                // Reset font and larger size for normal gameplay
                consoleSettings.SetConsoleFont();
                Sleep(100);
                consoleSettings.SetConsoleFont(21, 25, L"Lucida console");
                Sleep(100);

                // Move player back a few steps to previous position
                player.SetPosition(player.GetX() - 3, player.GetY());
                Sleep(100);

                break;

                // Logic to reload map with previous position can be added here if needed
            }
        }
    }

    // Allow movement only if tile is empty space or door
    if (tile == ' ' || tile == '-')
    {
        player.SetPosition(newX, newY);
    }
}

// Load map from file and set player spawn position
void BossRoomPreview::LoadLevel(std::string key)
{
    utils.ClearScreenComplety(); // Clear entire console screen

    // Read map data from file, initializes width and height as needed
    map.ReadMap(key, map.GetWidth(), map.GetHeight());

    // Set player starting position to spawn point on map
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Synchronize player coordinates variables
    playerX = player.GetX();
    playerY = player.GetY();

    // Determine view width based on console width and map width
    viewWidth = utils.GetConsoleWidth();
    if (viewWidth > map.GetWidth())
        viewWidth = map.GetWidth();
}

// Returns human-readable name for each MapId enum
std::wstring BossRoomPreview::GetMapName(MapId id)
{
    // Possible MapIds include BomberManGame, MazeGame, GeniusGame, WormGame, ElevatorGame
    switch (id)
    {
    case BomberManGame:
        return L"Bomberman";
    case MazeGame:
        return L"Maze";
    case GeniusGame:
        return L"Genius";
    case ChestGame:
        return L"Chests";
    case WormGame:
        return L"Worm";
    case ElevatorGame:
        return L"Elevator";
    case BoosMario:
        return L"Boss Mario";
    case BoosZelda:
        return L"Boss Zelda";
    default:
        return L"Unknown";
    }
}

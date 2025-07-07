#pragma once

#include <iostream>
#include <conio.h>
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/hud/hudMainRoom.h"
#include <mmsystem.h>             // Play sound
#pragma comment(lib, "winmm.lib") // Link with sound library

class MainRoomGame
{
public:
    MainRoomGame();
    MapId Run(Console consoleSettings, MapId options[3]);

private:
    Map map;
    Utils utils;
    Player player;
    HudMain hud;

    int viewWidth;        // Width of the viewport for map drawing
    int consoleWidth;     // Console window width
    int playerX;          // Player X coordinate
    int playerY;          // Player Y coordinate
    MapId selection;      // Currently selected option
    MapId gameOptions[3]; // Array holding game options for doors
    int totalOptions = 0; // Number of valid options

    int offsetX = 1, offsetY = 1; // Offset for drawing the map in the console

    bool isRunning; // Flag to keep main loop running

    void ProcessInput(char input, Console consoleSettings);                         // Handles player input
    void LoadLevel(std::string key);                                                // Loads map from file
    std::wstring GetMapName(MapId id);                                              // Returns display name for MapId
    void ReplaceDoorNames(MapId options[3]);                                        // Writes door names on the map
    void WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text); // Writes text on map grid
};

MainRoomGame::MainRoomGame() : isRunning(true)
{
    consoleWidth = utils.GetConsoleWidth(); // Get console width on initialization
    viewWidth = consoleWidth;               // Viewport width equals console width initially
}

MapId MainRoomGame::Run(Console consoleSettings, MapId options[3])
{
    isRunning = true;
    totalOptions = 0;

    // Copy options and count how many are valid (not None)
    for (int i = 0; i < 3; ++i)
    {
        gameOptions[i] = options[i];
        if (options[i] != MapId::None)
            totalOptions++;
    }

    Sleep(100);
    consoleSettings.SetConsoleFont(25, 25, L"Lucida Console"); // Set console font larger for better visibility
    Sleep(100);

    // Load the main room map file
    std::string mapFile = utils.GetAssetsPath() + "maps\\main\\mainRoom.txt";
    LoadLevel(mapFile);

    // Set initial player position inside the map
    player.SetPosition(3, 7);

    // Play background sound asynchronously in a loop
    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\LookOut.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // Main game loop for the main room
    while (isRunning)
    {
        viewWidth = 30;      // Fixed viewport width to display map slice
        utils.ClearScreen(); // Clear console screen

        // Draw map with window view centered on player position, with offset
        map.DrawWithWindowView(viewWidth, player.GetX(), player.GetY(), offsetX, offsetY, MapId::MainRoom);

        // Draw the HUD with player info and viewport width
        hud.Draw(player, 1, viewWidth);

        // Process input if a key was pressed
        if (_kbhit())
            ProcessInput(_getch(), consoleSettings);

        Sleep(15); // Small delay to reduce CPU usage
    }

    // Stop playing any sound
    PlaySoundW(NULL, NULL, 0);

    // Reset console font to default
    consoleSettings.SetConsoleFont();

    // Return the selected game/door option
    return selection;
}

void MainRoomGame::ReplaceDoorNames(MapId options[3])
{
    // For each valid option, write the door name on the map
    for (int i = 0; i < totalOptions; ++i)
    {
        if (gameOptions[i] == MapId::None)
            continue;

        std::wstring doorName = GetMapName(gameOptions[i]);

        int row;
        // Assign row to write the door name based on option index
        switch (i)
        {
        case 0:
            row = 3;
            break;
        case 1:
            row = 6;
            break;
        case 2:
            row = 9;
            break;
        }

        WriteTextOnMap(map, row, 44, doorName);
    }

    // If there is no third option, block door 3 by drawing walls (rows 10 and 11)
    if (totalOptions < 3)
    {
        map.SetTile(60, 10, L'|');
        map.SetTile(60, 11, L'|');
    }

    // If there is no second option, block door 2 (rows 7 and 8)
    if (totalOptions < 2)
    {
        map.SetTile(60, 7, L'|');
        map.SetTile(60, 8, L'|');
    }
}

// Write a wide string text starting at (row, colStart) on the map grid
void MainRoomGame::WriteTextOnMap(Map &map, int row, int colStart, const std::wstring &text)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        map.SetTile(colStart + i, row, text[i]);
    }
}

// Process keyboard input for player movement and door selection
void MainRoomGame::ProcessInput(char input, Console consoleSettings)
{
    std::pair<int, int> direction = player.GetInputDirection(input);
    int dx = direction.first;
    int dy = direction.second;

    // Ignore if input does not correspond to movement
    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY); // Get tile character at new player position

    // If stepping onto a door tile ('1', '2' or '3')
    if (tile == L'1' || tile == L'2' || tile == L'3')
    {
        // Set selection based on door stepped on
        if (tile == L'1')
            selection = gameOptions[0];
        if (tile == L'2')
            selection = gameOptions[1];
        if (tile == L'3')
            selection = gameOptions[2];

        Sleep(500);
        system("cls");
        consoleSettings.SetConsoleFont();
        Sleep(300);
        consoleSettings.SetConsoleFont(14, 20, L"Lucida Console");
        Sleep(500);

        // Ask player for confirmation to enter
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
                Sleep(300);
                isRunning = false; // Exit main loop to start selected game
                return;
            }
            else if (key == 'n' || key == 'N')
            {
                system("cls");
                Sleep(100);
                utils.PrintCentered(L"You decided not to enter.");
                Sleep(1000);
                system("cls");
                consoleSettings.SetConsoleFont();
                Sleep(100);
                consoleSettings.SetConsoleFont(22, 25, L"Lucida Console");
                Sleep(100);

                // Move player back 3 tiles to previous position
                player.SetPosition(player.GetX() - 3, player.GetY());
                Sleep(100);

                // Reset door tile to original character
                map.SetTile(newX, newY, tile);
                break;
                // Logic to reload map and player position can be added here if needed
            }
        }
    }

    // Allow player to move only on empty space or floor tile (' ' or '-')
    if (tile == ' ' || tile == '-')
    {
        player.SetPosition(newX, newY);
    }
}

// Loads map from given file path, initializes player position and view width
void MainRoomGame::LoadLevel(std::string mapFile)
{
    utils.ClearScreenComplety(); // Clear entire console

    // Read map data into Map object
    map.ReadMap(mapFile, map.GetWidth(), map.GetHeight());

    // Write door names on map according to options
    ReplaceDoorNames(gameOptions);

    // Set player position to map spawn point
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());

    // Sync player coordinates variables
    playerX = player.GetX();
    playerY = player.GetY();

    // Set view width according to console width and map width
    viewWidth = utils.GetConsoleWidth();
    if (viewWidth > map.GetWidth())
        viewWidth = map.GetWidth();
}

// Returns display name string for given MapId value
std::wstring MainRoomGame::GetMapName(MapId id)
{
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
    case SphinxGameM:
        return L"Sphinx";
    case DodgeGame:
        return L"Dodge";
    default:
        return L"Unknown";
    }
}

#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm> // for std::max
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// Function to display the welcome and title screen with centered ASCII art
bool InitializerThread(int width, int height, Console consoleSettings, Utils utils)
{
    utils.SetUtf8(); // Set UTF-8 encoding for console output

    std::wstring soundPath = utils.GetAssetsPathW() + L"sounds\\GamecubeStartupLogo.wav";
    PlaySoundW(soundPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);

    // ASCII art for "Welcome"
    const std::wstring welcomeArt[] = {
        L"  (`  .-') /`   ('-.                                  _   .-')       ('-.  ",
        L"   `.( OO ),' _(  OO)                                ( '.( OO )_   _(  OO) ",
        L",--./  .--.  (,------.,--.       .-----.  .-'),-----. ,--.   ,--.)(,------.",
        L"|  |   |  |   |  .---'|  |.-')  '  .--./ ( OO'  .-.  '|   `.'   |  |  .---'",
        L"|  |   |  |,  |  |    |  | OO ) |  |     /   |  | |  ||         |  |  |    ",
        L"|  |.'.|  |_)(|  '--. |  |`-' |/_) |     |--)|  | |  ||  |'.'|  |  |  '--. ",
        L"|         |   |  .--'(|  '---.'||  |         |  | |  ||  |   |  |  |  .--' ",
        L"|   ,'.   |   |  `---.|      |(_'  '--'|     '  '-'  '|  |   |  |  |  `---.",
        L"'--'   '--'   `------'`------'   `-----'      `-----' `--'   `--'  `------'",
        L"                                                                           ",
        L"                                                                           ",
        L"                                                                           ",
        L"                                                                           "};

    // ASCII art for "TOWER OF LEGUIM"
    const std::wstring titleArt[] = {
        L"                                                                                   ",
        L" _____                                    _                                        ",
        L"(_   _)                                  | |        /\\                             ",
        L"  | | ___   __   __  ___  ___     ___   _| |_      /  \\   ___ _   _ _   _ _  _   _ ",
        L"  | |/ _ \\ / / _ \\ \\/ __)/ _ \\   / _ \\ /     \\    / /\\ \\ / __| \\ / ) | | | || | | |",
        L"  | ( (_) ) |_/ \\_| > _)| |_) ) ( (_) | (| |) )  / /  \\ \\> _) \\ v /| |_| | || |_| |",
        L"  |_|\\___/ \\___^___/\\___)  __/   \\___/ \\_   _/  /_/    \\_\\___) | |  \\___/ \\_) ._,_|",
        L"                        | |              | |                   | |          | |    ",
        L"                        |_|              |_|                   |_|          |_|    "};

    const int welcomeLines = sizeof(welcomeArt) / sizeof(welcomeArt[0]);
    const int titleLines = sizeof(titleArt) / sizeof(titleArt[0]);

    // Set smaller console font for welcome screen
    consoleSettings.SetConsoleFont(12, 18, L"Lucida Console");
    system("cls"); // Clear screen

    int consoleW = utils.GetConsoleWidth();
    int consoleH = utils.GetConsoleHeight();

    // Rainbow color palette for welcome text
    const std::wstring rainbowColors[] = {
        PINK,
        PINK,
        RED,
        ORANGE,
        YELLOW,
        GREEN,
        CYAN,
        BLUE,
        PURPLE};
    const int numColors = sizeof(rainbowColors) / sizeof(rainbowColors[0]);

    int verticalPaddingW = std::max(0, (consoleH - welcomeLines) / 2);
    std::wcout << std::wstring(verticalPaddingW, L'\n'); // Add top padding

    // Print welcome lines with rainbow color cycling
    for (int i = 0; i < welcomeLines; ++i)
    {
        int padding = std::max(0, (consoleW - static_cast<int>(welcomeArt[i].length())) / 2);
        const std::wstring color = rainbowColors[i % numColors];
        std::wcout << std::wstring(padding, L' ') << color << welcomeArt[i] << RESET << L"\n";
    }
    utils.PrintAtPosition(10, 60, L"        Made by: Cntrl + Fe", GRAY);

    Sleep(1500); // Pause before clearing
    system("cls");

    int verticalPadding = std::max(0, (consoleH - titleLines) / 2);
    std::wcout << std::wstring(verticalPadding, L'\n'); // Add top padding for title

    // Print title in red
    for (int i = 0; i < titleLines; ++i)
    {
        int padding = std::max(0, (consoleW - static_cast<int>(titleArt[i].length())) / 2);
        std::wcout << std::wstring(padding, L' ') << RED << titleArt[i] << RESET << L"\n";
    }

    Sleep(2499); // Pause before clearing
    system("cls");

    // Reset console font to default size
    consoleSettings.SetConsoleFont();
    Sleep(600);

    // Construct the full path to the sound file and play it asynchronously
    std::wstring soundPath2 = utils.GetAssetsPathW() + L"sounds\\PikminCreepRadiohead.wav";
    PlaySoundW(soundPath2.c_str(), NULL, SND_FILENAME | SND_ASYNC);

    return true;
}
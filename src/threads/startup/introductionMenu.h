#pragma once

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"

using std::wifstream;
using std::wstring;

// Obtiene el ancho actual de la consola
static int GetConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return 80;
}

// Centra un texto en un ancho dado
static wstring CenterText(const wstring &text, int width)
{
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0)
        padding = 0;
    return wstring(padding, L' ') + text;
}

// Menú principal: Jugar / Continuar / Salir
// Devuelve 1..3
// Function that starts the main menu interface of the game
static int StartMenu(Console &consoleSettings, Utils &utils)
{
    // Set the console font to "Lucida Console" with size 16x24
    consoleSettings.SetConsoleFont(16, 24, L"Lucida Console");

    // Define the available options for the start menu
    const wstring allOptions[] = {L"New game", L"Continue", L"Exit"};

    // Flag that indicates whether the "Continue" option should be enabled
    bool canContinue = false;

    // Build the file path to the game progress counter file
    std::string filename = utils.GetAssetsPath() + "data\\GamesCounter.txt";

    // Try to open the file to read game progress
    std::ifstream gamesLecture(filename, std::ios::in);

    if (gamesLecture)
    {
        int dummy;
        // Attempt to read at least one value from the file
        while (gamesLecture >> dummy) // Read a number
        {
            canContinue = true; // If at least one number is read, allow "Continue"
            break;              // Stop reading after the first value
        }
        gamesLecture.close(); // Close the file after reading
    }

    const int totalOptions = 3; // Total number of menu options
    int currentOption = 0;      // Index of the currently selected option

    // Main loop to display and interact with the menu
    while (true)
    {
        // Clear the console screen
        system("cls");

        // Set console encoding to UTF-8
        utils.SetUtf8();

        // Get current console width to center the text
        int width = GetConsoleWidth();

        // Print the title of the game centered
        std::wcout << WHITE_BRIGHT << L"\n\n"
                   << CenterText(L"LEGUIM DUNGEON ☻", width) << L"\n"
                   << GRAY << CenterText(L"------------------------", width) << L"\n\n";

        // Loop to print each menu option
        for (int i = 0; i < totalOptions; ++i)
        {
            wstring displayText = allOptions[i];

            // If the option is "Continue" and the file was not found, disable it
            if (i == 1 && !canContinue)
                displayText = L"Continue"; // Keep the label but mark as disabled

            // Highlight the currently selected option
            wstring line = (i == currentOption)
                               ? L">> " + displayText + L" <<"
                               : L"   " + displayText + L"   ";

            // Change the color based on the option state
            std::wcout << (i == currentOption ? GREEN : (i == 1 && !canContinue ? GRAY : WHITE_BRIGHT))
                       << CenterText(line, width) // Center the line
                       << RESET << L"\n";         // Reset color after printing
        }

        // Read keyboard input
        int key = _getch();

        // Check for arrow key input (extended keys start with 224)
        if (key == 224)
        {
            key = _getch(); // Get the actual arrow key code

            if (key == 72)                                                         // Arrow Up
                currentOption = (currentOption - 1 + totalOptions) % totalOptions; // Move selection up
            else if (key == 80)                                                    // Arrow Down
                currentOption = (currentOption + 1) % totalOptions;                // Move selection down
        }
        else if (key == 13) // Enter key pressed
        {
            // Prevent selection if "Continue" is not allowed
            if (currentOption == 1 && !canContinue)
                continue; // Do nothing and wait for valid input

            // Return the selected option + 1 as an identifier
            return currentOption + 1;
        }
    }
}

// Menú de confirmación de salida: Sí / No
// Devuelve true si elige "Sí"
static bool ConfirmExitMenu(Console &consoleSettings, Utils &utils)
{
    consoleSettings.SetConsoleFont(16, 24, L"Lucida Console");

    const wstring choices[] = {L"Yes", L"No"};
    const int totalChoices = 2;
    int option = 0;

    while (true)
    {
        system("cls");
        int width = GetConsoleWidth();
        utils.SetUtf8();

        // Pregunta
        std::wcout << WHITE_BRIGHT << L"\n\n"
                   << CenterText(L"Are you sure you want to exit?", width)
                   << L"\n\n";

        // Opciones
        for (int i = 0; i < totalChoices; ++i)
        {
            wstring line = (i == option)
                               ? L">> " + choices[i] + L" <<"
                               : L"   " + choices[i] + L"   ";

            std::wcout << (i == option ? GREEN : WHITE_BRIGHT)
                       << CenterText(line, width)
                       << RESET << L"\n";
        }

        int key = _getch();
        if (key == 224)
        {
            key = _getch();
            if (key == 72) // arriba
                option = (option - 1 + totalChoices) % totalChoices;
            else if (key == 80) // abajo
                option = (option + 1) % totalChoices;
        }
        else if (key == 13) // Enter
        {
            return (option == 0);
        }
    }
}

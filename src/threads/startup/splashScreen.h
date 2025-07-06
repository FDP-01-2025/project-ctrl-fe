#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "../../utils/screen/colors.h"
#include "../../utils/functions/utils.h"
#include "../../core/engine/settings/console.h"

bool InitializerThread(int width, int height, Console consoleSettings, Utils utils)
{
    // Arte ASCII para "Bienvenido a..."
    const std::wstring welcomeArt[] = {
        L"  ____  _                        _           _            _           ",
        L" | __ )(_) ___  _ __ ___   __ _| |_ ___  __| | ___ _ __ | |__   ___  ",
        L" |  _ \\| |/ _ \\| '_ ` _ \\ / _` | __/ _ \\/ _` |/ _ \\ '_ \\| '_ \\ / _ \\ ",
        L" | |_) | | (_) | | | | | | (_| | ||  __/ (_| |  __/ | | | | | |  __/ ",
        L" |____/|_|\\___/|_| |_| |_|\\__,_|\\__\\___|\\__,_|\\___|_| |_|_| |_|\\___| "};

    // Arte ASCII para "LA TORRE DE LEGUIM"
    const std::wstring titleArt[] = {
        L"  _      _____     _______                       _____       _           _         ",
        L" | |    |_ _\\ \\   / / ____|                     |_   _|     | |         (_)        ",
        L" | |     | | \\ \\ / /| (___   ___  ___  _ __ ___   | |  _ __ | |_   _ ___ _ ___ ___ ",
        L" | |     | |  \\ V /  \\___ \\ / _ \\/ _ \\| '_ ` _ \\  | | | '_ \\| | | | / __| / __/ __|",
        L" | |_____| |_  | |   ____) |  __/ (_) | | | | | |_| |_| | | | | |_| \\__ \\ \\__ \\__ \\",
        L" |______|___|  |_|  |_____/ \\___|\\___/|_| |_| |_|_____|_| |_|_|\\__,_|___/_|___/___/"};

    consoleSettings.SetConsoleFont(12, 18, L"Lucida Console");

    // Mostrar "Bienvenido a..."
    system("cls");
    for (const auto &line : welcomeArt)
    {
        utils.PrintCentered(line);
    }

    Sleep(1000);
    system("cls");

    // Mostrar "LA TORRE DE LEGUIM"
    consoleSettings.SetConsoleFont(20, 26, L"Lucida Console");
    Sleep(500);

    for (const auto &line : titleArt)
    {
        consoleSettings.SetColor(FOREGROUND_BLUE);
        utils.PrintCentered(line);
    }

    Sleep(1000);
    system("cls");

    // Restaurar fuente por defecto
    consoleSettings.SetConsoleFont();
    Sleep(500);

    return true;
}

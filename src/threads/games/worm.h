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
    bool Run(Console consoleSettings);

private:
    deque<wchar_t> worm;
    int maxLength = 10;
    int lives;
    int corrects = 0;
    int maxCorrects = 10;
    int baseX = 18;
    int baseY = 2;

    vector<wchar_t> animationFrames = {L'@', L'o', L'O', L'0'};

    Map map;
    Utils utils;
    Player player;
    HudWorm hud;
    int viewW;
    int playerX;
    int playerY;
    int animationFrame;

    int offsetX = 1, offsetY = 1;

    bool isRunning;

    void SetGoodStyle(Console consoleSettings);
    void ShowLoadingAnimation(Console consoleSettings, int c, int d);
    void ProceesInput(char input, Console consoleSettings);
    void LoadLevel(std::string key);
    std::wstring GenerateKeys();
    void DrawWorm(int animationF);
    void ClearWorm();

    int mensajeX;
    int mensajeY;
};

Worm::Worm() : isRunning(true)
{
    srand(time(NULL));
    for (int i = 0; i < maxLength; ++i)
    {
        worm.push_back(L'@');
    }
}

bool Worm::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings);
    std::string key = utils.GetAssetsPath() + "maps\\worm\\easy-levels\\level1.txt";
    LoadLevel(key);

    lives = player.GetLives();

    int animationFrame = 0;

    while (isRunning && lives > 0 && corrects < maxCorrects && !worm.empty())
    {
        std::wstring seq = GenerateKeys(); // Secuencia aleatoria de letras

        bool inputDone = false;
        std::wstring userInput = L"";
        int animFrame = 0;

        auto startTime = std::chrono::steady_clock::now();

        while (!inputDone && isRunning)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            int secondsLeft = 6 - static_cast<int>(elapsed);
            if (secondsLeft < 0)
                secondsLeft = 0;

            utils.ClearScreen();

            // Dibujar mapa con jugador
            map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), 5, 10, offsetX, offsetY);

            // Dibujar gusano
            ClearWorm();
            DrawWorm(animFrame);

            // Actualizar vidas
            player.SetLives(lives);

            // Dibujar HUD con tiempo restante
            hud.Draw(player, 1, map.GetWidth(), seq, corrects, maxCorrects, userInput, secondsLeft);

            animFrame++;
            Sleep(100);

            // Verificar si se acabó el tiempo
            if (elapsed >= 6)
            {
                inputDone = true;
                userInput = L""; // lo borra para que falle automáticamente
            }

            if (_kbhit())
            {
                char c = _getch();
                if (c == '\r') // Enter
                {
                    inputDone = true;
                }
                else if (isalpha(c))
                {
                    userInput += c;
                }
            }
        }

        mensajeX = offsetX;
        mensajeY = offsetY + map.GetHeight() + 2;
        if (userInput == seq)
        {
            corrects++;
            if (!worm.empty())
                worm.pop_back(); // Reducir gusano
            utils.PrintAtPosition(mensajeX, mensajeY, L"¡Correcto! El gusano pierde una sección.", GREEN);
            Sleep(500);
            utils.PrintAtPosition(mensajeX, mensajeY, L"                                             ");
        }
        else
        {
            lives--;
            utils.PrintAtPosition(mensajeX, mensajeY, L"Incorrecto. Pierdes una vida.", RED);
            Sleep(500);
            utils.PrintAtPosition(mensajeX, mensajeY, L"                                             ");
        }
        Sleep(500);
    }

    // Final del juego
    utils.ClearScreen();
    if (lives == 0)
        utils.PrintAtPosition(mensajeX, mensajeY, L"Perdiste todas las vidas. Fin del juego.", RED);
    else if (worm.empty())
        utils.PrintAtPosition(mensajeX, mensajeY, L"¡Ganaste! El gusano desapareció.", GREEN);
    else
        utils.PrintAtPosition(mensajeX, mensajeY, L"¡Ganaste! Completaste todas las combinaciones.", GREEN);

    Sleep(2000);
    return (lives > 0);
}

void Worm::LoadLevel(std::string key)
{
    // Clear screen and load map if found
    utils.ClearScreenComplety();
    // We call ReadMap from the Map class to load the map
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    playerX = player.GetX(); // sincronizar por si acaso
    playerY = player.GetY();

    viewW = utils.GetConsoleWidth();
    if (viewW > map.GetWidth())
        viewW = map.GetWidth();
}

void Worm::ShowLoadingAnimation(Console consoleSettings, int cycles = 3, int delay = 300)
{
    Sleep(300);
    consoleSettings.SetConsoleFont(19, 25, L"Lucida Console");
    consoleSettings.SetColor(FOREGROUND_RED);

    std::wstring baseText = L"Loading";
    std::wstring dots[] = {L".", L"..", L"..."};

    for (int i = 0; i < cycles * 3; ++i)
    {
        std::wstring text = baseText + dots[i % 3];

        utils.ClearScreen();
        utils.PrintCentered(text);

        Sleep(delay);
    }

    system("cls");
}

void Worm::SetGoodStyle(Console consoleSettings)
{
    ShowLoadingAnimation(consoleSettings);
    Sleep(300);
    consoleSettings.SetConsoleFont(19, 25, L"Lucida console");
    Sleep(100);
}

// Generar secuencia random de 5 teclas (a-z)
std::wstring Worm::GenerateKeys()
{
    std::wstring seq;
    for (int i = 0; i < 5; ++i)
    {
        seq += 'a' + rand() % 26;
    }
    return seq;
}

void Worm::DrawWorm(int animationFrame)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;

    for (size_t i = 0; i < worm.size(); ++i)
    {
        wchar_t c = worm[i];
        if (animationFrame % 2 == 0 && i % 2 == 0)
            c = animationFrames[(animationFrame) % animationFrames.size()];
        else if (animationFrame % 2 == 1 && i % 2 == 1)
            c = animationFrames[(animationFrame) % animationFrames.size()];

        coord.X = baseX;
        coord.Y = baseY + static_cast<SHORT>(i);
        SetConsoleCursorPosition(hConsole, coord);

        wcout << c;
    }
}

void Worm::ClearWorm()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;

    for (int i = 0; i < maxLength; ++i)
    {
        coord.X = baseX;
        coord.Y = baseY + i;
        SetConsoleCursorPosition(hConsole, coord);
        wcout << L' ';
    }
}

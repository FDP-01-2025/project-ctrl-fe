#pragma once
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/elevator/hudElevator.h"

struct Ejercicio
{
    std::wstring pregunta;
    std::wstring opciones[3];
    int respuestaCorrecta;
};

Ejercicio GenerarEjercicio()
{
    int tipo = rand() % 3;
    Ejercicio e;

    switch (tipo)
    {
    case 0:
        e.pregunta = L"¿Cuál es el resultado de 3x + 2 si x = 2?";
        e.opciones[0] = L"8";
        e.opciones[1] = L"6";
        e.opciones[2] = L"9";
        e.respuestaCorrecta = 0;
        break;
    case 1:
        e.pregunta = L"¿Simplifica: (2x^2 + 4x) / 2x?";
        e.opciones[0] = L"x + 2";
        e.opciones[1] = L"x + 1";
        e.opciones[2] = L"x^2 + 2x";
        e.respuestaCorrecta = 0;
        break;
    case 2:
        e.pregunta = L"¿Cuál es el resultado de (x+3)(x-2) si x = 1?";
        e.opciones[0] = L"2";
        e.opciones[1] = L"-2";
        e.opciones[2] = L"1";
        e.respuestaCorrecta = 1;
        break;
    }

    return e;
}

class Elevator
{
public:
    Elevator();
    bool Run(Console consoleSettings);

private:
    Map map;
    Utils utils;
    Player player;
    HudElevator hud;

    int consoleW;
    int viewW;
    int playerX;
    int playerY;
    bool isRunning;
    bool result;
    int lives;
    int aciertos;
    int mensajeX;
    int mensajeY;
    Ejercicio ejercicioActual;
    int offsetX = 1, offsetY = 1; // Map drawing offset

    void LoadLevel(std::string key);
    void SetGoodStyle(Console consoleSettings);
    void ShowLoadingAnimation(Console consoleSettings, int c, int d);
    void ProceesInput(char input, Console consoleSettings);
    bool ResolverEjercicio(Ejercicio &e);
    std::chrono::steady_clock::time_point startTime;
};

Elevator::Elevator() : isRunning(true)
{
    consoleW = utils.GetConsoleWidth();
    viewW = consoleW;

    aciertos = 0;
    ejercicioActual = GenerarEjercicio();
    startTime = std::chrono::steady_clock::now();
}

bool Elevator::Run(Console consoleSettings)
{
    isRunning = true;
    lives = player.GetLives();
    SetGoodStyle(consoleSettings);
    std::string key = utils.GetAssetsPath() + "maps\\elevator\\elevator.txt";
    LoadLevel(key);

    int startX = 2;
    int spacing = 30;
    mensajeY = offsetY + map.GetHeight() + 2;

    while (isRunning)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        int secondsLeft = 20 - static_cast<int>(elapsed);

        // Tiempo agotado
        if (secondsLeft <= 0)
        {
            lives--;
            player.SetLives(lives);

            if (lives <= 0)
            {
                utils.ClearScreen();
                result = false;    // retornar que se quedó sin vidas
                isRunning = false; // parar el juego
            }

            // Generar nuevo ejercicio y reiniciar tiempo
            ejercicioActual = GenerarEjercicio();
            startTime = std::chrono::steady_clock::now();
            continue;
        }

        utils.ClearScreen();
        map.DrawWithWindowView(viewW, player.GetX(), player.GetY(), offsetX, offsetY);
        hud.Draw(player, 1, map.GetWidth(), aciertos, secondsLeft);

        utils.PrintAtPosition(startX, mensajeY, ejercicioActual.pregunta, BLUE);

        for (int i = 0; i < 3; ++i)
        {
            int number = i + 1;
            std::wstring respuesta = ToWString(number) + L") " + ejercicioActual.opciones[i];
            int posX = startX + (i * 20);
            utils.PrintAtPosition(posX, mensajeY + 2, respuesta, YELLOW_BRIGHT);
        }

        if (_kbhit())
            ProceesInput(_getch(), consoleSettings);

        Sleep(50);
    }
    consoleSettings.SetConsoleFont();
    return result;
}

void Elevator::ProceesInput(char input, Console consoleSettings)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return; // Invalid key

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;

    wchar_t tile = map.GetTile(newX, newY);
    // utils.PrintAtPosition(0, 0, std::wstring(L"Tile: ") + tile, GREEN); // para ver qué hay ahí

    if (tile == ' ' || tile == '|')
    {
        player.SetPosition(newX, newY);
    }

    /*if (tile == L'-')
    {
        map.SetTile(newX, newY, L'|'); // Abrir automáticamente
    }*/
    int posY = mensajeY + 4;
    if (tile == L'1' || tile == L'2' || tile == L'3')
    {
        int respuestaJugador = tile - L'1'; // convierte '1','2','3' → 0,1,2
        if (respuestaJugador == ejercicioActual.respuestaCorrecta)
        {

            utils.PrintAtPosition(2, posY, L"Correct", GREEN);
            Sleep(500);
            utils.PrintAtPosition(2, posY, L"               ", RESET);
            player.SetPosition(2, 2);
            aciertos++;
        }
        else
        {
            utils.PrintAtPosition(2, posY, L"Incorrect", RED);
            Sleep(500);
            utils.PrintAtPosition(2, posY, L"               ", RESET);
            player.SetPosition(2, 2);
            Sleep(100);
            lives--;
            player.SetLives(lives);
        }

        Sleep(1000);

        if (aciertos >= 3)
        {
            result = true;     // Indicar que todavia puede continuar con los demás juegos
            isRunning = false; // parar el juego
        }

        else if (lives <= 0)
        {
            result = false;    // Indicar que NO puede continuar con los demás jeugos
            isRunning = false; // parar el juego
        }

        ejercicioActual = GenerarEjercicio(); // siguiente ejercicio
        startTime = std::chrono::steady_clock::now();
    }
}

void Elevator::LoadLevel(std::string key)
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

void Elevator::ShowLoadingAnimation(Console consoleSettings, int cycles = 3, int delay = 300)
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

void Elevator::SetGoodStyle(Console consoleSettings)
{
    ShowLoadingAnimation(consoleSettings);
    Sleep(300);
    consoleSettings.SetConsoleFont(16, 22, L"Lucida console");
    Sleep(100);
}
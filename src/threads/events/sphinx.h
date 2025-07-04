#pragma once
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/sphinx/hudSphinx.h"

struct PreguntaGeografia
{
    std::wstring pregunta;
    std::wstring opciones[3];
    int respuestaCorrecta;
};

PreguntaGeografia GenerarPregunta()
{
    int tipo = rand() % 3;
    PreguntaGeografia p;

    switch (tipo)
    {
    case 0:
        p.pregunta = L"¿Cuál es el río más largo del mundo?";
        p.opciones[0] = L"Amazonas";
        p.opciones[1] = L"Nilo";
        p.opciones[2] = L"Yangtsé";
        p.respuestaCorrecta = 1;
        break;
    case 1:
        p.pregunta = L"¿Cuál es la capital de Australia?";
        p.opciones[0] = L"Sídney";
        p.opciones[1] = L"Canberra";
        p.opciones[2] = L"Melbourne";
        p.respuestaCorrecta = 1;
        break;
    case 2:
        p.pregunta = L"¿Dónde está ubicada la cordillera de los Andes?";
        p.opciones[0] = L"Asia";
        p.opciones[1] = L"Europa";
        p.opciones[2] = L"Sudamérica";
        p.respuestaCorrecta = 2;
        break;
    }

    return p;
}

class SphinxGame
{
public:
    SphinxGame();
    bool Run(Console consoleSettings);

private:
    Map map;
    Utils utils;
    Player player;
    HUDSphinx hud;

    int viewWidth = 36; // Se puede cambiar dinámicamente según mapa
    int viewHeight = 18;

    int playerX;
    int playerY;
    bool isRunning;
    int lives;
    int aciertos;
    int intentos;
    int mensajeX;
    int mensajeY;

    int offsetX = 2, offsetY = 1;

    PreguntaGeografia preguntaActual;

    void LoadLevel(std::string key);
    void SetGoodStyle(Console consoleSettings);
    void ProceesInput(char input);
};

SphinxGame::SphinxGame() : isRunning(true), intentos(3)
{
    player.SetLives(3);
    lives = player.GetLives();
    aciertos = 0;
    preguntaActual = GenerarPregunta();
}

bool SphinxGame::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings);

    std::string key = utils.GetAssetsPath() + "maps\\sphinx\\sphinx.txt";
    LoadLevel(key);

    // Actualizamos viewWidth para el ancho total del mapa
    viewWidth = map.GetWidth();

    // Calculamos offsetX para centrar el mapa en consola (asumiendo utils.GetConsoleWidth() da ancho)
    offsetX = (utils.GetConsoleWidth() - viewWidth) / 2;
    if (offsetX < 0)
        offsetX = 0; // Para evitar negativo

    // Inicializamos mensajeX y mensajeY
    mensajeX = offsetX;
    mensajeY = offsetY + viewHeight + 2;

    hud.SetCenteredOffset(offsetX); // Alinea HUD a la derecha del mapa

    utils.ClearScreenComplety();
    hud.Draw(player, 1, viewWidth); // usa viewWidth como ancho del mapa

    while (isRunning)
    {
        // Dibuja todo el mapa centrado, con jugador
        map.DrawWithPlayerSphinx(
            map.GetWidth(), map.GetHeight(),
            player.GetX(), player.GetY(),
            offsetX, offsetY);

        // Limpia área de preguntas
        for (int i = 0; i < 5; ++i)
            utils.PrintAtPosition(mensajeX, mensajeY + i, std::wstring(viewWidth, L' '), RESET);

        // Pregunta centrada
        int startX = mensajeX + (viewWidth - (int)preguntaActual.pregunta.size()) / 2;
        utils.PrintAtPosition(startX, mensajeY, preguntaActual.pregunta, BLUE);

        // Opciones de respuesta
        for (int i = 0; i < 3; ++i)
        {
            std::wstring texto = ToWString(i + 1) + L") " + preguntaActual.opciones[i];
            int xPos = mensajeX + 4 + i * 10;
            utils.PrintAtPosition(xPos, mensajeY + 2, texto, YELLOW_BRIGHT);
        }

        if (_kbhit())
        {
            ProceesInput(_getch());
            hud.Draw(player, 1, viewWidth); // redibuja HUD al responder
        }

        Sleep(50);
    }

    consoleSettings.SetConsoleFont();
    return true;
}

void SphinxGame::ProceesInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first;
    int dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;
    wchar_t tile = map.GetTile(newX, newY);
    // --- Bloque de colisión ---
    // Solo se puede mover si la casilla NO es A ni B
    if (tile != L'A' && tile != L'B' && tile != L'#')
    {
        player.SetPosition(newX, newY);
    }

    if (tile == L'1' || tile == L'2' || tile == L'3')
    {
        int respuestaJugador = tile - L'1';

        if (respuestaJugador == preguntaActual.respuestaCorrecta)
        {
            utils.PrintAtPosition(offsetX, mensajeY + 5, L"¡Correcto!", GREEN);
            Sleep(700);
            aciertos++;
            player.SetPosition(2, 2);
            intentos = 3;
        }
        else
        {
            utils.PrintAtPosition(offsetX, mensajeY + 5, L"Incorrecto...", RED);
            Sleep(700);
            intentos--;
            if (intentos <= 0)
            {
                lives--;
                player.SetLives(lives);
                intentos = 3;
            }
            player.SetPosition(2, 2);
        }

        if (aciertos >= 3)
        {
            utils.ClearScreen();
            std::wcout << L"\n\n\n¡Ganaste el desafío de la esfinge!\n";
            isRunning = false;
        }
        else if (lives <= 0)
        {
            utils.ClearScreen();
            std::wcout << L"\n\n\nPerdiste. La esfinge ha ganado.\n";
            isRunning = false;
        }

        preguntaActual = GenerarPregunta();
    }
}

void SphinxGame::LoadLevel(std::string key)
{
    utils.ClearScreenComplety();
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY());
    playerX = player.GetX();
    playerY = player.GetY();
}

void SphinxGame::SetGoodStyle(Console consoleSettings)
{
    consoleSettings.SetConsoleFont(8, 12, L"Lucida Console");
    utils.ClearScreenComplety();
    utils.PrintCentered(L"La esfinge te desafía con preguntas de geografía...");
    Sleep(1300);
}

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

    int viewWidth = 36;
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

    viewWidth = map.GetWidth();

    // Centrar horizontal y vertical
    offsetX = (utils.GetConsoleWidth() - viewWidth) / 2 - 4;
    if (offsetX < 0)
        offsetX = 0;

    offsetY = (utils.GetConsoleHeight() - map.GetHeight()) / 2 - 1;
    if (offsetY < 0)
        offsetY = 0;

    mensajeX = offsetX;
    mensajeY = offsetY + map.GetHeight() + 2;

    hud.SetCenteredOffset(offsetX);

    utils.ClearScreenComplety(); // Solo limpio una vez antes del loop
    hud.Draw(player, 1, viewWidth);

    while (isRunning)
    {
        // Solo limpiar la pantalla UNA vez al inicio o cuando ganes o pierdas
        // utils.ClearScreen();  <-- quitar de aquí para que no limpie todo

        // Dibujar mapa y jugador
        map.DrawWithPlayerSphinx(
            map.GetWidth(), map.GetHeight(),
            player.GetX(), player.GetY(),
            offsetX, offsetY);

        hud.Draw(player, 1, viewWidth);

        // Mostrar pregunta y opciones
        int startX = mensajeX + (viewWidth - (int)preguntaActual.pregunta.size()) / 2;
        utils.PrintAtPosition(startX, mensajeY, preguntaActual.pregunta, BLUE);

        for (int i = 0; i < 3; ++i)
        {
            std::wstring texto = ToWString(i + 1) + L") " + preguntaActual.opciones[i];
            int xPos = mensajeX + 4 + i * 10;
            utils.PrintAtPosition(xPos, mensajeY + 2, texto, YELLOW_BRIGHT);
        }

        // Detectar tecla sin bloquear, mover si hay input
        if (_kbhit())
        {
            char input = _getch();
            ProceesInput(input);
        }

        // Pausa mínima para no saturar el CPU (muy pequeña)
        Sleep(20);
    }
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

    player.SetPosition(14, 17); // Aquí pones X,Y donde quieras

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

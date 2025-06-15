#ifndef MAIN_BOMBERMAN_H
#define MAIN_BOMBERMAN_H

// Archivos de cabecera necesarios
#include "utils/settings/utils.h"
#include "mini-games/bomberman/hudBomberman.h"
#include "utils/settings/maping.h"
#include "utils/settings/player.h"
#include "mini-games/bomberman/bombRenderer.h"
#include "mini-games/bomberman/bomb.h"

#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>

#ifdef _WIN32
#include <windows.h> // Solo en Windows para sonidos y consola
#endif

const int MAX_BOMBS = 25; // Número máximo de bombas en juego

// Clase principal que controla el juego estilo Bomberman
class MainBomberman
{
public:
    MainBomberman(); // Constructor del juego

    // Ejecuta el ciclo principal del juego
    // Retorna true si pasa todos los niveles, false si pierde
    bool Run();

private:
    // Componentes del juego
    Map map;
    Player &player = Player::GetInstance();
    HUDBomberman hud;
    Utils utils;
    BombRenderer bombRenderer;
    std::string difficultyFolder;

    Bomb bombs[MAX_BOMBS]; // Arreglo de bombas
    int bombCount = 0;     // Número actual de bombas activas

    int currentLevel; // Nivel actual
    bool isRunning;   // Estado de ejecución del juego

    int offsetX = 1, offsetY = 1; // Offset para el render del mapa en pantalla
    std::string currentMapName;   // Ruta del mapa actual

    // Métodos auxiliares
    void processInput(char input); // Procesa entrada del jugador
    void LoadLevel(int level);     // Carga un nuevo nivel
    void handleExplosion(int i);   // Maneja explosión de la bomba i
    void DetermineDifficultyFolder();
};

// Constructor: inicializa estado y carga primer nivel
MainBomberman::MainBomberman() : currentLevel(1), isRunning(true), bombCount(0)
{
}

// Método principal de ejecución del juego
bool MainBomberman::Run()
{
    utils.ClearScreen();
    player.ActivateControlB(true); // Activa control para colocar bombas
    // Determina la carpeta correcta según dificultad
    DetermineDifficultyFolder();
    // Carga el primer nivel
    LoadLevel(currentLevel);

    while (isRunning)
    {
        utils.ClearScreen(); // Limpia consola

        // Dibuja mapa, jugador, bombas y HUD
        map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), player.GetX(), player.GetY(), offsetX, offsetY);
        bombRenderer.Draw(bombs, bombCount, 1, 1);
        hud.Draw(player, currentLevel, map.GetWidth());

        // Verifica bombas que ya deben explotar
        for (int i = 0; i < bombCount;)
        {
            if (bombs[i].HasExploded())
            {
                handleExplosion(i); // Explosión y limpieza

                // Compacta arreglo al eliminar bomba
                for (int j = i; j < bombCount - 1; ++j)
                    bombs[j] = bombs[j + 1];
                --bombCount;
            }
            else
            {
                ++i;
            }
        }

        // Captura entrada del usuario si hay
        if (_kbhit())
            processInput(_getch());

        // Verifica condición de derrota
        if (player.GetLives() <= 0)
        {
            utils.ClearScreen();
            std::cout << "\nHas perdido todas tus vidas. ¡Game Over!\n";
            isRunning = false;
            return false; // El jugador perdió
        }

        utils.Sleep(30); // Delay entre frames
    }

    return true; // Si se completa correctamente
}

// Procesa entrada del jugador
void MainBomberman::processInput(char input)
{
    // Coloca bomba si se presiona 'b'
    if (input == 'b' && player.IsControlBActive())
    {
        if (player.CanPlaceBomb() && (bombCount < MAX_BOMBS))
        {
            bombs[bombCount++] = Bomb(player.GetX(), player.GetY());
            map.SetTile(player.GetX(), player.GetY(), '0'); // Marca bomba en mapa
            player.PlaceBomb();
        }
        return;
    }

    // Movimiento y posible cambio de nivel
    bool changeLevel = player.TryMove(input, map);
    if (changeLevel)
    {
        int nextLevel = currentLevel + 1;

        // Determinar la ruta del siguiente mapa

        switch (player.GetDifficulty())
        {
        case Player::Difficulty::EASY:
            difficultyFolder = "easy-levels";
            break;
        case Player::Difficulty::NORMAL:
            difficultyFolder = "medium-levels";
            break;
        case Player::Difficulty::HARD:
            difficultyFolder = "hard-levels";
            break;
        }

        std::string nextMapPath = "src/maps/maps-bomberman/" + difficultyFolder + "/level" + std::to_string(nextLevel) + ".txt";

        // Verificar si existe el siguiente nivel
        std::ifstream file(nextMapPath);
        if (!file.is_open())
        {
            utils.ClearScreen();
            std::cout << "\n¡Felicidades! Has completado todos los niveles.\n";
            isRunning = false;
            return; // Finaliza el juego exitosamente
        }

        // Si existe, cargar el siguiente nivel
        currentLevel = nextLevel;
        LoadLevel(currentLevel);
    }
}

// Carga el mapa de un nivel determinado
void MainBomberman::LoadLevel(int level)
{
    // Define carpeta según dificultad
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        difficultyFolder = "easy-levels";
        break;
    case Player::Difficulty::NORMAL:
        difficultyFolder = "medium-levels";
        break;
    case Player::Difficulty::HARD:
        difficultyFolder = "hard-levels";
        break;
    }

    // Construye la ruta al archivo del mapa
    currentMapName = "src/maps/maps-bomberman/" + difficultyFolder + "/level" + std::to_string(level) + ".txt";
    std::ifstream file(currentMapName);

    // Si el archivo existe, lo carga
    utils.ClearScreenComplety();

    map.ReadMap(currentMapName, map.GetWidth(), map.GetHeight());
    bombCount = 0;                                        // Reinicia bombas
    player.SetPosition(map.GetSpawnX(), map.GetSpawnY()); // Coloca al jugador en spawn
}

// Maneja el efecto de una explosión de bomba
void MainBomberman::handleExplosion(int i)
{
    int x = bombs[i].GetX();
    int y = bombs[i].GetY();

    // Lambda que aplica la explosión en una dirección
    auto applyExplosion = [&](int dx, int dy)
    {
        int nx = x + dx, ny = y + dy;
        char tile = map.GetTile(nx, ny);

        // Si el jugador está en la zona, pierde una vida
        if (nx == player.GetX() && ny == player.GetY())
            player.LoseLife();

        // Si el tile es rompible, lo destruye o genera ítem
        if (tile != '#' && tile != '~' && tile != ']' && tile != '/' && tile != '0')
        {
            if (tile == '%')
                map.SetTile(nx, ny, (rand() % 4 == 0) ? 'B' : '*'); // 25% chance de 'B'
            else
                map.SetTile(nx, ny, '*');
        }
    };

    // Explosión en cruz
    applyExplosion(0, 0);  // Centro
    applyExplosion(0, -1); // Arriba
    applyExplosion(0, 1);  // Abajo
    applyExplosion(-1, 0); // Izquierda
    applyExplosion(1, 0);  // Derecha

#ifdef _WIN32
    // Sonido de explosión en Windows
    Beep(800, 50);
    Beep(600, 50);
    Beep(300, 100);
#endif

    // Redibuja tras explosión
    utils.ClearScreen();
    hud.Draw(player, currentLevel, map.GetWidth());
    map.DrawWithPlayer(map.GetWidth(), map.GetHeight(), player.GetX(), player.GetY(), offsetX, offsetY);
    bombRenderer.Draw(bombs, bombCount, 1, 1);
    utils.Sleep(30);

    // Limpia llamas de la explosión
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            // Solo direcciones cardinales (no diagonales)
            if ((dx == 0 || dy == 0) && !(dx != 0 && dy != 0))
            {
                int nx = x + dx, ny = y + dy;
                char tile = map.GetTile(nx, ny);
                if (tile == '*' || tile == '0')
                    map.SetTile(nx, ny, ' ');
            }
        }
    }
}

void MainBomberman::DetermineDifficultyFolder()
{
    switch (player.GetDifficulty())
    {
    case Player::Difficulty::EASY:
        difficultyFolder = "easy-levels";
        break;
    case Player::Difficulty::NORMAL:
        difficultyFolder = "medium-levels";
        break;
    case Player::Difficulty::HARD:
        difficultyFolder = "hard-levels";
        break;
    }
}

#endif

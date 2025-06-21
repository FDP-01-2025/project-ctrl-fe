#pragma once

//* Todos los headers
#include "./src/core/engine/settings/console.h"
#include "./src/utils/player/maping.h"
#include "./src/utils/player/player.h"
#include "./src/threads/startup/introductionMenu.h"
#include "./src/threads/startup/difficultiesMenu.h"
#include "./src/threads/startup/splashScreen.h"
#include "./src/threads/startup/storyBoard.h"
#include "./src/threads/games/bomberman.h"
#include "./src/threads/games/maze.h"
#include "./src/threads/events/geniousLamp.h"
#include "./src/utils/screen/multiColors.h"

class Global
{
protected:
    int consoleW;
    int consoleH;

    int mapW;
    int mapH;

    wchar_t chars[100][100] = {};
    int colors[100][100] = {};

    Console consoleSettings;
    Map mapSettings;
    Player player;
    MainBomberman bomberman;
    MainMaze maze;
    MainGenious geniusLamp;
    Utils utils;

    enum ProcessState
    {
        STATE_NOT_STARTED = 0,
        STATE_INITIALIZED = 1,
        STATE_STORYBOARD_SHOWN = 2,
        STATE_INITIAL_MENU_DONE = 3,
        STATE_SECOND_MENU_DONE = 4,
        STATE_GAME_STARTED = 5,
        STATE_FIRST_INIT = 6,
        STATE_SECOND_INIT = 7,

    };

    ProcessState processThread = STATE_NOT_STARTED;
    // Esta es una variable que representa el proceso de ejecucion del jeugo, representa el estado del hilo principal

public:
    //* Valores iniciales
    Global(int w = 100, int h = 45, int mW = 70, int mH = 20)
        : consoleW(w), consoleH(h), consoleSettings(w, h), mapW(mW), mapH(mH) {}

    // TODO ----- PROCESO (1) ----
    void Initializer()
    {
        if (processThread == STATE_NOT_STARTED)
        {
            consoleSettings.ConfigConsole();
            consoleSettings.SetTitle(L"Dungeon of leguim"); // Título

            Sleep(500);

            /*char chars[100][100] = {};
            int colors[100][100] = {};
            srand((unsigned)time(0)); // Semilla única, una vez al inicio

            processThread = InitializerThread(chars, colors, 100, 45, 4) ? STATE_INITIALIZED : STATE_NOT_STARTED;

            if (processThread == STATE_NOT_STARTED)
                std::cout << "Initializer no se completo";*/

            processThread = STATE_INITIALIZED;
        }
    }
    // TODO ----- PROCESO (2) ----
    void ReadPlayerStatusPoint()
    {
        if (processThread == STATE_INITIALIZED)
        {
            // Leer el file (osea, la base de datos trucha)
            int status = 0; // 0 = primera ejecución status = el estado dependiendo de lo que se lea del file
            // Después de identificar el estado las acciones

            switch (status)
            {
            case 0: // Si es 0 entonces podemos poner que el processThread sea STATE_FIRST_INIT (para mostrar story board y etc)
                processThread = STATE_FIRST_INIT;
                break;
            case 1: // Si es 1 entonces podemos poner processThread = STATE_SECOND_INIT (para YA NO mostrar el storyboard)
                processThread = STATE_SECOND_INIT;
                break;
            default:
                break;
            }
        }
    }
    // TODO ----- PROCESO (3) ----
    void SetStoryBoard()
    {
        switch (processThread)
        {
        case STATE_FIRST_INIT:
            // LOGICA de mostrar el story board
            processThread = ShowStoryBoard() ? STATE_STORYBOARD_SHOWN : STATE_NOT_STARTED;
            if (processThread == STATE_NOT_STARTED)
                std::wcout << L"storyBoard no se completo";
            break;
        case STATE_SECOND_INIT:
            // Si ya se mostró, toncs no hacer nada :p y solo poner que ya se mostró
            processThread = STATE_STORYBOARD_SHOWN;
            break;
        default:
            break;
        }
    }

    // TODO ----- PROCESO (4) ----
    void ShowInitialMenu(const wchar_t *title)
    {
        if (processThread == STATE_STORYBOARD_SHOWN)
        {
            consoleSettings.SetTitle(title);
            switch (StartMenu()) // Aqui se manda a llamar la logica del menu que devolveria un numero dependiendo de lo que escoga
            {
            case 1: // Si le dio a jugar entonces? (deberia borrar cualquier partida anterior que tenga si es que tiene o no para comenzar de 0)
                    // Si sale bien entonces:
                processThread = STATE_INITIAL_MENU_DONE;
                ShowSecondMenu();
                break;
            case 2: // Si le dio a continuar es que tiene partida guardada, entonces no es necesario mostrar el segundo menú
                processThread = STATE_SECOND_MENU_DONE;
                break;
            default:
                processThread = STATE_NOT_STARTED;
                if (ConfirmExitMenu())
                {
                    exit(0); // Finaliza el juego
                }
                else
                {
                    // El usuario no quiso salir, se queda en el menú
                    processThread = STATE_STORYBOARD_SHOWN;
                }
                break;
            }
        }
    }
    // TODO ----- PROCESO (5) ----
    int selectedDifficulty = 0;

    void ShowSecondMenu()
    {
        switch (processThread)
        {
        case STATE_INITIAL_MENU_DONE:

            selectedDifficulty = StartSecondMenu(); //
            if (selectedDifficulty >= 1 && selectedDifficulty <= 3)
            {
                processThread = STATE_GAME_STARTED;
            }
            break;
        case STATE_SECOND_MENU_DONE:
            // No muestra el menu porque se supone que ya tiene partida guardada con la dificultad ya puesta
            processThread = STATE_SECOND_MENU_DONE;
            break;

        default:
            break;
        }
    }
    // TODO ----- PROCESO (6) ----
    void StartGame()
    {
        // En este punto ya habiramos: Mostrado el logo, Leer el estado del usuario, mostrar la historia y mostrar ambos menus
        if (processThread == STATE_GAME_STARTED)
        {
            utils.ClearScreenComplety(); // Limpia la pantalla antes de iniciar el juego
            switch (selectedDifficulty)
            {
            case 1:
                player.removeStatusFile();
                player.ResetState(Player::EASY);
                //maze.Run();
                // bomberman.Run();
                geniusLamp.Run();
                break;

            case 2:
                player.removeStatusFile();
                player.ResetState(Player::NORMAL);
                //bomberman.Run();
                geniusLamp.Run();
                break;

            case 3:
                // Iniciar modo difícil
                player.removeStatusFile();
                player.ResetState(Player::HARD);
                //bomberman.Run();
                geniusLamp.Run();
                break;

            default:
                std::wcout << L"Dificultad no válida\n";
                break;
            }
        }
    }
};

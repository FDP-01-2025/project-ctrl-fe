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
#include "./src/threads/bosses/bossFightMario.h"
#include "./src/threads/bosses/bossFightZelda.h"
#include "./src/utils/screen/multiColors.h"
#include "./src/threads/startup/mainRoom.h"
#include "./src/threads/games/worm.h"
#include "./src/threads/events/elevator.h"
#include "./src/threads/events/sphinx.h"
#include "./src/threads/startup/way1.h"
#include "./src/threads/startup/bossSalaPrev.h"
#include <set>

class Global
{
protected:
    int consoleW;
    int consoleH;

    int mapW;
    int mapH;

    Console consoleSettings;
    Map mapSettings;
    Player player;
    MainBomberman bomberman;
    MainMaze maze;
    SphinxGame sphinx;
    MainGenious geniusLamp;
    Utils utils;
    MainBossFight bossFightMario;
    MainRoomGame mainRoom;
    MainBossFightZelda bossFightZelda;
    Worm worm;
    Elevator elevator;

    MapId currentMap;
    Way1 frstWay;
    BossSalaPrev bossSalaPrev;
    MapId selected;
    MapId opcionesGames[3];
    MapId opcionesBoses[2];

    int showMapsLot, showBoossLot;

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
    // Esta es una variable que representa el proceso de ejecucion del juego, representa el estado del hilo principal
    // Tamaño horizontal de la vista
    int sizeViewW = 40;
    const std::string filename = utils.GetAssetsPath() + "data\\GamesCounter.txt", filenameBoss = utils.GetAssetsPath() + "data\\BossesCounter.txt";

public:
    //* Valores iniciales
    Global(int w = 100, int h = 45, int mW = 70, int mH = 20)
        : consoleW(w), consoleH(h), consoleSettings(w, h), mapW(mW), mapH(mH)
    {
        currentMap = MapId::FrstWay;
    }

    // TODO ----- PROCESO (1) ----
    void Initializer()
    {
        if (processThread == STATE_NOT_STARTED)
        {
            consoleSettings.ConfigConsole();
            consoleSettings.SetTitle(L"Dungeon of leguim"); // Title

            processThread = InitializerThread(consoleW, consoleH, consoleSettings, utils) ? STATE_INITIALIZED : STATE_NOT_STARTED;

            if (processThread == STATE_NOT_STARTED)
                std::cout << "Initializer no se completo";
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
    // TODO ----- PROCESO (6) ----
    void StartGame()
    {
        if (processThread != STATE_GAME_STARTED)
            return;

        player.removeStatusFile();
        utils.ClearScreenComplety();

        player.ResetState(SetDificultyDetails());

        if (!ChangeMap(currentMap))
            return;

        if (!GamesExecute())
            return;

        if (counterMaps == showMapsLot)
            BossesExecute();

        if (counterBoss == showBoossLot)
        {
            // TODO: Mostrar final del juego
        }
    }

    bool GamesExecute()
    {
        MapId allGames[] = {BomberManGame, MazeGame, GeniusGame, WormGame, ElevatorGame};
        const int totalGames = sizeof(allGames) / sizeof(allGames[0]);

        wofstream gamesCompleted(filename, std::ios::app);
        wifstream gamesLecture;

        counterMaps = 0;

        while (counterMaps < showMapsLot)
        {
            if (player.GetLives() == 0)
            {
                // TODO mostrar mensaje de pérdida
                return false;
            }

            std::set<int> gamesAlreadyPlayed;
            ReadFileGamesId(filename, gamesAlreadyPlayed);

            std::vector<MapId> gamesNotPlayed;
            ReduceGamesPlayed(allGames, totalGames, gamesAlreadyPlayed, gamesNotPlayed);

            if (gamesNotPlayed.empty())
            {
                utils.ClearScreenComplety();
                std::wcout << L"No quedan juegos disponibles.\n";
                Sleep(1000);
                return false;
            }

            // LIMPIAR y generar nuevas opciones
            for (int i = 0; i < 3; ++i)
                opcionesGames[i] = MapId::None;

            int cantidadOpciones = std::min(3, (int)gamesNotPlayed.size());
            GenerateRandomMapId(gamesNotPlayed.data(), gamesNotPlayed.size(), opcionesGames, cantidadOpciones);

            selected = mainRoom.Run(consoleSettings, opcionesGames);

            bool valid = false;
            for (int i = 0; i < cantidadOpciones; ++i)
                if (selected == opcionesGames[i])
                    valid = true;

            if (valid && ChangeMap(selected))
            {
                gamesCompleted << static_cast<int>(selected) << std::endl;
                counterMaps++;
            }

            Sleep(50);
        }

        return true;
    }

    void BossesExecute()
    {
        MapId allBosses[] = {BoosMario, BoosZelda};
        const int totalBosses = sizeof(allBosses) / sizeof(allBosses[0]);

        wofstream bossesCompleted(filenameBoss, std::ios::app);

        std::set<int> bossesAlreadyPlayed;
        ReadFileGamesId(filenameBoss, bossesAlreadyPlayed);

        counterBoss = 0;

        while (counterBoss < showBoossLot)
        {
            if (!ChangeMap(MapId::BoosSalaPrev))
                return;

            std::vector<MapId> bossesNotPlayed;
            ReduceGamesPlayed(allBosses, totalBosses, bossesAlreadyPlayed, bossesNotPlayed);

            if (bossesNotPlayed.empty())
            {
                utils.ClearScreenComplety();
                std::wcout << L"No quedan jefes disponibles.\n";
                Sleep(1000);
                return;
            }

            GenerateRandomMapId(bossesNotPlayed.data(), bossesNotPlayed.size(), opcionesBoses, 3);

            for (int i = 0; i < 2 && i < bossesNotPlayed.size(); ++i)
                opcionesBoses[i] = bossesNotPlayed[i];

            selected = bossSalaPrev.Run(consoleSettings, opcionesBoses);
            bossesCompleted << static_cast<int>(selected) << std::endl;

            ChangeMap(selected);
            counterBoss++;
        }
    }

    Player::Difficulty SetDificultyDetails()
    {
        switch (selectedDifficulty)
        {
        case 1:
            currentDificulti = Player::EASY;
            showMapsLot = 3;
            showBoossLot = 1;
            break;
        case 2:
            currentDificulti = Player::NORMAL;
            showMapsLot = 5;
            showBoossLot = 1;
            break;
        case 3:
            currentDificulti = Player::HARD;
            showMapsLot = 6;
            showBoossLot = 1;
            break;
        default:
            std::wcout << L"Dificultad no válida\n";
            break;
        }
        return currentDificulti;
    }

    void ReadFileGamesId(const std::string &filename, std::set<int> &conjunto)
    {
        std::wifstream file(filename);
        int id;
        while (file >> id)
            conjunto.insert(id);
    }

    void ReduceGamesPlayed(MapId all[], int total, const std::set<int> &jugados, std::vector<MapId> &noJugados)
    {
        for (int i = 0; i < total; ++i)
        {
            if (jugados.find(static_cast<int>(all[i])) == jugados.end())
            {
                noJugados.push_back(all[i]);
            }
        }
    }

    bool ChangeMap(MapId map)
    {
        switch (map)
        {
        case FrstWay:
            return frstWay.Run(consoleSettings);

        case BomberManGame:
            return bomberman.Run();

        case MazeGame:
            return maze.Run();

        case GeniusGame:
            return geniusLamp.Run();

            /*case ChestGame:
                chestGame.Run();
                return true;*/

        case WormGame:
            return worm.Run(consoleSettings);

        case ElevatorGame:
            return elevator.Run(consoleSettings);

        case BoosMario:
            bossFightMario.Run();
            return true;

        case BoosZelda:
            bossFightZelda.Run();
            return true;

        default:
            std::wcout << L"Mapa desconocido: " << map << opcionesGames[0] << std::endl;
            return false;
        }
    }

    void GenerateRandomMapId(MapId *arr, int size)
    {
        for (int i = size - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            MapId temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
};

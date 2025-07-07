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
#include "./src/threads/startup/gameOver.h"
#include "./src/threads/events/chest.h"


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
    ChestEvent chestGame;
    Utils utils;
    MainBossFight bossFightMario;
    MainRoomGame mainRoom;
    MainBossFightZelda bossFightZelda;
    Worm worm;
    Elevator elevator;
    GameOverScreen gameOver;
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
    Player::Difficulty currentDificulti;
    int counterMaps = 0, counterBoss = 0;

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
            int choice = StartMenu(consoleSettings, utils);
            switch (choice)
            {
            case 1: // Jugar
                processThread = STATE_INITIAL_MENU_DONE;
                ShowSecondMenu();
                break;
            case 2: // Continuar
                processThread = STATE_SECOND_MENU_DONE;
                break;
            case 3: // Salir
                if (ConfirmExitMenu(consoleSettings, utils))
                    exit(0);
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

            selectedDifficulty = StartSecondMenu(consoleSettings, utils);
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
    // Starts the main game flow depending on the current state of the thread.
    void StartGame()
    {
        // Check if the game state is starting from the very beginning
        if (processThread == STATE_GAME_STARTED)
        {
            // Remove previous saved files (if any)
            std::remove(filename.c_str());
            std::remove(filenameBoss.c_str());

            // Remove player status file (if it exists)
            player.removeStatusFile();

            // Clear the entire console screen
            utils.ClearScreenComplety();

            // Reset the player's state with new difficulty settings
            player.ResetState(SetDificultyDetails());
        }
        // If starting from the manual load menu (continue game)
        else if (processThread == STATE_SECOND_MENU_DONE)
        {
            // Load manually saved player state
            player.LoadStateManual();

            // Set the difficulty based on loaded data (+1 for indexing or internal adjustment)
            selectedDifficulty = player.GetDifficulty() + 1;

            // Apply the selected difficulty settings
            SetDificultyDetails();
        }
        // If not in any of the above valid states, exit the function
        else
        {
            return;
        }

        // Attempt to change the map to the current one; if it fails, exit
        if (!ChangeMap(currentMap))
            return;

        // Execute the list of available games (minigames); if any error occurs, exit
        if (!GamesExecute())
            return;

        // If the required number of maps have been played, launch the boss sequence
        if (counterMaps == showMapsLot)
            BossesExecute();

        // If enough bosses have been defeated, show the end of the game (not implemented yet)
        if (counterBoss == showBoossLot)
        {
            // TODO: Display end of the game
        }
    }

    bool GamesExecute()
    {
        MapId allGames[] = {BomberManGame, MazeGame, GeniusGame, WormGame, ElevatorGame, ChestGame};
        const int totalGames = sizeof(allGames) / sizeof(allGames[0]);

        // Open the output file stream in append mode to store the list of completed games.
        // This allows new game IDs to be added without erasing the existing content.
        wofstream gamesCompleted(filename, std::ios::app);

        // Open the input file stream in append mode to read the list of previously completed games.
        // NOTE: Using std::ios::app here for reading is unusual — normally you use std::ios::in for input.
        wifstream gamesLecture(filename, std::ios::app);

        // Check if the game was loaded from a previously saved state (manual load)
        if (processThread == STATE_SECOND_MENU_DONE)
        {
            counterMaps = 0; // Initialize map counter to zero

            int value;
            // Read each integer (representing a game ID) from the file
            // Each successful read means one game was completed in the past
            while (gamesLecture >> value)
            {
                counterMaps++; // Increment for each game found in the file
            }
        }
        else
        {
            // If it's not a resumed game, start with zero maps played
            counterMaps = 0;
        }

        while (counterMaps < showMapsLot)
        {
            std::wcout << counterMaps;
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

            if (valid && ChangeMapAndCheck(selected))
            {
                gamesCompleted << static_cast<int>(selected) << std::endl;
                // Increment the player's room when a game is successfully played
                player.SetRoom(player.GetRoom() + 1);
                counterMaps++;

                if (counterMaps >= showMapsLot)
                    break;
            }
            else if (player.GetLives() == 0)
            {
                // Mostrar Game Over y preguntar
                bool restart = gameOver.Show(utils);
                if (restart)
                {
                    // Reinicia el estado del jugador y vuelve al menú o al inicio
                    player.ResetState(SetDificultyDetails());
                    return GamesExecute(); // o reinicia Global::StartGame()
                }
                else
                {
                    exit(0); // Sale del juego
                }
            }
            Sleep(50);
        }

        return true;
    }

    bool BossesExecute()
    {
        MapId allGames[] = {BoosMario, BoosZelda};
        const int totalGames = sizeof(allGames) / sizeof(allGames[0]);

        // Open the output file stream in append mode to store the list of completed games.
        // This allows new game IDs to be added without erasing the existing content.
        wofstream gamesCompleted(filenameBoss, std::ios::app);

        // Open the input file stream in append mode to read the list of previously completed games.
        // NOTE: Using std::ios::app here for reading is unusual — normally you use std::ios::in for input.
        wifstream gamesLecture(filenameBoss, std::ios::app);

        // Check if the game was loaded from a previously saved state (manual load)
        if (processThread == STATE_SECOND_MENU_DONE)
        {
            counterMaps = 0; // Initialize map counter to zero

            int value;
            // Read each integer (representing a game ID) from the file
            // Each successful read means one game was completed in the past
            while (gamesLecture >> value)
            {
                counterMaps++; // Increment for each game found in the file
            }
        }
        else
        {
            // If it's not a resumed game, start with zero maps played
            counterMaps = 0;
        }

        while (counterMaps < showBoossLot)
        {
            std::wcout << counterMaps;
            std::set<int> gamesAlreadyPlayed;
            ReadFileGamesId(filename, gamesAlreadyPlayed);

            std::vector<MapId> gamesNotPlayed;
            ReduceGamesPlayed(allGames, totalGames, gamesAlreadyPlayed, gamesNotPlayed);

            if (gamesNotPlayed.empty())
            {
                utils.ClearScreenComplety();
                std::wcout << L"No quedan jefes disponibles.\n";
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

            if (valid && ChangeMapAndCheck(selected))
            {
                gamesCompleted << static_cast<int>(selected) << std::endl;
                // Increment the player's room when a game is successfully played
                player.SetRoom(player.GetRoom() + 1);
                counterMaps++;

                if (counterMaps >= showMapsLot)
                    break;
            }
            else if (player.GetLives() == 0)
            {
                // Mostrar Game Over y preguntar
                bool restart = gameOver.Show(utils);
                if (restart)
                {
                    // Reinicia el estado del jugador y vuelve al menú o al inicio
                    player.ResetState(SetDificultyDetails());
                    return GamesExecute(); // o reinicia Global::StartGame()
                }
                else
                {
                    exit(0); // Sale del juego
                }
            }
            Sleep(50);
        }

        return true;
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
        // MapId allGames[] = {BomberManGame, MazeGame, GeniusGame, WormGame, ElevatorGame};
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

        case ChestGame:
            return chestGame.Run();

        case WormGame:
            return worm.Run(consoleSettings);

        case ElevatorGame:
            return elevator.Run(consoleSettings);

        case BoosMario:
            return bossFightMario.Run();

        case BoosZelda:
            return bossFightZelda.Run();

        case MainRoom:
            break;

        default:
            std::wcout << L"Mapa desconocido: " << map << opcionesGames[0] << std::endl;
            return false;
        }
    }

    bool ChangeMapAndCheck(MapId map)
    {
        bool result = ChangeMap(map);
        if (player.GetLives() == 0)
        {
            return false;
        }
        return result;
    }

    void GenerateRandomMapId(MapId *source, int sourceSize, MapId *dest, int count)
    {
        // Limpiar o asignar valor por defecto
        for (int i = 0; i < count; ++i)
            dest[i] = MapId::None; // o cualquier valor válido por defecto

        std::vector<MapId> pool(source, source + sourceSize);
        std::random_shuffle(pool.begin(), pool.end());

        for (int i = 0; i < count && i < pool.size(); ++i)
        {
            dest[i] = pool[i];
        }
    }
};

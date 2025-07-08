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
#include "./src/threads/startup/mainRoom.h"
#include "./src/threads/games/worm.h"
#include "./src/threads/events/elevator.h"
#include "./src/threads/events/sphinx.h"
#include "./src/threads/startup/way1.h"
#include "./src/threads/startup/bossSalaPrev.h"
#include "./src/threads/startup/gameOver.h"
#include "./src/threads/startup/Win.h"
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
    BossRoomPreview bossSalaPrev;
    MapId selected;
    MapId opcionesGames[3];
    MapId opcionesBoses[2];
    VictoryScreen victoryScreen;

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
    const std::string filename = utils.GetAssetsPath() + "data\\GamesCounter.txt", filenameBoss = utils.GetAssetsPath() + "data\\BossesCounter.txt", statusFileName = utils.GetAssetsPath() + "data\\status.txt";
    Player::Difficulty currentDificulti;
    int counterMaps = 0, counterBoss = 0;

public:
    //* Initial values constructor
    Global(int w = 100, int h = 45, int mW = 70, int mH = 20)
        : consoleW(w), consoleH(h), consoleSettings(w, h), mapW(mW), mapH(mH)
    {
        currentMap = MapId::FrstWay; // Set initial map to FrstWay
    }

    // TODO ----- PROCESS (1) ----
    // Initializes the console and sets up necessary configurations
    void Initializer()
    {
        // Only run if processThread has not started yet
        if (processThread == STATE_NOT_STARTED)
        {
            consoleSettings.ConfigConsole();                // Configure console settings (size, colors, etc.)
            consoleSettings.SetTitle(L"Dungeon of leguim"); // Set the console window title

            // Run the initializer thread and update processThread state based on success
            processThread = InitializerThread(consoleW, consoleH, consoleSettings, utils)
                                ? STATE_INITIALIZED
                                : STATE_NOT_STARTED;

            // If initialization failed, output an error message to standard output
            if (processThread == STATE_NOT_STARTED)
                std::cout << "Initializer did not complete";
        }
    }

    // TODO ----- PROCESO (2) ----
    void ReadPlayerStatusPoint()
    {
        if (processThread == STATE_INITIALIZED)
        {
            // Simulate reading a persistent "fake database" from a file
            int status = 0; // Default: first time

            // Check if the file exists
            std::ifstream checkFile(statusFileName);
            if (checkFile.good())
            {
                // File exists, not the first execution
                status = 1;
            }
            else
            {
                // File does not exist → first execution
                // Create the file to mark future runs
                std::ofstream createFile(statusFileName);
                if (createFile.is_open())
                {
                    createFile << "initialized";
                    createFile.close();
                }
                status = 0;
            }

            // Decide which state to enter depending on file presence
            switch (status)
            {
            case 0:
                processThread = STATE_FIRST_INIT; // First time: show story, tutorial, etc.
                break;
            case 1:
                processThread = STATE_SECOND_INIT; // Already initialized: skip intro
                break;
            default:
                break;
            }
        }
    }

    // TODO ----- PROCESS (3) -----
    // Handles displaying the storyboard based on the current process state
    void SetStoryBoard()
    {
        switch (processThread)
        {
        case STATE_FIRST_INIT:
            // Logic to show the storyboard for the first initialization
            processThread = ShowStoryBoard(consoleW, consoleH, consoleSettings, utils)
                                ? STATE_STORYBOARD_SHOWN
                                : STATE_NOT_STARTED;
            // If showing the storyboard failed, print an error message
            if (processThread == STATE_NOT_STARTED)
                std::wcout << L"Storyboard did not complete";
            break;

        case STATE_SECOND_INIT:
            // If storyboard was already shown, do nothing and mark it as shown
            processThread = STATE_STORYBOARD_SHOWN;
            break;

        default:
            // For any other state, no action needed
            break;
        }
    }

    // TODO ----- PROCESS (4) -----
    // Displays the initial menu and handles user choice
    void ShowInitialMenu(const wchar_t *title)
    {
        // Only show menu if storyboard has been displayed
        if (processThread == STATE_STORYBOARD_SHOWN)
        {
            consoleSettings.SetTitle(title);                // Set the console window title
            int choice = StartMenu(consoleSettings, utils); // Show the start menu and get user's choice

            switch (choice)
            {
            case 1:                                      // Play
                processThread = STATE_INITIAL_MENU_DONE; // Mark initial menu as done
                ShowSecondMenu();                        // Proceed to second menu
                break;
            case 2:                                     // Continue
                processThread = STATE_SECOND_MENU_DONE; // Mark second menu as done (skip initial menu)
                break;
            case 3:                                          // Exit
                if (ConfirmExitMenu(consoleSettings, utils)) // Ask user to confirm exit
                    exit(0);                                 // Exit the application
                break;
            }
        }
    }

    // TODO ----- PROCESS (5) -----
    // Handles displaying the second menu and setting the difficulty
    int selectedDifficulty = 0;

    void ShowSecondMenu()
    {
        switch (processThread)
        {
        case STATE_INITIAL_MENU_DONE:
            // Show the second menu to select difficulty
            selectedDifficulty = StartSecondMenu(consoleSettings, utils);

            // If a valid difficulty is selected, mark the game as started
            if (selectedDifficulty >= 1 && selectedDifficulty <= 3)
            {
                processThread = STATE_GAME_STARTED;
            }
            break;

        case STATE_SECOND_MENU_DONE:
            // Do not show the menu because a saved game with difficulty is assumed
            // Keep the state as is
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
            player.ResetState(SetDifficultyDetails());
        }
        // If starting from the manual load menu (continue game)
        else if (processThread == STATE_SECOND_MENU_DONE)
        {
            // Load manually saved player state
            player.LoadStateManual();

            // Set the difficulty based on loaded data (+1 for indexing or internal adjustment)
            selectedDifficulty = player.GetDifficulty() + 1;

            // Apply the selected difficulty settings
            SetDifficultyDetails();
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
        {
            if (BossesExecute())
            {
                bool restart = victoryScreen.Show(utils);
                if (restart)
                {
                    // Reset player state and return to menu or start
                    player.ResetState(SetDifficultyDetails());
                    counterMaps = 0;
                    counterBoss = 0;
                    StartGame(); // Or return to the main menu if preferred
                }
                else
                {
                    exit(0); // Exit the game
                }
            }
        }
    }

    bool GamesExecute()
    {
        // List of all available games
        MapId allGames[] = {BomberManGame, MazeGame, GeniusGame, ChestGame, WormGame, ElevatorGame, DodgeGame, SphinxGameM};
        const int totalGames = sizeof(allGames) / sizeof(allGames[0]);

        // Open output file stream in append mode to record completed games
        // This ensures existing data is preserved and new completions are appended
        std::wofstream gamesCompleted(filename, std::ios::app);

        // Open input file stream (note: using std::ios::app for reading is unusual, normally std::ios::in)
        std::wifstream gamesLecture(filename, std::ios::app);

        // If loading from a previously saved state (manual load)
        if (processThread == STATE_SECOND_MENU_DONE)
        {
            counterMaps = 0; // Reset map counter

            int value;
            // Read each completed game ID from file and count them
            while (gamesLecture >> value)
            {
                counterMaps++;
            }
        }
        else
        {
            // If starting fresh, reset the count of completed maps
            counterMaps = 0;
        }

        // Loop while the number of completed maps is less than allowed number of games to show/play
        while (counterMaps < showMapsLot)
        {
            // Load the IDs of games already played into a set
            std::set<int> gamesAlreadyPlayed;
            ReadFileGamesId(filename, gamesAlreadyPlayed);

            // Filter to get games that have not been played yet
            MapId gamesNotPlayed[totalGames];
            int gamesNotPlayedCount = 0;
            ReduceGamesPlayed(allGames, totalGames, gamesAlreadyPlayed, gamesNotPlayed, gamesNotPlayedCount);

            if (gamesNotPlayedCount == 0)
            {
                utils.ClearScreenComplety();
                std::wcout << L"No games left available.\n";
                Sleep(1000);
                return false; // No more games left to play
            }

            // Clear previous options and generate new game options
            for (int i = 0; i < 3; ++i)
                opcionesGames[i] = MapId::None;

            int optionCount = std::min(3, gamesNotPlayedCount);
            GenerateRandomMapId(gamesNotPlayed, gamesNotPlayedCount, opcionesGames, optionCount);

            // Run the main room selection with generated options and get user selection
            selected = mainRoom.Run(consoleSettings, opcionesGames);

            bool validSelection = false;
            for (int i = 0; i < optionCount; ++i)
                if (selected == opcionesGames[i])
                    validSelection = true;

            // If selected game is valid and map/game started successfully
            if (validSelection && ChangeMapAndCheck(selected))
            {
                // Log completed game to file
                gamesCompleted << static_cast<int>(selected) << std::endl;

                // Increase player’s room progress counter
                player.SetRoom(player.GetRoom() + 1);
                counterMaps++;

                if (counterMaps >= showMapsLot)
                    break; // Reached the maximum number of games to show/play
            }
            else if (player.GetLives() == 0)
            {
                // If player ran out of lives, show Game Over screen and ask for restart
                bool restart = gameOver.Show(utils);
                if (restart)
                {
                    // Reset player state and restart the games execution flow
                    player.ResetState(SetDifficultyDetails());
                    return GamesExecute(); // Or restart Global::StartGame()
                }
                else
                {
                    exit(0); // Exit the program/game
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

        // Open output file stream in append mode to record completed games.
        // Appending ensures previous entries are kept and new entries are added.
        std::wofstream gamesCompleted(filenameBoss, std::ios::app);

        // Open input file stream to read previously completed games.
        // Note: std::ios::app is unusual for input; normally std::ios::in is used.
        std::wifstream gamesLecture(filenameBoss, std::ios::app);

        // If the game was loaded from a saved state (manual load)
        if (processThread == STATE_SECOND_MENU_DONE)
        {
            counterBoss = 0; // Reset map counter

            int value;
            // Read each saved game ID from the file to count completed games
            while (gamesLecture >> value)
            {
                counterBoss++; // Increment counter for each completed game found
            }
        }
        else
        {
            // If not loading a saved game, start fresh with zero completed games
            counterBoss = 0;
        }

        // Loop while the number of completed maps is less than allowed number of bosses
        while (counterBoss < showBoossLot)
        {
            std::set<int> gamesAlreadyPlayed;
            ReadFileGamesId(filename, gamesAlreadyPlayed); // Reads completed game IDs into the set

            MapId gamesNotPlayed[totalGames];
            int gamesNotPlayedCount = 0;
            ReduceGamesPlayed(allGames, totalGames, gamesAlreadyPlayed, gamesNotPlayed, gamesNotPlayedCount);

            if (gamesNotPlayedCount == 0)
            {
                utils.ClearScreenComplety();
                std::wcout << L"No bosses left to play.\n";
                Sleep(1000);
                return false; // No more bosses available
            }

            // Clear previous options and generate new boss options
            for (int i = 0; i < 3; ++i)
                opcionesGames[i] = MapId::None;

            int optionCount = std::min(3, gamesNotPlayedCount);
            GenerateRandomMapId(gamesNotPlayed, gamesNotPlayedCount, opcionesGames, optionCount);

            // Run the boss selection screen and get selected boss
            selected = bossSalaPrev.Run(consoleSettings, opcionesGames);

            bool validSelection = false;
            for (int i = 0; i < optionCount; ++i)
                if (selected == opcionesGames[i])
                    validSelection = true;

            // If selection is valid and map change succeeds
            if (validSelection && ChangeMapAndCheck(selected))
            {
                gamesCompleted << static_cast<int>(selected) << std::endl; // Record completed boss
                player.SetRoom(player.GetRoom() + 1);                      // Increment player's current room count
                counterBoss++;

                if (counterBoss >= showBoossLot)
                    break; // Reached maximum allowed bosses
            }
            else if (player.GetLives() == 0)
            {
                // Show Game Over screen and ask for restart
                bool restart = gameOver.Show(utils);
                if (restart)
                {
                    // Reset player state and restart the games execution flow
                    player.ResetState(SetDifficultyDetails());
                    return GamesExecute(); // Or restart Global::StartGame()
                }
                else
                {
                    exit(0); // Exit the game entirely
                }
            }
            Sleep(50);
        }

        return true;
    }

    Player::Difficulty SetDifficultyDetails()
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
    // Reads integers (game IDs) from a file and inserts them into a set
    void ReadFileGamesId(const std::string &filename, std::set<int> &idSet)
    {
        std::wifstream file(filename);
        int id;
        while (file >> id)
            idSet.insert(id);
    }

    // From allGames array, filters out games already played (in 'played' set)
    // and appends the unplayed games to the 'notPlayed'
    void ReduceGamesPlayed(MapId all[], int total, const std::set<int> &played, MapId notPlayed[], int &notPlayedCount)
    {
        notPlayedCount = 0;
        for (int i = 0; i < total; ++i)
        {
            if (played.find(static_cast<int>(all[i])) == played.end())
            {
                notPlayed[notPlayedCount++] = all[i];
            }
        }
    }

    // Changes current map/game based on the MapId enum value
    // Calls the Run() method of the corresponding game/map class
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

        case ChestGame:
            return chestGame.Run();

        case WormGame:
            return worm.Run(consoleSettings);

        case ElevatorGame:
            return elevator.Run(consoleSettings);
        case DodgeGame:
            return sphinx.Run(consoleSettings);

        case SphinxGameM:
            return sphinx.Run(consoleSettings);

        case BoosMario:
            return bossFightMario.Run();

        case BoosZelda:
            return bossFightZelda.Run();

        case MainRoom:
            break;

        default:
            std::wcout << L"Unknown map: " << map << opcionesGames[0] << std::endl;
            return false;
        }
    }

    // Calls ChangeMap and checks player's lives after playing
    // Returns false if player has no lives left, otherwise returns the result of ChangeMap
    bool ChangeMapAndCheck(MapId map)
    {
        bool result = ChangeMap(map);
        if (player.GetLives() == 0)
        {
            return false;
        }
        return result;
    }

    // Randomly selects 'count' MapIds from 'source' array and writes them into 'dest' array
    void GenerateRandomMapId(MapId *source, int sourceSize, MapId *dest, int count)
    {
        // Initialize destination with default value None
        for (int i = 0; i < count; ++i)
            dest[i] = MapId::None;

        // Copy source elements into a temporary array for shuffling
        MapId *pool = new MapId[sourceSize];
        for (int i = 0; i < sourceSize; ++i)
            pool[i] = source[i];

        // Fisher-Yates shuffle on the pool array
        for (int i = sourceSize - 1; i > 0; --i)
        {
            int j = rand() % (i + 1);
            std::swap(pool[i], pool[j]);
        }

        // Assign first 'count' elements from shuffled pool to dest
        for (int i = 0; i < count && i < sourceSize; ++i)
        {
            dest[i] = pool[i];
        }

        delete[] pool;
    }
};

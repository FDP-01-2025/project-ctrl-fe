#ifndef PLAYER_H
#define PLAYER_H
// We use pragma once to prevent multiple inclusions of this header file
#pragma once
// Include necessary libraries
#include <string>
#include <fstream>
#include <utility>

// Include headers for used classes
#include "utils/functions/utils.h"

// This class represents the player in the Bomberman-style game.
// It handles position, lives, bombs, difficulty and also manages save/load of player state.
class Player
{
public:
    // Difficulty levels available in the game
    enum Difficulty
    {
        EASY,
        NORMAL,
        HARD
    };

    Player() {}

    // Get the player's X position from saved file
    int GetX()
    {
        LoadState();
        return x;
    }

    // Get the player's Y position
    int GetY()
    {
        LoadState();
        return y;
    }

    // Get how many lives the player currently has
    int GetLives()
    {
        LoadState();
        return lives;
    }

    // Get how many bombs the player can currently place
    int GetBombs()
    {
        LoadState();
        return bombsAvailable;
    }

    // Get the maximum bombs the player is allowed to have
    int GetMaxBombs() { return maxBombs; }

    // Get the current game difficulty
    Difficulty GetDifficulty()
    {
        LoadState();
        return difficulty;
    }

    // Get the current event or state of the player
    int GetRoom()
    {
        LoadState();
        return room;
    }

    // Get the current event or state of the player
    int GetEvent()
    {
        LoadState();
        return event;
    }
    // Obtener cuántos cofres ha abierto el jugador
    int GetOpenedChests()
    {
        LoadState();
        return openedChests;
    }

    // Incrementar la cantidad de cofres abiertos en 1
    void IncrementOpenedChests()
    {
        openedChests++;
        CleanEmptyLines();
        saveOpenedChests();
    }

    // Check whether control B (bomb placement) is active
    bool IsControlBActive()
    {
        LoadState();
        return controlB;
    }

    // Enables or disables the control for placing bombs
    // Only updates the ControlB value in the file
    void ActivateControlB(bool state)
    {
        controlB = state;
        CleanEmptyLines();
        saveControlB(); // save just this specific field
    }

    // Updates the player's position and saves only that data
    void SetPosition(int newX, int newY)
    {
        x = newX;
        y = newY;
        CleanEmptyLines();
        savePosition(); // save only X and Y values
    }

    // Set the number of lives and save them
    void SetLives(int newLives)
    {
        lives = newLives;
        CleanEmptyLines();
        saveLives(); // save only lives field
    }

    // Set the player's room or level and save it
    void SetRoom(int newRoom)
    {
        room = newRoom;
        CleanEmptyLines();
        saveRoom();
    }

    // Set the current event or state of the player and save it
    void SetEvent(int newEvent)
    {
        event = newEvent;
        CleanEmptyLines();
        saveEvent();
    }

    // Set how many bombs the player has (up to a max) and save them
    void SetBombs(int newBombs)
    {
        bombsAvailable = (newBombs > maxBombs) ? maxBombs : newBombs;
        CleanEmptyLines();
        saveBombs(); // save only bombs field
    }

    // Set the difficulty level and update the file with only that field
    void SetDifficulty(Difficulty dif)
    {
        difficulty = dif;
        CleanEmptyLines();
        saveDifficulty();
    }

    // Moves the player on the grid if the next tile is valid
    // Valid tiles are: space (' '), path ('/'), or bonus bomb ('B')
    void Move(int dx, int dy, char nextTile)
    {
        x += dx;
        y += dy;
        CleanEmptyLines();
        savePosition(); // full state save after movement
    }
    bool IsOpenChestKey(wchar_t input)
    {
        return input == 'q' || input == 'Q';
    }

    // Check if the player can place at least one bomb
    bool CanPlaceBomb() { return GetBombs() > 0; }

    // Places a bomb: reduces bomb count by 1 and saves the full state
    void PlaceBomb()
    {
        if (bombsAvailable > 0)
        {
            bombsAvailable--;
            CleanEmptyLines();
            saveBombs();
        }
    }

    // Subtract one life and save state
    void LoseLife()
    {
        lives--;
        CleanEmptyLines();
        saveLives();
    }

    // Add a bomb (if under limit) and save
    void IncrementBombs()
    {
        if (bombsAvailable < maxBombs)
        {
            bombsAvailable++;
            CleanEmptyLines();
            saveBombs();
        }
    }

    // Add one life and save
    void IncrementLife()
    {
        lives++;
        CleanEmptyLines();
        saveLives();
    }

    // Increment lives by a specific count and save
    void IncrementLives(int count)
    {
        lives += count;
        CleanEmptyLines();
        saveLives(); // save only lives field
    }

    // Decrement lives by a specific count and save
    void DecrementLives(int count)
    {
        lives -= count;
        if (lives < 0)
            lives = 1; // Prevent negative lives
        CleanEmptyLines();
        saveLives(); // save only lives field
    }

    // Increment bombs by a specific count and save
    void IncrementBombs(int count)
    {
        bombsAvailable += count;
        if (bombsAvailable > maxBombs)
            bombsAvailable = maxBombs; // Prevent exceeding max bombs
        CleanEmptyLines();
        saveBombs(); // save only bombs field
    }

    // Decrement bombs by a specific count and save
    void DecrementBombs(int count)
    {
        bombsAvailable -= count;
        if (bombsAvailable < 0)
            bombsAvailable = 0; // Prevent negative bombs
        CleanEmptyLines();
        saveBombs(); // save only bombs field
    }

    // Convert keyboard input into movement direction
    // Returns a pair (dx, dy)
    std::pair<int, int> GetInputDirection(wchar_t input)
    {
        int dx = 0, dy = 0;
        if (input == 'w')
            dy = -1;
        else if (input == 's')
            dy = 1;
        else if (input == 'a')
            dx = -1;
        else if (input == 'd')
            dx = 1;
        return {dx, dy};
    }
    // Reset the player's full state depending on difficulty
    // Called at game start or when restarting from menu
    void ResetState(Difficulty dif)
    {
        removeStatusFile(); // delete any old save
        openedChests = 0;
        difficulty = dif;
        x = 1;
        y = 1;
        controlB = false;
        room = 0; // Reset room or level

        // Setup based on difficulty
        switch (difficulty)
        {
        case EASY:
            lives = 5;
            bombsAvailable = 10;
            room = 1; // Start at room 1
            break;
        case NORMAL:
            lives = 3;
            bombsAvailable = 5;
            room = 1; // Start at room 1
            break;
        case HARD:
            lives = 1;
            bombsAvailable = 3;
            room = 1; // Start at room 1
            break;
        }

        CleanEmptyLines();
        saveState(); // Save entire state
    }

    // Deletes the save file from disk
    void removeStatusFile()
    {
        std::remove(filename.c_str());
    }

    // Force load player state manually (e.g. from outside class)
    void LoadStateManual()
    {
        LoadState();
    }

private:
    // Player data
    int x = 0, y = 0;
    int lives = 3;
    int room = 0;  // Current room or level
    int event = 0; // Current event or state
    int bombsAvailable = 1;
    int maxBombs = 25;
    Difficulty difficulty = EASY;
    bool controlB = false;
    Utils utils;
    int openedChests = 0; // Contador de cofres abiertos

    // Save file path: something like assets/data/playerState.txt
    const std::string filename = utils.GetAssetsPath() + "data\\playerState.txt";

    // Save all player data to file (full state)
    // This function writes the entire player state to the save file,
    // overwriting any previous content.
    void saveState()
    {
        std::ofstream out(filename, std::ios::trunc);
        if (out.is_open())
        {
            // Write the player's X coordinate
            out << "X: " << x << "\n";
            // Write the player's Y coordinate
            out << "Y: " << y << "\n";
            // Write the player's remaining lives
            out << "Lives: " << lives << "\n";
            // Write the number of bombs the player currently has available
            out << "Bombs: " << bombsAvailable << "\n";
            // Write the difficulty level as an integer
            out << "Difficulty: " << (int)difficulty << "\n";
            // Write whether the controlB is active (1) or not (0)
            out << "ControlB: " << (controlB ? 1 : 0) << "\n";
            // Write the current room or level
            out << "Room: " << room << "\n";
            // Write the current event or state
            out << "Event: " << event << "\n";
            out << "OpenedChests: " << openedChests << "\n";

            // Close the file after writing
            out.close();
        }
    }

    // Load the player state from file
    // This function reads the saved player state from the file,
    // restoring all fields by reading line by line.
    void LoadState()
    {
        std::ifstream in(filename, std::ios::binary);
        if (in.is_open())
        {
            std::string label;
            int diff = 0, ctrlB = 0;
            // Attempt to read each labeled field from the file.
            // The order must match exactly how saveState writes the data.
            if (in >> label >> x && in >> label >> y && in >> label >> lives && in >> label >> bombsAvailable && in >> label >> diff && in >> label >> ctrlB && in >> label >> room && in >> label >> event && in >> label >> openedChests)
            {
                // Convert integer read back to enum type for difficulty
                difficulty = (Difficulty)diff;
                // Convert integer read back to bool for controlB
                controlB = ctrlB != 0;
            }
            in.close();
        }
    }

    // Save only the ControlB line (without affecting other data)
    // This function updates only the ControlB line in the save file
    // while preserving all other saved data intact.
    // Since direct editing of files in place is not trivial,
    // it copies all lines to a temporary file, replacing only ControlB,
    // then replaces the original file with the temporary one.
    void saveControlB()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("ControlB:", 0) == 0)
                out << "ControlB: " << (controlB ? 1 : 0);
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save only X and Y position (used after movement)
    // Similar logic to saveControlB, but updates only the player's X and Y coordinates.
    void savePosition()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("X:", 0) == 0)
                out << "X: " << x;
            else if (line.rfind("Y:", 0) == 0)
                out << "Y: " << y;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save only bombsAvailable field
    // Updates the bombsAvailable count in the save file,
    // preserving all other data by rewriting to a temp file.
    void saveBombs()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("Bombs:", 0) == 0)
                out << "Bombs: " << bombsAvailable;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save only the number of lives
    // Updates player's lives count while preserving the rest of the save file.
    void saveLives()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("Lives:", 0) == 0)
                out << "Lives: " << lives;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    void saveOpenedChests()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("OpenedChests:", 0) == 0)
                out << "OpenedChests: " << openedChests;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save only the difficulty field
    // Updates difficulty level in the save file,
    // preserving all other data by rewriting the file.
    void saveDifficulty()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("Difficulty:", 0) == 0)
                out << "Difficulty: " << (int)difficulty;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save only the room or level number
    void saveRoom()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("Room:", 0) == 0)
                out << "Room: " << room;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Save event that the player is currently in
    void saveEvent()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt", std::ios::trunc);
        std::string line;
        bool firstLine = true;

        while (std::getline(in, line))
        {
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty())
                continue;

            if (!firstLine)
                out << "\n";
            if (line.rfind("Event:", 0) == 0)
                out << "Event: " << event;
            else
                out << line;

            firstLine = false;
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    // Cleans up empty lines in the save file
    // This function reads the save file and removes any empty lines,
    // rewriting the file without them to keep it clean.
    void CleanEmptyLines()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp_clean.txt", std::ios::trunc);

        std::string line;
        bool firstLine = true;
        while (std::getline(in, line))
        {
            // Trim whitespace
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (!line.empty())
            {
                if (!firstLine)
                    out << "\n"; // Only add newline before this if it's not the first line
                out << line;
                firstLine = false;
            }
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp_clean.txt", filename.c_str());
    }
};

#endif

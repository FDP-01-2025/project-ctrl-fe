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
        loadState();
        return x;
    }

    // Get the player's Y position
    int GetY()
    {
        loadState();
        return y;
    }

    // Get how many lives the player currently has
    int GetLives()
    {
        loadState();
        return lives;
    }

    // Get how many bombs the player can currently place
    int GetBombs()
    {
        loadState();
        return bombsAvailable;
    }

    // Get the maximum bombs the player is allowed to have
    int GetMaxBombs() { return maxBombs; }

    // Get the current game difficulty
    Difficulty GetDifficulty()
    {
        loadState();
        return difficulty;
    }

    // Check whether control B (bomb placement) is active
    bool IsControlBActive()
    {
        loadState();
        return controlB;
    }

    // Enables or disables the control for placing bombs
    // Only updates the ControlB value in the file
    void ActivateControlB(bool state)
    {
        controlB = state;
        saveControlB(); // save just this specific field
    }

    // Updates the player's position and saves only that data
    void SetPosition(int newX, int newY)
    {
        x = newX;
        y = newY;
        savePosition(); // save only X and Y values
    }

    // Set the number of lives and save them
    void SetLives(int newLives)
    {
        lives = newLives;
        saveLives(); // save only lives field
    }

    // Set how many bombs the player has (up to a max) and save them
    void SetBombs(int newBombs)
    {
        bombsAvailable = (newBombs > maxBombs) ? maxBombs : newBombs;
        saveBombs(); // save only bombs field
    }

    // Set the difficulty level and update the file with only that field
    void SetDifficulty(Difficulty dif)
    {
        difficulty = dif;
        saveDifficulty();
    }

    // Moves the player on the grid if the next tile is valid
    // Valid tiles are: space (' '), path ('/'), or bonus bomb ('B')
    void Move(int dx, int dy, char nextTile)
    {
        if (nextTile == ' ' || nextTile == '/' || nextTile == 'B')
        {
            x += dx;
            y += dy;
            saveState(); // full state save after movement
        }
    }

    // Check if the player can place at least one bomb
    bool CanPlaceBomb() { return GetBombs() > 0; }

    // Places a bomb: reduces bomb count by 1 and saves the full state
    void PlaceBomb()
    {
        if (bombsAvailable > 0)
        {
            bombsAvailable--;
            saveState();
        }
    }

    // Subtract one life and save state
    void LoseLife()
    {
        lives--;
        saveState();
    }

    // Add a bomb (if under limit) and save
    void IncrementBombs()
    {
        if (bombsAvailable < maxBombs)
        {
            bombsAvailable++;
            saveState();
        }
    }

    // Add one life and save
    void IncrementLife()
    {
        lives++;
        saveState();
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
        difficulty = dif;
        x = 1;
        y = 1;
        controlB = false;

        // Setup based on difficulty
        switch (difficulty)
        {
        case EASY:
            lives = 5;
            bombsAvailable = 10;
            break;
        case NORMAL:
            lives = 3;
            bombsAvailable = 5;
            break;
        case HARD:
            lives = 1;
            bombsAvailable = 3;
            break;
        }

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
        loadState();
    }

private:
    // Player data
    int x = 0, y = 0;
    int lives = 3;
    int bombsAvailable = 1;
    int maxBombs = 25;
    Difficulty difficulty = EASY;
    bool controlB = false;
    Utils utils;

    // Save file path: something like assets/data/playerState.txt
    const std::string filename = utils.GetAssetsPath() + "data\\playerState.txt";

    // Save all player data to file (full state)
    // This function writes the entire player state to the save file,
    // overwriting any previous content.
    void saveState()
    {
        std::ofstream out(filename);
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
            out.close();
        }
    }

    // Load the player state from file
    // This function reads the saved player state from the file,
    // restoring all fields by reading line by line.
    void loadState()
    {
        std::ifstream in(filename, std::ios::binary);
        if (in.is_open())
        {
            std::string label;
            int diff = 0, ctrlB = 0;
            // Attempt to read each labeled field from the file.
            // The order must match exactly how saveState writes the data.
            if (in >> label >> x &&
                in >> label >> y &&
                in >> label >> lives &&
                in >> label >> bombsAvailable &&
                in >> label >> diff &&
                in >> label >> ctrlB)
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
        std::ofstream out("temp.txt");
        std::string line;

        while (std::getline(in, line))
        {
            // Check if the line starts with "ControlB:"
            if (line.rfind("ControlB:", 0) == 0)
                out << "ControlB: " << (controlB ? 1 : 0) << "\n"; // Write updated value
            else
                out << line << "\n"; // Copy line unchanged
        }

        in.close();
        out.close();
        // Delete the original file
        std::remove(filename.c_str());
        // Rename the temporary file to original filename
        std::rename("temp.txt", filename.c_str());
    }

    // Save only X and Y position (used after movement)
    // Similar logic to saveControlB, but updates only the player's X and Y coordinates.
    void savePosition()
    {
        std::ifstream in(filename, std::ios::binary);
        std::ofstream out("temp.txt");
        std::string line;
        while (std::getline(in, line))
        {
            if (line.rfind("X:", 0) == 0)
                out << "X: " << x << "\n"; // Update X position
            else if (line.rfind("Y:", 0) == 0)
                out << "Y: " << y << "\n"; // Update Y position
            else
                out << line << "\n"; // Copy other lines unchanged
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
        std::ofstream out("temp.txt");
        std::string line;
        while (std::getline(in, line))
        {
            if (line.rfind("Bombs:", 0) == 0)
                out << "Bombs: " << bombsAvailable << "\n"; // Update bombs count
            else
                out << line << "\n"; // Copy other lines unchanged
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
        std::ofstream out("temp.txt");
        std::string line;
        while (std::getline(in, line))
        {
            if (line.rfind("Lives:", 0) == 0)
                out << "Lives: " << lives << "\n"; // Update lives count
            else
                out << line << "\n"; // Copy other lines unchanged
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
        std::ofstream out("temp.txt");
        std::string line;
        while (std::getline(in, line))
        {
            if (line.rfind("Difficulty:", 0) == 0)
                out << "Difficulty: " << (int)difficulty << "\n"; // Update difficulty
            else
                out << line << "\n"; // Copy other lines unchanged
        }

        in.close();
        out.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }
};

#endif

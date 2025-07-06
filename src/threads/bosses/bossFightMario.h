#ifndef MAIN_BOSS_FIGHT_MARIO_H
#define MAIN_BOSS_FIGHT_MARIO_H

// Include necessary headers for input/output, delay, cursor handling, and game modules
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <thread>

// Include utility and game component headers
#include "utils/functions/utils.h"
#include "utils/screen/colors.h"
#include "core/modules/boss-fight-mario/miniBoss.h"
#include "core/modules/boss-fight-mario/bullet.h"
#include "core/modules/boss-fight-mario/playerMario.h"
#include "core/modules/boss-fight-mario/hudFightMario.h"

// Main class that controls the Mario-style boss fight
class MainBossFight
{
private:
    Utils utils;                      // Utility class for cursor and screen handling
    PlayerMario player;              // The Mario-style player character
    HUDFightMario hudfFight;         // HUD display for the fight
    MiniBoss boss;                   // The mini boss enemy
    Bullet bullet;                   // Bullet projectile shot by the player
    bool isRunning = true;           // Flag to control the game loop
    int offsetX = 0, offsetY = 0;    // Offset to center the arena
    bool showDoor = false;           // Whether the exit door is visible
    int doorX = 38, doorY = 13;      // Coordinates of the door once the boss is defeated

public:
    // Main loop of the boss fight
    bool Run()
    {
        utils.SetUtf8();                      // Set console encoding to UTF-8
        utils.ClearScreenComplety();          // Clear screen before starting
        CalculateOffsets();                   // Calculate where to draw things centered

        boss.Init(player.playerBase.GetDifficulty()); // Set boss stats based on difficulty

        // Main game loop
        while (isRunning)
        {
            utils.ClearScreen();             // Clear screen every frame

            DrawArena();                     // Draw arena borders and ground
            boss.Move(2, 38);                // Move boss between boundaries

            boss.Draw(utils, offsetX, offsetY); // Draw the boss
            bullet.Move(utils, offsetX, offsetY); // Move bullet if active
            bullet.Draw(utils, offsetX, offsetY); // Draw bullet if active
            hudfFight.SetCenteredOffset(offsetX); // Center HUD next to arena

            // Draw regular or exit HUD
            if (!showDoor)
                hudfFight.Draw(player.playerBase, 40);
            else
                hudfFight.DrawExit(player.playerBase, 40);

            // Handle input for movement, jumping, and shooting
            if (GetAsyncKeyState('A') & 0x8000)
                player.MoveLeft();
            if (GetAsyncKeyState('D') & 0x8000)
                player.MoveRight();
            if (GetAsyncKeyState('W') & 0x8000)
                player.Jump();
            if (GetAsyncKeyState('J') & 0x8000)
                bullet.Shoot(player.x, player.y, -1); // Shoot left
            if (GetAsyncKeyState('K') & 0x8000)
                bullet.Shoot(player.x, player.y, 1);  // Shoot right

            // Update player state (jumping, gravity, cooldowns)
            player.UpdateJump();
            player.Fall();
            player.UpdateImmunity();
            player.UpdateMovementCooldown();

            CheckCollision();                // Check if bullet or player touched the boss
            DrawHUD();                       // Draw boss HP bar
            player.Draw(utils, offsetX, offsetY); // Draw the player

            // Check for player death
            if (!player.IsAlive())
            {
                utils.MoveCursor(offsetX + 30, offsetY + 10);
                isRunning = false;
                return false; // End game if player is defeated
            }

            // If boss is defeated and door hasn't been shown, activate it
            if (!boss.isAlive && !showDoor)
            {
                showDoor = true;
            }

            // Show door and check if player reaches it
            if (showDoor)
            {
                utils.MoveCursor(offsetX + doorX, offsetY + doorY);
                std::wcout << L"/"; // Door symbol

                if (player.x == doorX && player.y == doorY)
                {
                    utils.MoveCursor(offsetX + 25, offsetY + 10);
                    isRunning = false;
                    return true; // Player reached the door, end fight
                }
            }

            utils.Sleep(100); // Small delay to control game speed
        }

        utils.MoveCursor(0, 25); // Move cursor below arena after ending
    }

    // Calculates where to position the map and HUD so they are centered
    void CalculateOffsets()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int totalWidth = 40 + 3 + hudfFight.GetWidth(); // Arena width + gap + HUD width
        offsetX = (screenWidth - totalWidth) / 2;
        offsetY = (screenHeight - 15) / 2; // 15 rows of arena height
    }

    // Draws the arena floor and borders
    void DrawArena()
    {
        // Floor line
        for (int i = 0; i < 40; i++)
        {
            utils.MoveCursor(offsetX + i, offsetY + 14);
            std::wcout << L"=";
        }

        // Left and right walls
        for (int i = 0; i <= 14; i++)
        {
            utils.MoveCursor(offsetX + 0, offsetY + i);
            std::wcout << L"#";
            utils.MoveCursor(offsetX + 39, offsetY + i);
            std::wcout << L"#";
        }
    }

    // Draws the boss’s health bar in the HUD
    void DrawHUD()
    {
        int hudX = offsetX + 2;
        int hudY = offsetY + 0;
        
        // Clear the area where HUD is drawn
        utils.MoveCursor(hudX, hudY);
        std::wcout << L"                                  ";

        // Print label
        utils.MoveCursor(hudX, hudY);
        std::wcout << CYAN << L"Boss: " << RESET;

        // Calculate fill ratio and percentage
        const int barWidth = 10;
        float ratio = (float)boss.health / boss.maxHealth;
        int filled = (int)(ratio * barWidth);
        int percent = (int)(ratio * 100);

        // Choose color based on health ratio
        std::wstring color;
        if (ratio > 0.6f)
            color = GREEN_BRIGHT;
        else if (ratio > 0.3f)
            color = YELLOW_BRIGHT;
        else
            color = RED_BRIGHT;

        // Draw visual health bar
        std::wcout << L"[";
        std::wcout << color;
        for (int i = 0; i < filled; ++i)
            std::wcout << L"█";
        for (int i = filled; i < barWidth; ++i)
            std::wcout << L" ";
        std::wcout << RESET << L"] ";

        // Display percent
        std::wcout << percent << L"%";
    }

    // Checks for collisions between the player, boss, and bullets
    void CheckCollision()
    {
        // Skip if boss is already dead
        if (!boss.isAlive)
            return;

        // Bullet hits boss
        if (bullet.isActive && bullet.x == boss.x && bullet.y == boss.y)
        {
            boss.TakeHit();
            bullet.Deactivate();
            return;
        }

        // Check if player is touching the boss (direct or adjacent)
        bool isTouching = false;
        if (player.x == boss.x && player.y == boss.y)
            isTouching = true;
        else if ((player.x == boss.x - 1 || player.x == boss.x + 1) && player.y == boss.y)
            isTouching = true;

        // If touching, player takes damage
        if (isTouching)
            player.LoseLife();
    }
};

#endif
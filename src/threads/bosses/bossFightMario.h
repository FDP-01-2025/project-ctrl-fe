#ifndef MAIN_BOSS_FIGHT_MARIO_H
#define MAIN_BOSS_FIGHT_MARIO_H

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <iomanip>
#include <thread>
#include "utils/functions/utils.h"
#include "utils/screen/colors.h"
#include "core/modules/boss-fight-mario/miniBoss.h"
#include "core/modules/boss-fight-mario/bullet.h"
#include "core/modules/boss-fight-mario/playerMario.h"
#include "core/modules/boss-fight-mario/hudFightMario.h"

class MainBossFight
{
private:
    Utils utils;
    PlayerMario player;
    HUDFightMario hudfFight;
    MiniBoss boss;
    Bullet bullet;
    bool isRunning = true;
    int offsetX = 0, offsetY = 0;
    bool showDoor = false;
    int doorX = 38, doorY = 13; // Posición de la puerta dentro del mapa (ajústala si gustas)

public:
    void Run()
    {
        utils.SetUtf8();
        utils.ClearScreenComplety();
        CalculateOffsets();
        boss.Init(player.playerBase.GetDifficulty());

        while (isRunning)
        {
            utils.ClearScreen();

            DrawArena();
            boss.Move(2, 38);

            boss.Draw(utils, offsetX, offsetY);
            bullet.Move(utils, offsetX, offsetY);
            bullet.Draw(utils, offsetX, offsetY);
            hudfFight.SetCenteredOffset(offsetX);

            if (!showDoor)
            {
                hudfFight.Draw(player.playerBase, 40);
            }

            if (showDoor)
            {
                hudfFight.DrawExit(player.playerBase, 40);
            }

            if (GetAsyncKeyState('A') & 0x8000)
                player.MoveLeft();
            if (GetAsyncKeyState('D') & 0x8000)
                player.MoveRight();
            if (GetAsyncKeyState('W') & 0x8000)
                player.Jump();
            if (GetAsyncKeyState('J') & 0x8000)
                bullet.Shoot(player.x, player.y, -1);
            if (GetAsyncKeyState('K') & 0x8000)
                bullet.Shoot(player.x, player.y, 1);

            player.UpdateJump();
            player.Fall();
            player.UpdateImmunity();
            player.UpdateMovementCooldown();

            CheckCollision();
            DrawHUD();
            player.Draw(utils, offsetX, offsetY);

            if (!player.IsAlive())
            {
                utils.MoveCursor(offsetX + 30, offsetY + 10);
                std::wcout << RED << L"Game Over! The boss defeated you." << RESET;
                isRunning = false;
            }

            if (!boss.isAlive && !showDoor)
            {
                showDoor = true; // Activar la puerta
            }

            if (showDoor)
            {
                utils.MoveCursor(offsetX + doorX, offsetY + doorY);
                std::wcout << L"/"; // Representación de la puerta

                // Si el jugador está tocando la puerta
                if (player.x == doorX && player.y == doorY)
                {
                    utils.MoveCursor(offsetX + 25, offsetY + 10);
                    std::wcout << GREEN_BRIGHT << L"You entered the next room!" << RESET;
                    isRunning = false;
                }
            }

            utils.Sleep(100);
        }

        utils.MoveCursor(0, 25);
    }

    void CalculateOffsets()
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        int totalWidth = 40 + 3 + hudfFight.GetWidth(); // mapa + espacio + HUD
        offsetX = (screenWidth - totalWidth) / 2;
        offsetY = (screenHeight - 15) / 2;
    }

    void DrawArena()
    {
        for (int i = 0; i < 40; i++)
        {
            utils.MoveCursor(offsetX + i, offsetY + 14); // piso
            std::wcout << L"=";
        }

        for (int i = 0; i <= 14; i++)
        {
            utils.MoveCursor(offsetX + 0, offsetY + i);
            std::wcout << L"#";
            utils.MoveCursor(offsetX + 39, offsetY + i);
            std::wcout << L"#";
        }
    }

    void DrawHUD()
    {
        int hudX = offsetX + 2;
        int hudY = offsetY + 0;
        
        utils.MoveCursor(hudX, hudY);
        std::wcout << L"                                  "; // Limpiar pantalla antes de dibujar el HUD

        // Etiqueta
        utils.MoveCursor(hudX, hudY);
        std::wcout << CYAN << L"Boss: " << RESET;

        // Cálculo
        const int barWidth = 10;
        float ratio = (float)boss.health / boss.maxHealth;
        int filled = (int)(ratio * barWidth);
        int percent = (int)(ratio * 100);

        // Color
        std::wstring color;
        if (ratio > 0.6f)
            color = GREEN_BRIGHT;
        else if (ratio > 0.3f)
            color = YELLOW_BRIGHT;
        else
            color = RED_BRIGHT;

        // Barra visual
        std::wcout << L"[";
        std::wcout << color;
        for (int i = 0; i < filled; ++i)
            std::wcout << L"█";
        for (int i = filled; i < barWidth; ++i)
            std::wcout << L" ";
        std::wcout << RESET << L"] ";

        // Porcentaje// Borra 4 espacios antes (3 para número + 1 para %)
        std::wcout << percent << L"%";
    }

    void CheckCollision()
    {
        if (!boss.isAlive)
            return;

        if (bullet.isActive && bullet.x == boss.x && bullet.y == boss.y)
        {
            boss.TakeHit();
            bullet.Deactivate();
            return;
        }

        bool isTouching = false;
        if (player.x == boss.x && player.y == boss.y)
            isTouching = true;
        else if ((player.x == boss.x - 1 || player.x == boss.x + 1) && player.y == boss.y)
            isTouching = true;

        if (isTouching)
            player.LoseLife();
    }
};

#endif

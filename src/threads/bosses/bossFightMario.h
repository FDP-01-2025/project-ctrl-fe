#ifndef MAIN_BOSS_FIGHT_MARIO_H
#define MAIN_BOSS_FIGHT_MARIO_H

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include "utils/functions/utils.h"
#include "utils/screen/colors.h"
#include "core/modules/boss-fight-mario/miniBoss.h"
#include "core/modules/boss-fight-mario/bullet.h"
#include "core/modules/boss-fight-mario/playerMario.h"

class MainBossFight
{
private:
    Utils utils;
    PlayerMario player;
    MiniBoss boss;
    Bullet bullet;
    bool isRunning = true;

public:
    void Run()
    {
        utils.SetUtf8();
        utils.ClearScreenComplety();

        while (isRunning)
        {
            utils.ClearScreen();

            DrawArena();
            boss.Move(2, 77);
            boss.Draw(utils);
            bullet.Move();
            bullet.Draw(utils);

            // Movimiento más fluido con GetAsyncKeyState (requiere <windows.h>)
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

            if (!player.IsAlive())
            {
                utils.MoveCursor(30, 10);
                std::wcout << RED << L"Game Over! The boss defeated you." << RESET;
                isRunning = false;
            }

            if (!boss.isAlive)
            {
                utils.MoveCursor(25, 10);
                std::wcout << GREEN_BRIGHT << L"Congratulations! You defeated the Mini Boss." << RESET;
                isRunning = false;
            }
            utils.ClearScreen(); 
            player.Draw(utils);

            utils.Sleep(100);
        }

        utils.MoveCursor(0, 25);
    }

    void DrawArena()
    {
        for (int i = 0; i < 80; i++)
        {
            utils.MoveCursor(i, 20);
            std::wcout << L"=";
        }
        for (int i = 0; i <= 20; i++)
        {
            utils.MoveCursor(0, i);
            std::wcout << L"#";
            utils.MoveCursor(79, i);
            std::wcout << L"#";
        }
        for (int i = 0; i < 80; i++)
        {
            utils.MoveCursor(i, 0);
            std::wcout << L"=";
        }
    }

    void DrawHUD()
    {
        utils.MoveCursor(2, 0);
        std::wcout << YELLOW << L"Lives: " << player.lives << RESET;
        utils.MoveCursor(65, 0);
        std::wcout << CYAN << L"Boss: " << boss.health << RESET;
    }

    void CheckCollision()
    {
        if (!boss.isAlive)
            return;

        // Daño por bala
        if (bullet.isActive && bullet.x == boss.x && bullet.y == boss.y)
        {
            boss.TakeHit();
            bullet.Deactivate();
            return;
        }

        // Colisión cuerpo a cuerpo (jefe toca a Mario)
        bool isTouching = false;

        // Colisión exacta
        if (player.x == boss.x && player.y == boss.y)
        {
            isTouching = true;
        }
        // Colisión por contacto lateral (más ajustado a sprites ASCII de 1 ancho)
        else if ((player.x == boss.x - 1 || player.x == boss.x + 1) && player.y == boss.y)
        {
            isTouching = true;
        }

        if (isTouching)
        {
            player.LoseLife();
        }
    }
};

#endif

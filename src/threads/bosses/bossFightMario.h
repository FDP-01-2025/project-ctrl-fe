#ifndef MAIN_BOSS_FIGHT_MARIO_H
#define MAIN_BOSS_FIGHT_MARIO_H

#include <iostream>
#include <conio.h>   // Para _kbhit y _getch
#include <windows.h> // Para consola
#include <chrono>
#include <thread>
#include "utils/functions/utils.h"
#include "utils/screen/colors.h"

class MiniBoss
{
public:
    int x = 40, y = 20;
    int health = 3;
    bool alive = true;
    bool movingRight = true;

    void Move(int minX, int maxX)
    {
        if (!alive)
            return;
        if (movingRight)
        {
            if (x < maxX)
                x++;
            else
                movingRight = false;
        }
        else
        {
            if (x > minX)
                x--;
            else
                movingRight = true;
        }
    }

    void Draw(Utils &utils)
    {
        if (!alive)
            return;
        utils.MoveCursor(x, y);
        std::wcout << RED << L"M" << RESET; // 'M' en rojo
    }

    void TakeHit()
    {
        health--;
        if (health <= 0)
            alive = false;
    }
};

class PlayerMario
{
public:
    int x = 10;
    int y = 19; // suelo
    bool isJumping = false;
    int jumpCounter = 0;
    int lives = 3;

    void Draw(Utils &utils)
    {
        utils.MoveCursor(x, y);
        std::wcout << PINK << L"இ" << RESET; // Representación estilo Mario
    }

    void MoveLeft()
    {
        if (x > 1)
            x--;
    }

    void MoveRight()
    {
        if (x < 78)
            x++;
    }

    void Jump()
    {
        if (!isJumping && y == 19)
        {
            isJumping = true;
            jumpCounter = 3; // 3 frames de salto hacia arriba
        }
    }

    void UpdateJump()
    {
        if (isJumping)
        {
            if (jumpCounter > 0)
            {
                y--;
                jumpCounter--;
            }
            else
            {
                y++;
                if (y >= 19)
                {
                    y = 19;
                    isJumping = false;
                }
            }
        }
    }

    void Fall()
    {
        if (!isJumping && y < 19)
        {
            y++;
        }
    }

    void LoseLife()
    {
        lives--;
    }

    bool IsAlive()
    {
        return lives > 0;
    }
};

class MainBossFight
{
private:
    Utils utils;
    PlayerMario player;
    MiniBoss boss;
    bool running = true;
    int frame = 0;

public:
    void Run()
    {
        utils.SetUtf8();
        utils.ClearScreenComplety();

        while (running)
        {
            utils.ClearScreen();

            DrawGround();
            boss.Move(10, 70);
            boss.Draw(utils);
            player.Draw(utils);

            CheckCollision();

            if (_kbhit())
            {
                char ch = _getch();
                if (ch == 'a')
                    player.MoveLeft();
                else if (ch == 'd')
                    player.MoveRight();
                else if (ch == 'w')
                    player.Jump();
            }

            player.UpdateJump();
            player.Fall();

            DrawHUD();

            if (!player.IsAlive())
            {
                utils.MoveCursor(30, 10);
                std::wcout << RED << L"Game Over! El jefe te derrotó." << RESET;
                running = false;
            }

            if (!boss.alive)
            {
                utils.MoveCursor(25, 10);
                std::wcout << GREEN_BRIGHT << L"Felicidades! Derrotaste al Mini Jefe." << RESET;
                running = false;
            }

            utils.Sleep(80);
            frame++;
        }

        utils.MoveCursor(0, 25);
    }

    void DrawGround()
    {
        for (int i = 0; i < 80; i++)
        {
            utils.MoveCursor(i, 20);
            std::wcout << L"=";
        }
    }

    void DrawHUD()
    {
        utils.MoveCursor(0, 0);
        std::wcout << YELLOW << L"Vidas: " << player.lives << RESET;
        utils.MoveCursor(70, 0);
        std::wcout << CYAN << L"Jefe: " << boss.health << RESET;
    }

    void CheckCollision()
    {
        if (!boss.alive)
            return;

        if (player.x == boss.x && player.y == boss.y)
        {
            if (player.isJumping)
            {
                boss.TakeHit();
                player.y--; // rebote
            }
            else
            {
                player.LoseLife();
            }
        }
    }
};

#endif

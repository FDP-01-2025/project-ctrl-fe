#pragma once
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/sphinx/hudSphinx.h"

struct GeographyQuestion
{
    std::wstring question;
    std::wstring options[3];
    int correctAnswer;
};

GeographyQuestion GenerateQuestion()
{
    int type = rand() % 8; // tienes 8 preguntas
    GeographyQuestion q;

    switch (type)
    {
    case 0:
        q.question = L"What is the longest river in the world?";
        q.options[0] = L"Amazon";
        q.options[1] = L"Nile";
        q.options[2] = L"Yangtze";
        q.correctAnswer = 1;
        break;
    case 1:
        q.question = L"What is the capital of Australia?";
        q.options[0] = L"Sydney";
        q.options[1] = L"Canberra";
        q.options[2] = L"Melbourne";
        q.correctAnswer = 1;
        break;
    case 2:
        q.question = L"Where are the Andes mountains located?";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    case 3:
        q.question = L"1";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    case 4:
        q.question = L"2";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    case 5:
        q.question = L"3";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    case 6:
        q.question = L"4";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    case 7:
        q.question = L"5";
        q.options[0] = L"Asia";
        q.options[1] = L"Europe";
        q.options[2] = L"South America";
        q.correctAnswer = 2;
        break;
    }

    return q;
}

class SphinxGame
{
public:
    SphinxGame();
    bool Run(Console consoleSettings);

private:
    Map map;
    Utils utils;
    Player player;
    HUDSphinx hud;

    int viewWidth = 36;
    int viewHeight = 18;
    int playerX, playerY;
    bool isRunning;
    int lives;
    int correctAnswers;
    int currentQuestion;
    int totalQuestions;

    int messageX, messageY;
    int offsetX = 2, offsetY = 1;

    GeographyQuestion question;
    int answerStatus; // -1 = sin mensaje, 0 = incorrecto, 1 = correcto
    bool questionAnswered;

    void LoadLevel(std::string key);
    void SetGoodStyle(Console consoleSettings);
    void ProcessInput(char input);
};

SphinxGame::SphinxGame()
    : isRunning(true), correctAnswers(0), currentQuestion(1), answerStatus(-1), questionAnswered(false)
{
    player.SetLives(3);
    lives = player.GetLives();
    totalQuestions = 5; // fijo
    question = GenerateQuestion();
}

bool SphinxGame::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings);

    std::string key = utils.GetAssetsPath() + "maps\\sphinx\\sphinx.txt";
    LoadLevel(key);
    viewWidth = map.GetWidth();

    offsetX = (utils.GetConsoleWidth() - viewWidth) / 2 - 4;
    offsetY = (utils.GetConsoleHeight() - map.GetHeight()) / 2 - 1;
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;

    messageX = offsetX;
    messageY = offsetY + map.GetHeight() + 2;

    hud.SetCenteredOffset(offsetX);
    utils.ClearScreenComplety();

    while (isRunning)
    {
        map.DrawWithPlayerSphinx(
            map.GetWidth(), map.GetHeight(),
            player.GetX(), player.GetY(),
            offsetX, offsetY);

        hud.Draw(player, currentQuestion, viewWidth, answerStatus);
        answerStatus = -1;

        int preguntaY = offsetY + map.GetHeight() + 1;
        int preguntaX = offsetX + (viewWidth - question.question.length()) / 2;
        utils.PrintAtPosition(preguntaX, preguntaY, question.question, ORANGE);

        for (int i = 0; i < 3; ++i)
        {
            std::wstring text = ToWString(i + 1) + L") " + question.options[i];
            int optionX = offsetX + (viewWidth - text.length()) / 2;
            utils.PrintAtPosition(optionX, preguntaY + 2 + i, text, YELLOW_BRIGHT);
        }

        if (_kbhit())
        {
            char input = _getch();
            ProcessInput(input);
        }

        Sleep(20);
    }

    return true;
}

void SphinxGame::ProcessInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first, dy = dir.second;

    if (dx == 0 && dy == 0) return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;
    wchar_t tile = map.GetTile(newX, newY);

    if (tile != L'A' && tile != L'B' && tile != L'#')
    {
        player.SetPosition(newX, newY);

        if (!questionAnswered && (tile == L'1' || tile == L'2' || tile == L'3'))
        {
            int answer = tile - L'1';

            if (answer == question.correctAnswer)
            {
                answerStatus = 1;
                correctAnswers++;
            }
            else
            {
                answerStatus = 0;
                lives--;
                player.SetLives(lives);
            }

            questionAnswered = true;
            currentQuestion++;

            if (correctAnswers >= totalQuestions)
            {
                utils.ClearScreen();
                std::wcout << L"\n\n\nYou passed the Sphinx's challenge!\n";
                isRunning = false;
            }
            else if (lives <= 0)
            {
                utils.ClearScreen();
                std::wcout << L"\n\n\nYou lost. The Sphinx has defeated you.\n";
                isRunning = false;
            }
            else if (currentQuestion <= totalQuestions)
            {
                question = GenerateQuestion();
                questionAnswered = false;
            }
        }
    }
}

void SphinxGame::LoadLevel(std::string key)
{
    utils.ClearScreenComplety();
    map.ReadMap(key, map.GetWidth(), map.GetHeight());
    player.SetPosition(14, 17);
    playerX = player.GetX();
    playerY = player.GetY();
}

void SphinxGame::SetGoodStyle(Console consoleSettings)
{
    consoleSettings.SetConsoleFont(8, 12, L"Lucida Console");
    utils.ClearScreenComplety();
    utils.PrintCentered(L"The sphinx challenges you with geography questions...");
    Sleep(1300);
}

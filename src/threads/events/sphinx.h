#pragma once
#include "utils/player/maping.h"
#include "utils/functions/utils.h"
#include "utils/player/player.h"
#include "../../core/engine/settings/console.h"
#include "core/modules/sphinx/hudSphinx.h"
#include <locale>

struct GeographyQuestion
{
    std::wstring question;
    std::wstring options[3];
    int correctAnswer;
};

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

    int viewWidth = 40;
    int viewHeight = 20;
    int playerX, playerY;
    bool isRunning;
    int lives;
    int correctAnswers;
    int currentQuestion;
    int totalQuestions;

    int messageX, messageY;
    int offsetX = 2, offsetY = 1;

    GeographyQuestion questions[8];
    bool usedQuestions[8];
    GeographyQuestion question;

    int answerStatus; // -1 = no message, 0 = wrong, 1 = correct
    bool questionAnswered;

    void LoadLevel(const std::string &key);
    void SetGoodStyle(Console consoleSettings);
    void ProcessInput(char input);
    void InitializeQuestions();
    GeographyQuestion GenerateQuestion();
};

// Constructor
SphinxGame::SphinxGame()
    : isRunning(true), correctAnswers(0), currentQuestion(1), answerStatus(-1), questionAnswered(false)
{
    lives = player.GetLives();
    totalQuestions = 5;

    InitializeQuestions();
    question = GenerateQuestion();
}

void SphinxGame::InitializeQuestions()
{
    questions[0] = {L"Who was the first President of the United States?", {L"George Washington", L"Thomas Jefferson", L"Abraham Lincoln"}, 0};
    questions[1] = {L"In what year did World War II end?", {L"1943", L"1945", L"1947"}, 1};
    questions[2] = {L"Which civilization built the pyramids of Giza?", {L"Roman", L"Mayan", L"Egyptian"}, 2};
    questions[3] = {L"Who discovered America in 1492?", {L"Ferdinand Magellan", L"Christopher Columbus", L"Vasco da Gama"}, 1};
    questions[4] = {L"Which war was fought between the North and South regions in the United States?", {L"World War I", L"The American Civil War", L"The Revolutionary War"}, 1};
    questions[5] = {L"Who was the famous leader of the Indian independence movement?", {L"Nehru", L"Bhagat Singh", L"Mahatma Gandhi"}, 2};
    questions[6] = {L"What was the name of the ship that brought the Pilgrims to America in 1620?", {L"Mayflower", L"Santa Maria", L"Endeavour"}, 0};
    questions[7] = {L"Who was the dictator of Germany during World War II?", {L"Joseph Stalin", L"Adolf Hitler", L"Benito Mussolini"}, 1};

    for (int i = 0; i < 8; ++i)
        usedQuestions[i] = false;
}

GeographyQuestion SphinxGame::GenerateQuestion()
{
    int available[8];
    int count = 0;

    for (int i = 0; i < 8; ++i)
    {
        if (!usedQuestions[i])
            available[count++] = i;
    }

    if (count == 0)
        return questions[0]; // fallback

    int randomIndex = available[rand() % count];
    usedQuestions[randomIndex] = true;
    return questions[randomIndex];
}

bool SphinxGame::Run(Console consoleSettings)
{
    SetGoodStyle(consoleSettings); // ya no cambia tamaño agresivamente

    std::string key = utils.GetAssetsPath() + "maps\\sphinx\\sphinx.txt";
    LoadLevel(key); // Ya pone mapa completo

    isRunning = true;

    // Mostrar TODO el mapa
    viewWidth = 30; // o el tamaño que tú quieras
    viewHeight = 12;

    // Centrado
    offsetX = 2; // fijo en la izquierda
    offsetY = 1; // o mantén centrado verticalmente si quieres
    offsetX = std::max(0, offsetX);
    offsetY = std::max(0, offsetY);

    hud.SetCenteredOffset(offsetX);

    while (isRunning)
    {

        map.DrawWithSphinx(viewWidth, player.GetX(), player.GetY(), offsetX, offsetY);
        hud.Draw(player, currentQuestion, viewWidth, answerStatus);
        // Mostrar pregunta
        int preguntaY = offsetY + viewHeight + 1;
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

        Sleep(100);
    }

    return (lives > 0);
}

void SphinxGame::ProcessInput(char input)
{
    std::pair<int, int> dir = player.GetInputDirection(input);
    int dx = dir.first, dy = dir.second;

    if (dx == 0 && dy == 0)
        return;

    int newX = player.GetX() + dx;
    int newY = player.GetY() + dy;
    wchar_t tile = map.GetTile(newX, newY);

    if (tile != L'A' && tile != L'B' && tile != L'#')
    {
        player.SetPosition(newX, newY);

        if (!questionAnswered && (tile == L'1' || tile == L'2' || tile == L'3'))
        {
            int answer = tile - L'1';
            questionAnswered = true;

            if (answer == question.correctAnswer)
            {
                answerStatus = 1;
                correctAnswers++;
                currentQuestion++;

                if (correctAnswers >= totalQuestions)
                {
                    utils.ClearScreen();
                    std::wcout << L"\n\n\nYou passed the Sphinx's challenge!\n";
                    isRunning = false;
                    return;
                }

                question = GenerateQuestion();
            }
            else
            {
                answerStatus = 0;
                lives--;
                player.SetLives(lives);

                if (lives <= 0)
                {
                    utils.ClearScreen();
                    std::wcout << L"\n\n\nYou lost. The Sphinx has defeated you.\n";
                    isRunning = false;
                    return;
                }
            }

            questionAnswered = false;
        }
    }
}

void SphinxGame::LoadLevel(const std::string &key)
{
    utils.ClearScreenComplety();
    map.ReadMap(key, 0, 0);     // usa dimensiones del archivo
    player.SetPosition(14, 17); // Puedes ajustar esto según mapa
    playerX = player.GetX();
    playerY = player.GetY();
}

void SphinxGame::SetGoodStyle(Console consoleSettings)
{
    // consoleSettings.SetConsoleFont(10, 18, L"Consolas");
    std::wcout.imbue(std::locale(""));
    utils.ClearScreenComplety();
    utils.PrintCentered(L"The sphinx challenges you with geography questions...");
    Sleep(1300);
}

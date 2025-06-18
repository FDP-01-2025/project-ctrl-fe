#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h> // For Windows-specific console functions
#endif

class Utils
{
public:
        // Clears the console screen by moving the cursor to the top-left corner
        void ClearScreen()
        {
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord = {0, 0};
                SetConsoleCursorPosition(hConsole, coord);
#else
                std::cout << "\033[H"; // ANSI escape code to move cursor to top-left
#endif
        }

        // Completely clears the console screen, including filling it with spaces and resetting colors
        void ClearScreenComplety()
        {
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                DWORD written, cells;

                if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
                        return;

                cells = csbi.dwSize.X * csbi.dwSize.Y;

                // Fill the entire screen with spaces
                FillConsoleOutputCharacter(hConsole, ' ', cells, {0, 0}, &written);
                // Reset all attributes (like colors) to default
                FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, {0, 0}, &written);
                // Move the cursor to the top-left corner
                SetConsoleCursorPosition(hConsole, {0, 0});
#else
                std::cout << "\033[2J\033[H"; // ANSI codes to clear screen and move cursor home
#endif
        }

        // Pauses the execution of the program for a specified amount of milliseconds
        void Sleep(int milliseconds)
        {
                // std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
                // (Currently commented out)
        }

        // Moves the console cursor to a specific position (x: column, y: row)
        void MoveCursor(int x, int y)
        {
#ifdef _WIN32
                COORD pos = {(SHORT)x, (SHORT)y};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
                std::cout << "\033[" << y << ";" << x << "H"; // ANSI escape code for cursor positioning
#endif
        }

        // Reads the entire content of a text file and returns it as a std::string
        std::string ReadFile(const std::string &path)
        {
                std::ifstream file(path);
                if (!file)
                        return "";

                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                return content;
        }

        // Function to get the assets directory path relative to the executable's directory
        std::string GetAssetsPath()
        {
                // Buffer to store the executable path
                char buffer[MAX_PATH];
                // Get the full path of the current executable
                GetModuleFileNameA(NULL, buffer, MAX_PATH);
                // Convert to std::string for easier manipulation
                std::string fullPath(buffer);
                // Find the last path separator '\' or '/' to isolate directory
                size_t pos = fullPath.find_last_of("\\/");
                if (pos == std::string::npos)
                        return ""; // Return empty string if no separator found

                std::string exeDir = fullPath.substr(0, pos);
                // Return the relative path to the assets folder (one directory up + assets)
                return exeDir + "\\..\\assets\\";
        }

        // Function use to prints a single line of text at a specific position in the HUD.
        // Automatically moves the cursor to the next line (y++).
        void PrintLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT) const
        {
                std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET;
                y++;
        }
};

#endif
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h> // Para funciones específicas de consola en Windows
#endif

class Utils
{
public:
        // Limpia la pantalla de la consola
        void ClearScreen()
        {
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                COORD coord = {0, 0};
                SetConsoleCursorPosition(hConsole, coord);
#else
                std::cout << "\033[H";
#endif
        }

        void ClearScreenComplety()
        {
#ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                DWORD written, cells;

                if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
                        return;

                cells = csbi.dwSize.X * csbi.dwSize.Y;

                // Llenar toda la pantalla con espacios
                FillConsoleOutputCharacter(hConsole, ' ', cells, {0, 0}, &written);
                // Llenar los atributos (color) por defecto
                FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, {0, 0}, &written);
                // Posicionar el cursor en la esquina superior izquierda
                SetConsoleCursorPosition(hConsole, {0, 0});
#else
                std::cout << "\033[2J\033[H";
#endif
        }

        // Pausa la ejecución del programa durante un tiempo dado (en milisegundos)
        void Sleep(int milliseconds)
        {
                // std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }

        // Mueve el cursor a una posición específica en la consola (x: columna, y: fila)
        void MoveCursor(int x, int y)
        {
#ifdef _WIN32
                COORD pos = {(SHORT)x, (SHORT)y};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
                std::cout << "\033[" << y << ";" << x << "H";
#endif
        }

        // Lee el contenido completo de un archivo de texto y lo retorna como string
        std::string ReadFile(const std::string &path)
        {
                std::ifstream file(path);
                if (!file)
                        return "";

                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                return content;
        }

        //Function to get the assets path relative to the executable directory
        std::string GetAssetsPath()
        {
                // Declaration of the buffer to hold the path
                char buffer[MAX_PATH];
                // Get the full path of the executable and extract the directory
                GetModuleFileNameA(NULL, buffer, MAX_PATH);
                // Find the last occurrence of the path separator to get the directory
                std::string fullPath(buffer);
                // Find the last occurrence of either '\' or '/' to handle both Windows and Unix-like paths
                size_t pos = fullPath.find_last_of("\\/");
                // If not found, return an empty string
                std::string exeDir = fullPath.substr(0, pos);
                // Return the assets path relative to the executable directory
                return exeDir + "\\..\\assets\\"; 
        }
};

#endif // UTILS_H

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
    void clearScreen()
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {0, 0};                      
        SetConsoleCursorPosition(hConsole, coord);
#else
        std::cout << "\033[H";
#endif
    }

    // Pausa la ejecución del programa durante un tiempo dado (en milisegundos)
    void sleep(int milliseconds)
    {
       // std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    // Mueve el cursor a una posición específica en la consola (x: columna, y: fila)
    void moveCursor(int x, int y)
    {
#ifdef _WIN32
        COORD pos = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
        std::cout << "\033[" << y << ";" << x << "H";
#endif
    }

    // Lee el contenido completo de un archivo de texto y lo retorna como string
    std::string readFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
            return "";

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content;
    }
};

#endif // UTILS_H

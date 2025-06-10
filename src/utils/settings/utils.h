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

// Namespace que agrupa funciones utilitarias comunes para el juego
namespace Utils {

    // Limpia la pantalla de la consola
    void clearScreen();

    // Pausa la ejecución del programa durante un tiempo dado (en milisegundos)
    void sleep(int milliseconds);

    // Mueve el cursor a una posición específica en la consola (x: columna, y: fila)
    void moveCursor(int x, int y);

    // Lee el contenido completo de un archivo de texto y lo retorna como string
    std::string readFile(const std::string& path);

    // ------------------- Implementaciones -------------------

    // Limpia la pantalla o coloca el cursor al inicio (según el sistema operativo)
    void clearScreen() {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {0, 0}; // Coordenadas al inicio
        SetConsoleCursorPosition(hConsole, coord); // Mueve cursor arriba a la izquierda
    #else
        std::cout << "\033[H"; // Secuencia ANSI para ir a la parte superior izquierda
    #endif
    }

    /* Duerme el hilo actual por la cantidad de milisegundos indicada
    void sleep(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }*/

    // Mueve el cursor a la posición (x, y) en la consola
    void moveCursor(int x, int y) {
    #ifdef _WIN32
        COORD pos = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    #else
        // Secuencia ANSI para mover el cursor a (x, y)
        std::cout << "\033[" << y << ";" << x << "H";
    #endif
    }

    // Lee un archivo de texto y retorna todo su contenido como un string
    std::string readFile(const std::string &path) {
        std::ifstream file(path); // Abre archivo en modo lectura
        if (!file)
            return ""; // Si no se puede abrir, retorna string vacío

        // Lee todo el contenido del archivo usando iteradores
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return content;
    }
}

#endif // UTILS_H

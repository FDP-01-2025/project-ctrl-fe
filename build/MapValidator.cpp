#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

int main() {
    // Configurar la consola para UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Ruta del archivo (usamos string normal)
    std::string path = "C:\\Users\\RICARDO MEGIA\\Documents\\C++\\project-ctrl-fe\\assets\\maps\\bomberman\\easy-levels\\level1.txt";

    // Abrir archivo en modo binario
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo.\n";
        return 1;
    }

    // Leer contenido completo
    std::wstring content((std::istreambuf_iterator<char>(file)), 
                     std::istreambuf_iterator<char>());

    // Imprimir directamente
    std::wcout << content;

    return 0;
}
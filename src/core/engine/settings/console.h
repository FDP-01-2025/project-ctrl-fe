#pragma once

// Include de librerías necesarias
#include <windows.h> // API de Windows para control de consola
#include <iostream>  // Para salida de errores (std::cerr)
#include <cstdio>    // Para freopen_s
#include <conio.h>   // Para _kbhit, _getch
#include <wchar.h>   // Para funciones de caracteres anchos
#include <algorithm> // Para std::max y std::min
#include <io.h>
#include <fcntl.h>
#include "utils\functions\utils.h"

// Comentado: definición de CONSOLE_FONT_INFOEX por si no está disponibleMore actions

#ifndef _CONSOLE_FONT_INFOEX
#define _CONSOLE_FONT_INFOEX

typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
#endif

// Define un puntero a función que apunta a SetCurrentConsoleFontEx de Windows API
typedef BOOL(WINAPI *PFN_SetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

// Clase Console para manejar la configuración y apariencia de la consola de Windows
class Console
{
public:
    // Constructor con valores por defecto para dimensiones de consola, buffer y fuente
    Console(int w = 100, int h = 40, int fw = 10, int fh = 16)
        : consoleW(w), consoleH(h), fontW(fw), fontH(fh)
    {
        // Buffer igual al tamaño de ventana por defecto
        bufferW = w;
        bufferH = h;
    }

    // Configura la consola: crea una nueva consola, redirige entradas/salidas y aplica ajustes
    void ConfigConsole()
    {
        Utils utils;
        FreeConsole(); // Libera la consola actual (si existe)More actions

        if (AllocConsole()) // Asigna una nueva consola
        {
            // Redirige stdout, stdin y stderr a la consola recién creadaMore actions
            if (!freopen("CONOUT$", "w", stdout))
                std::cerr << "Error al redirigir stdout.\n";
            if (!freopen("CONIN$", "r", stdin))
                std::cerr << "Error al redirigir stdin.\n";
            if (!freopen("CONOUT$", "w", stderr))
                std::cerr << "Error al redirigir stderr.\n";

            utils.SetUtf8();

            // Obtiene los handles de entrada y salida de la consola
            hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
            hConsoleIN = GetStdHandle(STD_INPUT_HANDLE);

            // Verifica si el handle es válido
            if (hConsoleOUT == NULL || hConsoleOUT == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: Handle de consola invalido.\n";
                return;
            }
            // Si la configuración es válida, aplica los cambios visuales
            if (ValidConfigs(hConsoleOUT))
            {
                ApllySettings();  // Aplica tamaño de buffer y ventana
                SetConsoleFont(); // Cambia el tipo y tamaño de fuente
                HideCursor();     // Oculta el cursor
                CenterWindow();   // Centra la ventana en la pantalla
            }
        }
        else
        {
            std::cerr << "No se pudo crear la consola." << std::endl;
        }
    }

    // Verifica y ajusta configuraciones de consola según límites máximos
    bool ValidConfigs(HANDLE handle)
    {
        if (!GetConsoleScreenBufferInfo(handle, &csbi))
        {
            DWORD error = GetLastError();
            LPSTR errorMsg = nullptr;

            FormatMessageA( // Obtiene mensaje legible del error
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                error,
                0,
                (LPSTR)&errorMsg,
                0,
                nullptr);

            std::cerr << "Error al obtener info de la consola. Codigo: " << error << "\n";
            std::cerr << "Mensaje: " << (errorMsg ? errorMsg : "Error desconocido") << "\n";
            std::cerr << "Error obteniendo info consola\n";
            return false;
        }

        maxSize = csbi.dwMaximumWindowSize;

        // Ajustar a máximos permitidos
        consoleW = std::min(consoleW, static_cast<int>(maxSize.X));
        consoleH = std::min(consoleH, static_cast<int>(maxSize.Y));

        // Buffer DEBE ser igual al tamaño de ventana
        bufferW = consoleW;
        bufferH = consoleH;

        return true;
    }

    // Aplica tamaño del buffer y la ventana de la consola
    void ApllySettings()
    {
        // Paso 1: Reducir temporalmente la ventana (antes de cambiar el buffer)
        SMALL_RECT tmp = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tmp);

        // Paso 2: Cambiar el tamaño del buffer
        bufferSize = {static_cast<SHORT>(consoleW), static_cast<SHORT>(consoleH)};
        if (!SetConsoleScreenBufferSize(hConsoleOUT, bufferSize))
        {
            std::cerr << "Error estableciendo buffer: " << GetLastError() << "\n";
            return;
        }

        // Paso 3: Establecer tamaño final de la ventana
        windowSize = {0, 0, static_cast<SHORT>(consoleW - 1), static_cast<SHORT>(consoleH - 1)};
        if (!SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize))
        {
            std::cerr << "Error ajustando ventana: " << GetLastError() << "\n";
            // Intentar con tamaño máximo
            windowSize = {0, 0, static_cast<SHORT>(csbi.dwMaximumWindowSize.X - 1),
                          static_cast<SHORT>(csbi.dwMaximumWindowSize.Y - 1)};
            SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize);
        }
    }

    void SetConsoleFont()
    {
        HMODULE hKernel = GetModuleHandleA("kernel32.dll");
        if (!hKernel)
        {
            std::cerr << "No se pudo obtener kernel32.dll\n";
            return;
        }

        PFN_SetCurrentConsoleFontEx pSetFont = (PFN_SetCurrentConsoleFontEx)GetProcAddress(hKernel, "SetCurrentConsoleFontEx");
        if (!pSetFont)
        {
            std::cerr << "SetCurrentConsoleFontEx no disponible en esta versión de Windows\n";
            return;
        }

        CONSOLE_FONT_INFOEX cfi = {0};
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        cfi.dwFontSize.X = static_cast<SHORT>(fontW);
        cfi.dwFontSize.Y = static_cast<SHORT>(fontH);
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        lstrcpyW(cfi.FaceName, L"Lucida Console");

        if (!pSetFont(hConsoleOUT, FALSE, &cfi))
        {
            std::cerr << "Error cambiando fuente\n";
        }
    }

    // Centra la ventana de la consola en la pantalla
    void CenterWindow()
    {
        HWND hwnd = GetConsoleWindow();
        if (!hwnd)
            return;

        RECT workArea, rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
        GetWindowRect(hwnd, &rect);

        int posX = workArea.left + (workArea.right - workArea.left - (rect.right - rect.left)) / 2;
        int posY = workArea.top + (workArea.bottom - workArea.top - (rect.bottom - rect.top)) / 2;

        SetWindowPos(hwnd, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    // Cambia el título de la ventana de consola
    void SetTitle(const wchar_t *title)
    {
        SetConsoleTitleW(title);
    }

    // Cambia el color de texto de la consola
    void SetColor(WORD attributes)
    {
        SetConsoleTextAttribute(hConsoleOUT, attributes);
    }

    // Oculta el cursor de texto en la consola
    void HideCursor()
    {
        if (hConsoleOUT == INVALID_HANDLE_VALUE)
        {
            hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsoleOUT == INVALID_HANDLE_VALUE)
                return;
        }

        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsoleOUT, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsoleOUT, &cursorInfo);
    }

protected:
    // Parámetros de configuración
    int consoleW; // Ancho de la ventana
    int consoleH; // Alto de la ventana
    int bufferW;  // Ancho del buffer
    int bufferH;  // Alto del buffer
    int fontW;    // Ancho de fuente
    int fontH;    // Alto de fuente

    // Handles de entrada y salida
    HANDLE hConsoleOUT;
    HANDLE hConsoleIN;

    // Información del estado de la consola
    SMALL_RECT windowSize;
    COORD bufferSize;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD maxSize;
    CONSOLE_CURSOR_INFO cursorInfo;
};
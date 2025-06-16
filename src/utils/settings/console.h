#pragma once

// Include de librerías necesarias
#include <windows.h> // API de Windows para control de consola
#include <iostream>  // Para salida de errores (std::cerr)
#include <cstdio>    // Para freopen_s
#include <conio.h>   // Para _kbhit, _getch
#include <wchar.h>   // Para funciones de caracteres anchos
#include <algorithm> // Para std::max y std::min

// Define un puntero a función que apunta a SetCurrentConsoleFontEx de Windows API
typedef BOOL(WINAPI *PFN_SetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

// Clase Console para manejar la configuración y apariencia de la consola de Windows
class Console
{
public:
    // Constructor con valores por defecto para dimensiones de consola, buffer y fuente
    Console(int w = 100, int h = 40, int fw = 8, int fh = 16)
        : consoleW(w), consoleH(h), fontW(fw), fontH(fh)
    {
        // Buffer igual al tamaño de ventana por defecto
        bufferW = w;
        bufferH = h;
    }

    // Configura la consola: crea una nueva consola, redirige entradas/salidas y aplica ajustes
    void ConfigConsole()
    {
        bool attached = AttachConsole(ATTACH_PARENT_PROCESS);
        if (!attached)
        {
            // No hay consola padre, crear una nueva consola
            if (!AllocConsole())
            {
                std::cerr << "No se pudo crear consola." << std::endl;
                return;
            }

            // Redirigir streams estándar a la consola creada
            FILE *fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
            freopen_s(&fp, "CONIN$", "r", stdin);
        }

        hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
        hConsoleIN = GetStdHandle(STD_INPUT_HANDLE);

        // PRIMERO: Configurar buffer
        bufferSize = {static_cast<SHORT>(consoleW), static_cast<SHORT>(consoleH)};
        SetConsoleScreenBufferSize(hConsoleOUT, bufferSize);

        // SEGUNDO: Configurar ventana (en 2 pasos)
        SMALL_RECT tmp = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tmp);

        windowSize = {0, 0, static_cast<SHORT>(consoleW - 1), static_cast<SHORT>(consoleH - 1)};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize);

        // TERCERO: Fuente y otros ajustes
        SetConsoleFont();
        Sleep(200); // Espera crítica
        CenterWindow();
        HideCursor();
    }

    // Verifica y ajusta configuraciones de consola según límites máximos
    bool ValidConfigs(HANDLE handle)
    {
        if (!GetConsoleScreenBufferInfo(handle, &csbi))
        {
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
        bufferSize = {static_cast<SHORT>(consoleW), static_cast<SHORT>(consoleH)};
        if (!SetConsoleScreenBufferSize(hConsoleOUT, bufferSize))
        {
            std::cerr << "Error estableciendo buffer: " << GetLastError() << "\n";
            return;
        }

        // Paso 1: Reducir temporalmente
        SMALL_RECT tmp = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tmp);

        // Paso 2: Establecer tamaño final
        windowSize = {0, 0, static_cast<SHORT>(consoleW - 1), static_cast<SHORT>(consoleH - 1)};
        if (!SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize))
        {
            std::cerr << "Error ajustando ventana: " << GetLastError() << "\n";
            // Si falla, intentar con tamaño máximo permitido
            windowSize = {0, 0, static_cast<SHORT>(csbi.dwMaximumWindowSize.X - 1),
                          static_cast<SHORT>(csbi.dwMaximumWindowSize.Y - 1)};
            SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize);
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
    // Cambia la fuente de la consola
    void SetConsoleFont()
    {
        HMODULE hKernel = GetModuleHandleA("kernel32.dll");
        if (!hKernel)
        {
            std::cerr << "No se pudo obtener kernel32.dll\n";
        }

        PFN_SetCurrentConsoleFontEx pSetFont = (PFN_SetCurrentConsoleFontEx)GetProcAddress(hKernel, "SetCurrentConsoleFontEx");

        if (!pSetFont)
        {
            std::cerr << "SetCurrentConsoleFontEx no disponible en esta version de Windows\n";
        }

        // Configura los parámetros de fuente
        CONSOLE_FONT_INFOEX cfi = {0};
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        cfi.dwFontSize.X = fontW;
        cfi.dwFontSize.Y = fontH;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        lstrcpyW(cfi.FaceName, L"Consolas"); // Fuente preferida

        if (!pSetFont(hConsoleOUT, FALSE, &cfi))
        {
            std::cerr << "Error cambiando fuente\n";
        }
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

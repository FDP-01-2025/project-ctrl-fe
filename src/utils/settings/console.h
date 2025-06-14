#pragma once // Asegura que el archivo solo se incluya una vez en la compilación

// Include de librerías necesarias
#include <windows.h>   // Librería principal para manipulación de consola en Windows
#include <iostream>    
#include <cstdio>      // Para funciones de C como freopen
#include <conio.h>     // Para manejo de teclado (_getch, _kbhit, etc.)
#include <wchar.h>     // Para soporte de wide characters (wchar_t, lstrcpyW, etc.)

// Comentado: definición de CONSOLE_FONT_INFOEX por si no está disponible
/*
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
*/

// Define un puntero a función que apunta a SetCurrentConsoleFontEx de Windows API
typedef BOOL(WINAPI *PFN_SetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

// Clase Console para manejar la configuración y apariencia de la consola de Windows
class Console
{
public:
    // Constructor con valores por defecto para dimensiones de consola, buffer y fuente
    Console(int w = 100, int h = 40, int bW = 100, int bH = 40, int fw = 8, int fh = 16)
        : consoleW(w), consoleH(h), bufferW(bW), bufferH(bH), fontW(fw), fontH(fh) {}

    // Permite reiniciar el tamaño deseado de la consola
    void ResetConfigure(int w, int h)
    {
        consoleW = w;
        consoleH = h;
    }

    // Configura la consola: crea una nueva consola, redirige entradas/salidas y aplica ajustes
    void ConfigConsole()
    {
        FreeConsole(); // Libera la consola actual (si existe)

        if (AllocConsole()) // Asigna una nueva consola
        {
            // Redirige stdout, stdin y stderr a la consola recién creada
            if (!freopen("CONOUT$", "w", stdout))
                std::cerr << "Error al redirigir stdout.\n";
            if (!freopen("CONIN$", "r", stdin))
                std::cerr << "Error al redirigir stdin.\n";
            if (!freopen("CONOUT$", "w", stderr))
                std::cerr << "Error al redirigir stderr.\n";

            // Obtiene los handles de entrada y salida de la consola
            hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
            hConsoleIN = GetStdHandle(STD_INPUT_HANDLE);

            // Verifica si el handle es válido
            if (hConsoleOUT == NULL || hConsoleOUT == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: Handle de consola invalido.\n";
                return;
            }

            // 🟡 Habilita soporte para secuencias ANSI (colores y estilos \033[...m)
            DWORD dwMode = 0;
            if (GetConsoleMode(hConsoleOUT, &dwMode))
            {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                if (!SetConsoleMode(hConsoleOUT, dwMode))
                {
                    std::cerr << "No se pudo habilitar procesamiento ANSI.\n";
                }
            }

            // Si la configuración es válida, aplica los cambios visuales
            if (ValidConfigs(hConsoleOUT))
            {
                ApllySettings();   // Aplica tamaño de buffer y ventana
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
        if (!GetConsoleScreenBufferInfo(handle, &csbi)) // Obtiene info actual del buffer
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
            return false;
        }

        maxSize = csbi.dwMaximumWindowSize;

        // Ajusta dimensiones si exceden los máximos
        if (consoleW > maxSize.X)
            consoleW = maxSize.X;
        if (consoleH > maxSize.Y)
            consoleH = maxSize.Y;

        // Ajusta buffer si es menor que la ventana
        if (bufferW < consoleW)
            bufferW = consoleW;
        if (bufferH < consoleH)
            bufferH = consoleH;

        return true;
    }

    // Aplica tamaño del buffer y la ventana de la consola
    void ApllySettings()
    {
        // Reduce ventana a 1x1 temporalmente para permitir cambio de buffer
        SMALL_RECT tempWindow = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tempWindow);

        // Establece tamaño del buffer
        bufferSize = {(SHORT)bufferW, (SHORT)bufferH};
        if (!SetConsoleScreenBufferSize(hConsoleOUT, bufferSize))
        {
            std::cerr << "Error al establecer tamano buffer. Codigo: " << GetLastError() << "\n";
            return;
        }

        // Establece tamaño final de la ventana
        windowSize = {0, 0, (SHORT)(consoleW - 1), (SHORT)(consoleH - 1)};
        if (!SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize))
        {
            std::cerr << "Error al establecer tamano ventana. Codigo: " << GetLastError() << "\n";
            return;
        }
    }

    // Centra la ventana de la consola en la pantalla
    void CenterWindow()
    {
        HWND hwnd = GetConsoleWindow();
        if (!hwnd)
            return;

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        RECT rect;
        GetWindowRect(hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        int posX = (screenWidth - windowWidth) / 2;
        int posY = (screenHeight - windowHeight) / 2;

        // Posiciona ventana al centro sin cambiar tamaño ni orden Z
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

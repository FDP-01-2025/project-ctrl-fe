#pragma once

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <wchar.h>

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

typedef BOOL(WINAPI *PFN_SetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

class Console
{
public:
    Console(int w = 100, int h = 40, int bW = 100, int bH = 40, int fw = 8, int fh = 16)
        : consoleW(w), consoleH(h), bufferW(bW), bufferH(bH), fontW(fw), fontH(fh) {}

    void configConsole()
    {
        // Libera cualquier consola existente (por si este .exe es de consola)
        FreeConsole();

        // Crea una nueva consola para el proceso actual (solo funciona si no se tiene ya una consola activa)
        if (AllocConsole())
        {
            // Redirige entrada y salida estándar a la nueva consola
            if (!freopen("CONOUT$", "w", stdout))
                std::cerr << "Error al redirigir stdout.\n";
            if (!freopen("CONIN$", "r", stdin))
                std::cerr << "Error al redirigir stdin.\n";
            if (!freopen("CONOUT$", "w", stderr))
                std::cerr << "Error al redirigir stderr.\n";

            hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsoleOUT == NULL || hConsoleOUT == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: Handle de consola invalido.\n";
                return;
            }

            // Validamos que las configuraciones que queremos estan dentro de los limites
            if (validConfigs(hConsoleOUT))
            {
                apllySettings();
                setConsoleFont();
                hideCursor();
                centerWindow();
            }
        }
        else
        {
            // Si no se pudo crear la consola (por ejemplo, ya hay una activa), se muestra un error
            std::cerr << "No se pudo crear la consola." << std::endl;
        }
    }

    bool validConfigs(HANDLE handle)
    {
        // Obtener info actual de consola para conocer límites
        if (!GetConsoleScreenBufferInfo(handle, &csbi))
        {
            // Obtiene el último error de Windows
            DWORD error = GetLastError();

            // Buffer para el mensaje de error
            LPSTR errorMsg = nullptr;

            // Convierte el código de error en un mensaje legible
            FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                error,
                0,
                (LPSTR)&errorMsg,
                0,
                nullptr);

            // Imprime el error
            std::cerr << "Error al obtener info de la consola. Codigo: " << error << "\n";
            std::cerr << "Mensaje: " << (errorMsg ? errorMsg : "Error desconocido") << "\n";
            return false;
        }

        maxSize = csbi.dwMaximumWindowSize;

        // Ajustar ancho y alto para que no superen máximo permitido
        if (consoleW > maxSize.X)
            consoleW = maxSize.X;
        if (consoleH > maxSize.Y)
            consoleH = maxSize.Y;

        // Buffer debe ser >= ventana
        if (bufferW < consoleW)
            bufferW = consoleW;
        if (bufferH < consoleH)
            bufferH = consoleH;

        return true;
    }

    void apllySettings()
    {
        SMALL_RECT tempWindow = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tempWindow);

        // Establecer tamaño del buffer
        bufferSize = {(SHORT)bufferW, (SHORT)bufferH};
        if (!SetConsoleScreenBufferSize(hConsoleOUT, bufferSize))
        {
            std::cerr << "Error al establecer tamano buffer. Codigo: " << GetLastError() << "\n";
            return;
        }

        // Ahora establecer el tamaño real de la ventana visible
        windowSize = {0, 0, (SHORT)(consoleW - 1), (SHORT)(consoleH - 1)};
        if (!SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize))
        {
            std::cerr << "Error al establecer tamano ventana. Codigo: " << GetLastError() << "\n";
            return;
        }
    }

    void centerWindow()
    {
        HWND hwnd = GetConsoleWindow(); // Obtiene el handle de la ventana de consola
        if (!hwnd)
            return;

        // Tamaño de pantalla
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // Tamaño actual de la ventana de consola
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int windowWidth = rect.right - rect.left;
        int windowHeight = rect.bottom - rect.top;

        // Posición para centrar
        int posX = (screenWidth - windowWidth) / 2;
        int posY = (screenHeight - windowHeight) / 2;

        // Mover ventana al centro
        SetWindowPos(hwnd, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    void setConsoleFont()
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

        CONSOLE_FONT_INFOEX cfi = {0};
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        cfi.dwFontSize.X = fontW;
        cfi.dwFontSize.Y = fontH;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        lstrcpyW(cfi.FaceName, L"Terminal"); // O "Consolas", "Lucida Console"

        if (!pSetFont(hConsoleOUT, FALSE, &cfi))
        {
            std::cerr << "Error cambiando fuente\n";
        }
    }

    void setTitle(const wchar_t *title)
    {
        SetConsoleTitleW(title);
    }

    void setColor(WORD attributes)
    {
        SetConsoleTextAttribute(hConsoleOUT, attributes);
    }

    void hideCursor()
    {
        GetConsoleCursorInfo(hConsoleOUT, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsoleOUT, &cursorInfo);
    }

protected:
    // Parámetros de configuración
    int consoleW;
    int consoleH;
    int bufferW;
    int bufferH;
    int fontW;
    int fontH;

    // Handles para la consola
    HANDLE hConsoleOUT;
    HANDLE hConsoleIN;

    // Tamaño de la ventana visible de la consola
    SMALL_RECT windowSize;

    // Buffer
    COORD bufferSize;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD maxSize;

    CONSOLE_CURSOR_INFO cursorInfo;
};

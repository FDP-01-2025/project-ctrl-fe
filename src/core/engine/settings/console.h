#pragma once

// Include necessary libraries
#include <windows.h> // Windows API for console control
#include <iostream>  // For error output (std::cerr)
#include <cstdio>    // For freopen_s
#include <conio.h>   // For _kbhit, _getch
#include <wchar.h>   // For wide character functions
#include <algorithm> // For std::max and std::min
#include <io.h>
#include <fcntl.h>
#include "utils\functions\utils.h"

// Commented out: Definition of CONSOLE_FONT_INFOEX in case it's not available

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
// Define a function pointer type that points to Windows API's SetCurrentConsoleFontEx function
typedef BOOL(WINAPI *PFN_SetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

// Console class to manage Windows console configuration and appearance
class Console
{
public:
    // Constructor with default values for console dimensions, buffer, and font
    Console(int w = 100, int h = 45, int fw = 10, int fh = 16)
        : consoleW(w), consoleH(h), fontW(fw), fontH(fh),
          originalW(w), originalH(h), originalFW(fw), originalFH(fh) // Save original values
    {
        bufferW = w;
        bufferH = h;
    }
    // Configure the console: allocate a new console, redirect I/O, and apply settings
    void ConfigConsole()
    {
        Utils utils;
        FreeConsole(); // Release the current console (if any)

        if (AllocConsole()) // Allocate a new console
        {
            // Redirect stdout, stdin, and stderr to the new console
            if (!freopen("CONOUT$", "w", stdout))
                std::cerr << "Error redirecting stdout.\n";
            if (!freopen("CONIN$", "r", stdin))
                std::cerr << "Error redirecting stdin.\n";
            if (!freopen("CONOUT$", "w", stderr))
                std::cerr << "Error redirecting stderr.\n";

            utils.SetUtf8();

            // Get the handles for console input and output
            hConsoleOUT = GetStdHandle(STD_OUTPUT_HANDLE);
            hConsoleIN = GetStdHandle(STD_INPUT_HANDLE);

            // Check if handles are valid
            if (hConsoleOUT == NULL || hConsoleOUT == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Error: Invalid console handle.\n";
                return;
            }
            // If configuration is valid, apply visual changes
            if (ValidConfigs(hConsoleOUT))
            {
                ApllySettings(); // Apply buffer and window size
                Sleep(10);
                hwnd = GetConsoleWindow();
                SetConsoleFont(); // Change font type and size
                HideCursor();     // Hide the cursor
                StylizeWindow();
                CenterWindow(); // Center the window on the screen
            }
        }
        else
        {
            std::cerr << "Failed to create console." << std::endl;
        }
    }

    // Validate and adjust console configurations according to maximum limits
    bool ValidConfigs(HANDLE handle)
    {
        if (!GetConsoleScreenBufferInfo(handle, &csbi))
        {
            DWORD error = GetLastError();
            LPSTR errorMsg = nullptr;

            FormatMessageA( // Get readable error message
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                error,
                0,
                (LPSTR)&errorMsg,
                0,
                nullptr);

            std::cerr << "Error getting console info. Code: " << error << "\n";
            std::cerr << "Message: " << (errorMsg ? errorMsg : "Unknown error") << "\n";
            std::cerr << "Error retrieving console info\n";
            return false;
        }
        return true;
    }

    // Apply console buffer and window size
    void ApllySettings()
    {
        // Step 1: Temporarily shrink the window before changing buffer size
        SMALL_RECT tmp = {0, 0, 1, 1};
        SetConsoleWindowInfo(hConsoleOUT, TRUE, &tmp);

        // Step 2: Change the buffer size
        bufferSize = {static_cast<SHORT>(consoleW), static_cast<SHORT>(consoleH)};
        if (!SetConsoleScreenBufferSize(hConsoleOUT, bufferSize))
        {
            std::cerr << "Error setting buffer: " << GetLastError() << "\n";
            return;
        }

        // Step 3: Set final window size
        windowSize = {0, 0, static_cast<SHORT>(consoleW - 1), static_cast<SHORT>(consoleH - 1)};
        if (!SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize))
        {
            std::cerr << "Error setting window size: " << GetLastError() << "\n";
            // Try max possible size
            windowSize = {0, 0, static_cast<SHORT>(csbi.dwMaximumWindowSize.X - 1),
                          static_cast<SHORT>(csbi.dwMaximumWindowSize.Y - 1)};
            SetConsoleWindowInfo(hConsoleOUT, TRUE, &windowSize);
        }
    }

    // Public function without parameters that uses default class values
    void SetConsoleFont()
    {
        SetConsoleFontImpl(originalFW, originalFH, L"Lucida console");
        ResetConsoleSize();
    }

    // Set console font with specified width, height, and font face name
    void SetConsoleFont(int width, int height, const wchar_t *faceName)
    {
        // Save current physical window dimensions
        GetWindowRect(hwnd, &rect);
        int currentWidth = rect.right - rect.left;
        int currentHeight = rect.bottom - rect.top;

        // Calculate new console width and height in characters
        int newConsoleW = currentWidth / width;
        int newConsoleH = currentHeight / height;

        consoleW = newConsoleW;
        consoleH = newConsoleH;

        // Apply changes
        SetConsoleFontImpl(width, height, faceName);
        ApllySettings();
    }

    // Center the console window on the screen
    void CenterWindow()
    {
        if (!hwnd)
            return;

        RECT workArea, rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
        GetWindowRect(hwnd, &rect);

        int posX = workArea.left + (workArea.right - workArea.left - (rect.right - rect.left)) / 2;
        int posY = workArea.top + (workArea.bottom - workArea.top - (rect.bottom - rect.top)) / 2;

        SetWindowPos(hwnd, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    // Change the console window title
    void SetTitle(const wchar_t *title)
    {
        SetConsoleTitleW(title);
    }

    // Change the text color of the console
    void SetColor(WORD attributes)
    {
        SetConsoleTextAttribute(hConsoleOUT, attributes);
    }

    // Hide the text cursor in the console
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

    // Remove border and resizing styles from the window
    void StylizeWindow()
    {
        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~WS_CAPTION;
        style &= ~WS_THICKFRAME;
        style &= ~WS_MAXIMIZEBOX;
        style &= ~WS_MINIMIZEBOX;
        style &= ~WS_SYSMENU;

        SetWindowLong(hwnd, GWL_STYLE, style);

        // Force Windows to reapply the styles
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

private:
    // Internal helper method to set the console font using Windows API
    void SetConsoleFontImpl(int width, int height, const wchar_t *faceName)
    {
        // Get a handle to the kernel32.dll module where the function is located
        HMODULE hKernel = GetModuleHandleA("kernel32.dll");
        if (!hKernel)
        {
            std::cerr << "Failed to get kernel32.dll\n"; // Unable to get module handle
            return;
        }

        // Get the address of the SetCurrentConsoleFontEx function dynamically
        PFN_SetCurrentConsoleFontEx pSetFont = (PFN_SetCurrentConsoleFontEx)GetProcAddress(hKernel, "SetCurrentConsoleFontEx");
        if (!pSetFont)
        {
            // The function may not be available on older Windows versions
            std::cerr << "SetCurrentConsoleFontEx not available on this Windows version\n";
            return;
        }

        // Prepare the CONSOLE_FONT_INFOEX structure to specify the font parameters
        CONSOLE_FONT_INFOEX cfi = {0};
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);      // Size of this structure
        cfi.dwFontSize.X = static_cast<SHORT>(width);  // Font width in pixels
        cfi.dwFontSize.Y = static_cast<SHORT>(height); // Font height in pixels
        cfi.FontFamily = FF_DONTCARE;                  // Let the system choose the font family
        cfi.FontWeight = FW_NORMAL;                    // Normal font weight (not bold)
        lstrcpyW(cfi.FaceName, faceName);              // Copy the font face name (e.g., "Lucida Console")

        // Call the function to set the console font; pass FALSE for the bMaximumWindow parameter
        if (!pSetFont(hConsoleOUT, FALSE, &cfi))
        {
            // If the function fails, output an error message
            std::cerr << "Error changing font\n";
        }
    }

    // Reset console size parameters to original and reapply settings
    void ResetConsoleSize()
    {
        consoleW = originalW;
        consoleH = originalH;
        fontW = originalFW;
        fontH = originalFH;
        ApllySettings();
    }

protected:
    // Configuration parameters
    int consoleW;   // Window width in characters
    int consoleH;   // Window height in characters
    int bufferW;    // Buffer width in characters
    int bufferH;    // Buffer height in characters
    int fontW;      // Font width in pixels
    int fontH;      // Font height in pixels
    int originalW;  // Original window width
    int originalH;  // Original window height
    int originalFW; // Original font width
    int originalFH; // Original font height

    // Input and output handles
    HANDLE hConsoleOUT;
    HANDLE hConsoleIN;

    // Console state info
    SMALL_RECT windowSize;
    RECT rect;
    COORD bufferSize;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD maxSize;
    CONSOLE_CURSOR_INFO cursorInfo;
    HWND hwnd;
};

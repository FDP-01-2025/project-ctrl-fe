#ifndef HUD_BASE_H
#define HUD_BASE_H

// Include headers for used classes
#include "player.h"
#include "utils/functions/utils.h"

// HUD stands for "Heads-Up Display".
// It refers to an interface that shows critical game information
// (like player stats, level info, objectives, etc.) without disrupting gameplay.
// This base class defines the general interface for all HUDs.
class HUDBase
{
public:
    Utils utils; // Instance of the Utils class for common operations

    // Default virtual destructor.
    // This ensures proper cleanup in derived classes.
    virtual ~HUDBase() = default;

    // Returns the width of the HUD.
    // Declared as virtual so derived classes can customize it.
    virtual int GetWidth() const = 0;

    // Renders the HUD in the console.
    // Must be implemented by derived classes.
    virtual void Draw(Player player, int currentLevel, int mapWidth = 0) = 0;

    // Prints a single line of text at a specific position in the HUD.
    // Automatically moves the cursor to the next line (y++).
    void PrintLine(int x, int &y, const std::string &text, const std::string &color = GRAY_BRIGHT) const
    {
        std::cout << "\033[" << y << ";" << x << "H" << color << text << RESET;
        y++;
    }
};

#endif

#ifndef KEYS_H
#define KEYS_H

#include <ncurses.h>

namespace GrandMaze
{
    // User-friendly key names
    enum Keys
    {
        Enter = 10,
        Escape = 27,
        Backspace = KEY_BACKSPACE,
        Tab = 9,
        ShiftTab = 353,
        Up = KEY_UP,
        Down = KEY_DOWN,
        Left = KEY_LEFT,
        Right = KEY_RIGHT,
    };
};

#endif

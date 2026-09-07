#include <ncurses.h>

#include "colour.hpp"

void GrandMaze::initColour(void)
{
    init_pair(Colour::Invert, COLOR_BLACK, COLOR_WHITE);
    init_pair(Colour::Red, COLOR_RED, COLOR_BLACK);
    init_pair(Colour::RedInvert, COLOR_BLACK, COLOR_RED);
    init_pair(Colour::Green, COLOR_GREEN, COLOR_BLACK);
    init_pair(Colour::GreenInvert, COLOR_BLACK, COLOR_GREEN);
    init_pair(Colour::Blue, COLOR_BLUE, COLOR_BLACK);
    init_pair(Colour::BlueInvert, COLOR_BLACK, COLOR_BLUE);
    init_pair(Colour::Yellow, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Colour::YellowInvert, COLOR_BLACK, COLOR_YELLOW);
    init_pair(Colour::Magenta, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(Colour::MagentaInvert, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(Colour::Cyan, COLOR_CYAN, COLOR_BLACK);
    init_pair(Colour::CyanInvert, COLOR_BLACK, COLOR_CYAN);
    init_pair(Colour::Black, COLOR_BLACK, COLOR_BLACK);
}
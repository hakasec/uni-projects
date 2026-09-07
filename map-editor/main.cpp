#include <clocale>

#include <ncurses.h>

#include <libgrandmaze/map.hpp>
#include <libgrandmaze/colour.hpp>
#include <libgrandmaze/room.hpp>

#include "lib/mapview.hpp"

using namespace GrandMaze;

void initialise(void)
{
    initscr();
    raw();
    noecho();

    curs_set(0);
}

void createColour(void)
{
    start_color();
    initColour();
}

int main(int argc, char *argv[])
{
    initialise();
    createColour();

    // set locale
    std::setlocale(LC_ALL, "");

    int maxrow, maxcol;
    getmaxyx(stdscr, maxrow, maxcol);

    Map m;
    MapView mv(stdscr, &m);

    mv.show();

    endwin();
    return 0;
}

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <exception>

#include <sqlite3.h>
#include <ncurses.h>
#include <libgrandmaze/colour.hpp>
#include <libgrandmaze/mapfile.hpp>

#include "main.hpp"

void initialise(void)
{
    // set current working directory
    setCwd();

    // init db
    sqlite3_open((Cwd + "/db/game.db").c_str(), &Db);
    
    // init map
    GrandMaze::DbMapReader r(Db);
    CurrentMap = r.load("maps", "data", 1);
    
    // init ncurses
    initscr();
    cbreak();
    noecho();
    start_color();
    curs_set(0);
    keypad(stdscr, true);

    // init game colours
    GrandMaze::initColour();
}

void setCwd(void)
{
    // set the current working directory
    char *tmp = new char[1024];
    if (getcwd(tmp, 1024) == NULL)
    {
        throw strerror(errno);
    }

    Cwd = tmp;
    delete tmp;
}

int main(int argc, char *argv[])
{
    // initialise global vars and ncurses
    initialise();

    // draw map to stdscr
    CurrentMap->draw(stdscr);

    // pause execution to display map
    getch();
    endwin();

    delete CurrentMap;
    return 0;
}


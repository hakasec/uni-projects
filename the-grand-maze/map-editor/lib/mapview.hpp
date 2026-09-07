#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <string>

#include <ncurses.h>

#include <libgrandmaze/form.hpp>
#include <libgrandmaze/room.hpp>
#include <libgrandmaze/map.hpp>

using namespace GrandMaze;

class MapView
{
private:
    WINDOW *win;
    Map *map;
    int cursorX, cursorY;
    int viewX, viewY;
    Room *roomSelected;

    // Draw view to window
    void drawView(void);
    // Move view to absolute position at x, y
    void moveView(int x, int y);
    // Offset view from current position by x, y
    void offsetView(int x, int y);
    // Get room under the cursors position
    Room* getRoomUnderCursor(void);
    
    // Static form callback for handling input
    static FormOp handleFormInput(int key);

public:
    MapView(WINDOW *win, Map *map);

    // Get the map that the MapView is rendering
    Map* getMap(void);

    // Show the map and check for input from the user
    void show(void);
};

#endif

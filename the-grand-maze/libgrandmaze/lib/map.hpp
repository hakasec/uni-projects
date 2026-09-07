#ifndef MAP_H
#define MAP_H

#include <vector>
#include <initializer_list>
#include <ncurses.h>

#include "room.hpp"

namespace GrandMaze
{
    class Map
    {
    private:
        std::vector<Room*> rooms;

    public:
        // Get a vector of the rooms.
        std::vector<Room*> getRooms(void);
        // Add a room
        void addRoom(Room *room);
        // Add multiple rooms.
        void addRooms(std::initializer_list<Room*> rooms);
        // Delete a given room.
        void delRoom(Room *room);
        // Delete multiple rooms.
        void delRooms(std::initializer_list<Room*> rooms);
        
        // Draw the rooms to a ncurses window.
        void draw(WINDOW *win);
    };
};

#endif
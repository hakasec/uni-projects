#include <vector>
#include <initializer_list>
#include <algorithm>
#include <ncurses.h>

#include "room.hpp"

#include "map.hpp"

using namespace GrandMaze;

std::vector<Room*> Map::getRooms(void)
{
    return rooms;
}

void Map::addRoom(Room *room)
{
    rooms.push_back(room);
}

void Map::addRooms(std::initializer_list<Room*> rooms)
{
    for (auto room : rooms)
    {
        addRoom(room);
    }
}

void Map::delRoom(Room *room)
{
    // find room
    auto elem = std::find(rooms.begin(), rooms.end(), room);
    if (elem == rooms.end())
        return;
    
    // delete room
    rooms.erase(elem);
}

void Map::delRooms(std::initializer_list<Room*> rooms)
{
    for (auto room : rooms)
    {
        delRoom(room);
    }
}

void Map::draw(WINDOW *win)
{
    // draw all rooms in map
    for (Room *room : rooms)
    {
        room->draw(win);
    }
    wrefresh(win);
}

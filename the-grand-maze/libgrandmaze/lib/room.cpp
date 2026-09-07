#include <array>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>

#include <ncurses.h>

#include "colour.hpp"
#include "room.hpp"

using namespace GrandMaze;

Room::Room(RoomType type, int x, int y, Colour colour, uint8_t attrs)
    : type(type), x(x), y(y), colour(colour), attrs(attrs) 
{
    updateTemplate();
    doorOffsets = {0, 0, 0, 0};
}

Room::Room(RoomType type, int x, int y, Colour colour)
    : Room(type, x, y, colour, 0)
{

}

Room::Room(RoomType type, int x, int y)
    : Room(type, x, y, Colour::None, 0)
{

}

void Room::updateTemplate(void)
{
    // assign room template given the room type
    if (type == RoomType::Square)
    {
        roomTemplate = &_SQUARE_ROOM;
    }
    else if (type == RoomType::Vertical)
    {
        roomTemplate = &_VERTICAL_ROOM;
    }
    else if (type == RoomType::Horizontal)
    {
        roomTemplate = &_HORIZONTAL_ROOM;
    }
}

RoomType Room::getType(void)
{
    return type;
}

void Room::setType(RoomType type)
{
    // sets type and updates template
    this->type = type;
    updateTemplate();
}

int Room::getX(void)
{
    return x;
}

void Room::setX(int x)
{
    this->x = x;
}

int Room::getY(void)
{
    return y;
}

void Room::setY(int y)
{
    this->y = y;
}

int Room::getWidth(void)
{
    return (*roomTemplate)[0].length();
}

int Room::getHeight(void)
{
    return roomTemplate->size();
}

Colour Room::getColour(void)
{
    return colour;
}

void Room::setColour(Colour colour)
{
    this->colour = colour;
}

uint8_t Room::getAttributes(void)
{
    return attrs;
}

void Room::setAttributes(uint8_t attrs)
{
    this->attrs = attrs;
}

int Room::getDoorOffset(RoomAttribute door)
{
    if (door == RoomAttribute::NorthDoor)
    {
        return doorOffsets[0];
    }
    else if (door == RoomAttribute::EastDoor)
    {
        return doorOffsets[1];
    }
    else if (door == RoomAttribute::SouthDoor)
    {
        return doorOffsets[2];
    }
    else if (door == RoomAttribute::WestDoor)
    {
        return doorOffsets[3];
    }
    else
    {
        throw std::invalid_argument(
            "door must be a valid door"
            " (NorthDoor, EastDoor, SouthDoor, WestDoor)."
        );
    }
}

void Room::setDoorOffset(RoomAttribute door, int offset)
{
    int width, height;
    width = getWidth(), height = getHeight();

    std::invalid_argument ex("offset is too large.");

    // check door type, check for errors, and set offset
    if (door == RoomAttribute::NorthDoor)
    {
        if (abs(offset) > (width-2)/2)
            throw ex;
        doorOffsets[0] = offset;
    }
    else if (door == RoomAttribute::EastDoor)
    {
        if (abs(offset) > (height-2)/2)
            throw ex;
        doorOffsets[1] = offset;
    }
    else if (door == RoomAttribute::SouthDoor)
    {
        if (abs(offset) > (width-2)/2)
            throw ex;
        doorOffsets[2] = offset;
    }
    else if (door == RoomAttribute::WestDoor)
    {
        if (abs(offset) > (height-2)/2)
            throw ex;
        doorOffsets[3] = offset;
    }
}

void Room::drawDoors(WINDOW *win)
{
    int offset;
    int width = getWidth();
    int length = getHeight();
    // detect doors, find the midpoint + offset and add a space
    if ((attrs & RoomAttribute::NorthDoor) == RoomAttribute::NorthDoor)
    {
        offset = doorOffsets[0];
        mvwaddch(win, y, (x+(width/2)) + offset, ' ');
    }
    if ((attrs & RoomAttribute::EastDoor) == RoomAttribute::EastDoor)
    {
        offset = doorOffsets[1];
        mvwaddch(win, (y+(length/2)) + offset, x+width-1, ' ');
    }
    if ((attrs & RoomAttribute::SouthDoor) == RoomAttribute::SouthDoor)
    {
        offset = doorOffsets[2];
        mvwaddch(win, (y+length-1), x+(width/2) + offset, ' ');
    }
    if ((attrs & RoomAttribute::WestDoor) == RoomAttribute::WestDoor)
    {
        offset = doorOffsets[3];
        mvwaddch(win, y+(length/2) + offset, x, ' ');
    }
}

void Room::draw(WINDOW *win)
{
    // iterate through the roomTemplate
    for (auto it = roomTemplate->begin(); it != roomTemplate->end(); it++)
    {
        int ydistance = it - roomTemplate->begin();
        // draw differently if colour is applied
        // for some reason, space characters stop working
        if (colour >= 0)
        {
            for (auto jt = (*it).begin(); jt != (*it).end(); jt++)
            {
                int xdistance = jt - (*it).begin(); 
                // if the character in the string isn't a space, enable colour
                // else, turn off colour
                if (*jt != ' ')
                    wattron(win, COLOR_PAIR(colour));
                else
                    wattroff(win, COLOR_PAIR(colour));

                mvwaddch(win, y+ydistance, x+xdistance, *jt);
            }
            wattroff(win, COLOR_PAIR(colour));
        }
        else
        {
            mvwaddstr(win, y+ydistance, x, (*it).c_str());
        }
    }
    drawDoors(win);
}

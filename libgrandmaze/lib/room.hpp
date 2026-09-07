#ifndef ROOM_H
#define ROOM_H

#include <array>
#include <vector>
#include <string>

#include <ncurses.h>

#include "colour.hpp"

namespace GrandMaze
{
    const std::vector<std::string> _SQUARE_ROOM = {
        "#########",
        "#       #",
        "#       #",
        "#       #",
        "#########"
    };

    const std::vector<std::string> _VERTICAL_ROOM = {
        "#########",
        "#       #",
        "#       #",
        "#       #",
        "#       #",
        "#       #",
        "#       #",
        "#       #",
        "#########"
    };

    const std::vector<std::string> _HORIZONTAL_ROOM = {
        "#######################",
        "#                     #",
        "#                     #",
        "#                     #",
        "#######################"
    };

    // Bitflags of the room attributes
    enum RoomAttribute
    {
        NorthDoor = 1u,
        EastDoor  = 2u,
        SouthDoor = 4u,
        WestDoor  = 8u,
        Secret    = 16u,
    };

    enum RoomType
    {
        Square = 1,
        Vertical,
        Horizontal,
    };

    class Room
    {
    private:
        RoomType type;
        int x, y;
        Colour colour;
        uint8_t attrs;
        std::array<int, 4> doorOffsets;
        const std::vector<std::string> *roomTemplate;

        // Update the internal pointer roomTemplate to the template
        void updateTemplate(void);
        // Draw the doors
        void drawDoors(WINDOW *win);

    public:
        Room(RoomType type, int x, int y, Colour colour, uint8_t attrs);
        Room(RoomType type, int x, int y, Colour colour);
        Room(RoomType type, int x, int y);

        // Get room type
        RoomType getType(void);
        // Set room type
        void setType(RoomType type);
        
        // Get X position
        int getX(void);
        // Get Y position
        int getY(void);
        // Set X position
        void setX(int x);
        // Set Y position
        void setY(int y);
        
        // Get room width
        int getWidth(void);
        // Get room height
        int getHeight(void);

        // Get room colour
        Colour getColour(void);
        // Set room colour
        void setColour(Colour colour);

        // Get room attributes
        uint8_t getAttributes(void);
        // Set room atrributes
        void setAttributes(uint8_t attrs);

        // Get door offset for a given door
        int getDoorOffset(RoomAttribute door);
        // Set door offset for a given door
        void setDoorOffset(RoomAttribute door, int offset);

        // Draw the room to the window
        void draw(WINDOW *win);
    };
};

#endif

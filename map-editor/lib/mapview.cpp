#include <vector>
#include <cstdio>
#include <string>
#include <functional>

#include <ncurses.h>

#include <libgrandmaze/map.hpp>
#include <libgrandmaze/colour.hpp>
#include <libgrandmaze/keys.hpp>
#include <libgrandmaze/form.hpp>
#include <libgrandmaze/mapfile.hpp>

#include "mapview.hpp"

using namespace GrandMaze;

MapView::MapView(WINDOW *win,Map *map)
    : win(win), map(map)
{
    cursorX = 0;
    cursorY = 0;
    viewX = 0;
    viewY = 0;
    roomSelected = nullptr;
}

Map* MapView::getMap(void)
{
    return map;
}

void MapView::drawView(void)
{
    int maxrow, maxcol;
    getmaxyx(win, maxrow, maxcol);

    wclear(win);
    map->draw(win);

    // draw cursor and offset stats at the bottom of the screen
    char offsetStat[32];
    char cursorStat[32];
    int offN = sprintf(offsetStat, "Offset: %d, %d", viewX, viewY);
    int curN = sprintf(cursorStat, "Cursor: %d, %d", 
                       viewX + cursorX, viewY + cursorY);

    int n = offN > curN ? offN : curN;

    mvwaddch(win, cursorY, cursorX, 'X' | COLOR_PAIR(Colour::Yellow));
    // check for cursor collision
    if (cursorX >= maxcol-n && cursorY >= maxrow-2)
    {
        // draw on left side
        mvwaddnstr(win, maxrow-2, 0, offsetStat, offN);
        mvwaddnstr(win, maxrow-1, 0, cursorStat, curN);
    }
    else
    {
        // draw on right side
        mvwaddnstr(win, maxrow-2, maxcol-offN, offsetStat, offN);
        mvwaddnstr(win, maxrow-1, maxcol-curN, cursorStat, curN);
    }
    
    wrefresh(win);
}

void MapView::moveView(int x, int y)
{
    // find the deltas for current position to absolute position
    int dx = x - viewX, dy = y - viewY;
    offsetView(dx, dy);
}

void MapView::offsetView(int x, int y)
{
    // increment the view position
    viewX += x;
    viewY += y;

    for (auto room : map->getRooms())
    {
        // move the rooms in the opposite direction
        int roomX = room->getX(), roomY = room->getY();
        room->setX(roomX - x);
        room->setY(roomY - y);
    }
}

Room* MapView::getRoomUnderCursor(void)
{
    int maxrow, maxcol;
    getmaxyx(win, maxrow, maxcol);

    for (auto room : map->getRooms())
    {
        // get room details
        int roomX = room->getX(), roomY = room->getY();
        int roomH = room->getHeight(), roomW = room->getWidth();
        // if room is onscreen
        if (roomX <= maxcol && roomY <= maxrow)
        {
            // if the room encompasses the cursor
            if (cursorX >= roomX && cursorX < roomX+roomW 
                && cursorY >= roomY && cursorY < roomY+roomH)
            {
                // return it
                return room;
            }
        }
    }
    return nullptr;
}

FormOp MapView::handleFormInput(int key)
{
    // handle form input
    if (key == Keys::Enter)
        return FormOp::SubmitForm;
    else if (key == Keys::Backspace)
        return FormOp::Pop;
    else if (key == Keys::Escape)
        return FormOp::CancelForm;
    else if (key == Keys::Tab)
        return FormOp::NextField;
    else if (key == Keys::ShiftTab)
        return FormOp::PrevField;
    else
        return FormOp::Push;
}

void MapView::show(void)
{
    int maxrow, maxcol;
    // store last loaded filename
    std::string lastFilename = "";

    drawView();
    keypad(win, true);
    set_escdelay(1);
    do
    {
        getmaxyx(win, maxrow, maxcol);
        // get user input
        int c = wgetch(win);
        bool redraw = false;
        if (c == Keys::Escape)
        {
            // break on escape
            break;
        }
        else if (c == Keys::Up)
        {
            if (cursorY > 0)
            {
                cursorY--;
                redraw = true;
            }
            else if (viewY > 0)
            {
                offsetView(0, -1);
                redraw = true;
            }
            else
            {
                beep();
            }
        }
        else if (c == Keys::Down)
        {
            if (cursorY < maxrow-1)
            {
                cursorY++;
            }
            else
            {
                offsetView(0, 1);
            }
            redraw = true;
        }
        else if (c == Keys::Left)
        {
            if (cursorX > 0)
            {
                cursorX--;
                redraw = true;
            }
            else if (viewX > 0)
            {
                offsetView(-1, 0);
                redraw = true;
            }
            else
            {
                beep();
            }
        }
        else if (c == Keys::Right)
        {
            if (cursorX < maxcol-1)
            {
                cursorX++;
            }
            else
            {
                offsetView(1, 0);
            }
            redraw = true;
        }
        else if (c == Keys::Enter)
        {
            // if no room selected
            if (roomSelected == nullptr)
            {
                // find room under cursor
                auto room = getRoomUnderCursor();
                if (room != nullptr)
                {
                    // check is room origin is off screen and offset
                    int roomX = room->getX(), roomY = room->getY();
                    if (roomX < 0 && roomY < 0)
                    {
                        offsetView(roomX, roomY);
                    }
                    else if (roomX < 0)
                    {
                        offsetView(roomX, 0);
                    }
                    else if (roomY < 0)
                    {
                        offsetView(0, roomY);
                    }
                    // set cursor to room origin
                    cursorX = room->getX();
                    cursorY = room->getY();

                    roomSelected = room;

                    // change room colour to signify selection
                    roomSelected->setColour(
                        (Colour)(roomSelected->getColour()+1)
                    );
                }
            }
            else
            {
                // change room colour back to original
                roomSelected->setColour(
                    (Colour)(roomSelected->getColour()-1)
                );
                roomSelected = nullptr;
            }
            redraw = true;
        }
        // if room selected, enter room editting mode
        else if (roomSelected != nullptr)
        {

            bool deleted = false;

            uint8_t attrs = roomSelected->getAttributes();
            uint8_t newAttr = 0;

            Colour roomColour = roomSelected->getColour();
            RoomType roomType = roomSelected->getType();
            int roomWidth = roomSelected->getWidth();
            int roomHeight = roomSelected->getHeight();
            int halfWidth = (roomWidth-2)/2;
            int halfHeight = (roomHeight-2)/2;

            // check room-specific input

            // handle door toggle commands
            if (c == 'n')
                newAttr = RoomAttribute::NorthDoor;
            else if (c == 'e')
                newAttr = RoomAttribute::EastDoor;
            else if (c == 's')
                newAttr = RoomAttribute::SouthDoor;
            else if (c == 'w')
                newAttr = RoomAttribute::WestDoor;
            // handle room colour commands
            else if (c == '.')
                roomColour = (Colour)((int)roomColour + 2);
            else if (c == ',')
                roomColour = (Colour)((int)roomColour - 2);
            // handle room type commands
            else if (c == '>')
                roomType = (RoomType)((int)roomType + 1);
            else if (c == '<')
                roomType = (RoomType)((int)roomType - 1);
            // handle offset commands
            else if (c == 'N' || c == 'S')
            {
                RoomAttribute door;
                if (c == 'N') 
                    door = RoomAttribute::NorthDoor;
                else
                    door = RoomAttribute::SouthDoor;

                int offset = roomSelected->getDoorOffset(door) + 1;
                if (halfWidth + offset >= roomWidth-2)
                {
                    offset = -halfWidth;
                }

                roomSelected->setDoorOffset(door, offset);
                redraw = true;
            }
            else if (c == 'W' || c == 'E')
            {
                RoomAttribute door;
                if (c == 'W') 
                    door = RoomAttribute::WestDoor;
                else
                    door = RoomAttribute::EastDoor;

                int offset = roomSelected->getDoorOffset(door) + 1;
                
                if (halfHeight + offset >= roomHeight-2)
                {
                    offset = -halfHeight;
                }

                roomSelected->setDoorOffset(door, offset);
                redraw = true;
            }
            // handle deletion
            else if (c == Keys::Backspace)
            {
                deleted = true;
            }

            // check for new attributes
            if (newAttr)
            {
                // if the newAttr already exists
                if ((attrs & newAttr) == newAttr)
                {
                    // remove it
                    attrs ^= newAttr;
                }
                else
                {
                    // otherwise add it
                    attrs |= newAttr;
                }
                
                roomSelected->setAttributes(attrs);
                redraw = true;
            }

            // check for colour change
            if (roomColour != roomSelected->getColour())
            {
                roomSelected->setColour(roomColour);
                redraw = true;
            }

            // check for room type change
            if (roomType != roomSelected->getType())
            {
                roomSelected->setType(roomType);
                redraw = true;
            }

            // check for deletion
            if (deleted)
            {
                map->delRoom(roomSelected);
                roomSelected = nullptr;
                redraw = true;
            }
        }
        // Save routine
        else if (c == 'S')
        {
            int tmpX = viewX, tmpY = viewY;
            // reset room X,Y coords before save
            moveView(0, 0);

            WINDOW *prompt = newwin(7, maxcol/2, (maxrow/2)-3, maxcol/4);
            keypad(prompt, true);

            Field filename(1, 0, 1, (maxcol/2)-4, "Filename:", true, 
                           lastFilename);
            Form f(prompt, "Save to File");
            f.addField(&filename);
            f.show(handleFormInput);

            delwin(prompt);

            // save
            FileMapWriter mw(map);
            if (!filename.getValue().empty())
            {
                lastFilename = filename.getValue();
                mw.write(lastFilename);
            }

            // move back
            moveView(tmpX, tmpY);

            redraw = true;
        }
        // Load routine
        else if (c == 'L')
        {
            WINDOW *prompt = newwin(7, maxcol/2, (maxrow/2)-3, maxcol/4);
            keypad(prompt, true);

            Field filename(1, 0, 1, (maxcol/2)-4, "Filename:", true,
                           lastFilename);
            Form f(prompt, "Load from File");
            f.addField(&filename);
            f.show(handleFormInput);

            delwin(prompt);

            // load
            FileMapReader mr;
            if (!filename.getValue().empty())
            {
                lastFilename = filename.getValue();
                try 
                {
                    map = mr.load(lastFilename);
                }
                catch (std::invalid_argument &ie)
                {
                    beep();
                    drawView();
                    std::string message = "Error: ";
                    message.append(ie.what());
                    mvwaddstr(
                        win, 
                        maxrow / 2, 
                        (maxcol / 2) - (message.size() / 2),
                        message.c_str()
                    );
                    wgetch(win);
                }
                
                moveView(0, 0);
                cursorX = 0;
                cursorY = 0;
            }

            redraw = true;
        }
        // add new room
        else if (c == 'N')
        {
            Room *newRoom = new Room(RoomType::Square, 
                                     cursorX, 
                                     cursorY, 
                                     Colour::Invert);
            map->addRoom(newRoom);
            roomSelected = newRoom;
            redraw = true;
        }

        // check for redraw
        if (redraw)
        {
            // if room selected
            if (roomSelected != nullptr)
            {
                // move room to cursor pos
                roomSelected->setX(cursorX);
                roomSelected->setY(cursorY);
            }
            drawView();
        }
    }
    while (true);
}

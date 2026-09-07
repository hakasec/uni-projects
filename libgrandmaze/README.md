# libgrandmaze

This is the core library for [the-grand-maze][game-repo].

## Installation

**In order to install this library, the ncurses development library is needed.**

```bash
# clone and change dir
git clone https://github.coventry.ac.uk/soperd/libgrandmaze.git
cd libgrandmaze

# generate makefile and install
cmake .
make
sudo make install
```

## Usage

This library contains a couple classes:

### Room

A room to be drawn to a given window.

```cpp
// Colour of the room
enum Colour
{
    None,
    Invert,
    Red,
    RedInvert,
    Blue,
    BlueInvert,
    Yellow,
    YellowInvert,
};


// Bitflags used to set attrs for Room.
enum RoomAttribute
{
    NorthDoor = 1u,
    EastDoor  = 2u,
    SouthDoor = 4u,
    WestDoor  = 8u,
    Secret    = 16u,
};

// Type/shape of the room
enum RoomType
{
    Square = 1,
    Vertical,
    Horizontal,
};

// Constructors
Room(RoomType type, int x, int y, Colour colour, uint8_t attrs);
Room(RoomType type, int x, int y, Colour colour);
Room(RoomType type, int x, int y);

// getters and setters
RoomType getType(void);
void setType(RoomType type);

int getX(void);
int getY(void);

void setX(int x);
void setY(int y);

int getWidth(void);
int getHeight(void);

Colour getColour(void);
void setColour(Colour colour);

uint8_t getAttributes(void);
void setAttributes(uint8_t attrs);

int getDoorOffset(RoomAttribute door);
void setDoorOffset(RoomAttribute door, int offset);

// Draws the room to a given ncurses window.
void draw(WINDOW *win);
```

For example, you can construct a room like so:

```cpp
uint8_t doors = RoomAttribute::NorthDoor | RoomAttribute::SouthDoor
Room r(RoomType::Square, 0, 0, Colour::Blue, doors);
```

This is create a blue square room at X: 0, Y: 0 with a north and south doors.

### Map

Contains a set of rooms to be drawn to a given window.

Functions:

```cpp
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
```

### Form

A form that contains user-defined data fields and keybindings.

### MapReader

A reader for loading in map files.

### MapWriter

A writer for creating map files.

[game-repo]: https://github.coventry.ac.uk/soperd/the-grand-maze
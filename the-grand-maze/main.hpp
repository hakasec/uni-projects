#ifndef MAIN_H
#define MAIN_H

#include <string>

#include <sqlite3.h>
#include <libgrandmaze/map.hpp>

sqlite3 *Db;
GrandMaze::Map *CurrentMap;
std::string Cwd;

// initialise ncurses and database
void initialise(void);
// set the current working directory
void setCwd(void);

#endif

#ifndef MAPFILE_H
#define MAPFILE_H

#include <fstream>
#include <string>

#include <sqlite3.h>

#include "map.hpp"
#include "room.hpp"

namespace GrandMaze
{
    const std::string MAGIC_NUMBER_v1 = "SGMAP1";
    const std::string MAGIC_NUMBER_v2 = "SGMAP2";
    
    class MapWriter
    {
    protected:
        Map *map;
        
        // Write the room to the output stream out
        void virtual writeRoom(std::ostream *out, Room *room);

        // Get the output size of the map
        size_t getSize(void);

    public:
        MapWriter(Map *map);

        // Write the map to a given output stream
        void virtual write(std::ostream *stream);
    };

    class MapReader
    {
    protected:
        // parse multiple rooms from stream into the map m (version 1)
        void virtual parseRooms(std::istream *stream, Map *m);
        // parse a single room from stream and return it (version 1)
        virtual Room* parseRoom(std::istream *stream);

        // parse multiple rooms from stream into the map m (version 1)
        void virtual parseRooms_v2(std::istream *stream, Map *m);
        // parse a single room from stream and return it (version 2)
        virtual Room* parseRoom_v2(std::istream *stream);

    public:
        // load a map from an input stream
        virtual Map* load(std::istream *stream);
    };

    class FileMapWriter : public MapWriter
    {
    public:
        FileMapWriter(Map *m);

        // write to a file at location specified by filename
        void write(std::string filename);
    };

    class FileMapReader : public MapReader
    {
    public:
        // load from a file at location specified by filename
        Map* load(std::string filename);
    };

    class DbMapReader : public MapReader
    {
    protected:
        sqlite3 *db;

    public:
        DbMapReader(sqlite3 *db);

        // load from the db from the given table, column, and rowID
        Map* load(std::string table, std::string column, int rowID);
    };

    class DbMapWriter : public MapWriter
    {
    protected:
        sqlite3 *db;

        // checks if input is a safe and valid SQL string
        bool isValidSQLInput(std::string input);

    public:
        DbMapWriter(Map* map, sqlite3 *db);

        // insert a new map at the given table and column
        void write(std::string table, std::string column);

        // update a map at the given table, column, and rowID
        void write(std::string table, std::string column, long rowid);
    };
};

#endif

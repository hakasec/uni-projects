#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

#include "map.hpp"
#include "mapfile.hpp"
#include "colour.hpp"
#include "exception.hpp"

using namespace GrandMaze;
// ad hoc; copied from and modified:
// https://stackoverflow.com/questions/7781898/get-an-istream-from-a-char
struct membuf : std::streambuf
{
    membuf(char* buf, size_t size) 
    {
        this->setg(buf, buf, buf+size);
    }
};

MapWriter::MapWriter(Map *map) : map(map)
{

}

size_t MapWriter::getSize(void)
{
    return (
        map->getRooms().size() * 9
        + MAGIC_NUMBER_v2.size()
    );
}

void MapWriter::writeRoom(std::ostream *out, Room *room)
{
    *out << (uint8_t)room->getType();
    *out << (uint8_t)room->getColour();
    *out << (uint8_t)room->getX();
    *out << (uint8_t)room->getY();
    *out << (uint8_t)room->getAttributes();
    *out << (int8_t)room->getDoorOffset(RoomAttribute::NorthDoor);
    *out << (int8_t)room->getDoorOffset(RoomAttribute::EastDoor);
    *out << (int8_t)room->getDoorOffset(RoomAttribute::SouthDoor);
    *out << (int8_t)room->getDoorOffset(RoomAttribute::WestDoor);
}

void MapWriter::write(std::ostream *stream)
{
    // write magic number
    *stream << MAGIC_NUMBER_v2;
    for (auto room : map->getRooms())
    {
        writeRoom(stream, room);
    }
}

Room* MapReader::parseRoom(std::istream *stream)
{
    char buffer[5] = {'\0'};
    stream->read(buffer, 5);

    if (stream->eof())
        return nullptr;

    return new Room(
        (RoomType)buffer[0],
        (int)buffer[2],
        (int)buffer[3],
        (Colour)buffer[1], 
        (uint8_t)buffer[4]
    );
}

void MapReader::parseRooms(std::istream *stream, Map *m)
{
    while (stream->good())
    {
        Room *r = parseRoom(stream);
        if (r != nullptr)
            m->addRoom(r);
    }
}

void MapReader::parseRooms_v2(std::istream *stream, Map *m)
{
    while (stream->good())
    {
        Room *r = parseRoom_v2(stream);
        if (r != nullptr)
            m->addRoom(r);
    }
}

Room* MapReader::parseRoom_v2(std::istream *stream)
{
    char buffer[9] = {'\0'};
    stream->read(buffer, 9);

    if (stream->eof())
        return nullptr;

    Room *r = new Room(
        (RoomType)buffer[0],
        (int)buffer[2],
        (int)buffer[3],
        (Colour)buffer[1], 
        (uint8_t)buffer[4]
    );
    r->setDoorOffset(RoomAttribute::NorthDoor,  (int)buffer[5]);
    r->setDoorOffset(RoomAttribute::EastDoor,   (int)buffer[6]);
    r->setDoorOffset(RoomAttribute::SouthDoor,  (int)buffer[7]);
    r->setDoorOffset(RoomAttribute::WestDoor,   (int)buffer[8]);

    return r;
}

Map* MapReader::load(std::istream *stream)
{
    int magicSize = MAGIC_NUMBER_v1.size();
    char *buffer = new char[magicSize+1];
    buffer[magicSize] = '\0';
    
    Map *m = new Map();

    stream->read(buffer, magicSize);
    if (buffer == MAGIC_NUMBER_v1)
    {
        parseRooms(stream, m);
    }
    else if (buffer == MAGIC_NUMBER_v2)
    {
        parseRooms_v2(stream, m);
    }
    else
    {
        delete buffer;
        throw std::invalid_argument("File must be a valid map file.");
    }

    delete buffer;

    return m;
}

Map* FileMapReader::load(std::string filename)
{
    std::ifstream *ifs = new std::ifstream(
        filename,
        std::ifstream::in |
        std::ifstream::binary
    );

    Map *m;
    try
    {
        m = MapReader::load(ifs);    
    }
    catch (std::invalid_argument &e)
    {
        // tidy
        ifs->close();
        delete ifs;

        // rethrow
        throw e;
    }

    ifs->close();
    delete ifs;

    return m;
}

FileMapWriter::FileMapWriter(Map *m) : MapWriter(m)
{

}

void FileMapWriter::write(std::string filename)
{
    std::ofstream *out = new std::ofstream(
        filename, 
        std::ofstream::out | 
        std::ofstream::trunc | 
        std::ofstream::binary
    );

    // use generic ostream write function
    MapWriter::write(out);

    out->close();
    delete out;
}

DbMapReader::DbMapReader(sqlite3 *db) : db(db)
{

}

Map* DbMapReader::load(std::string table, std::string column, int rowID)
{
    int err, size;
    char *buffer;
    sqlite3_blob *blob;

    // open blob
    err = sqlite3_blob_open(
        db, 
        "main", 
        table.c_str(), 
        column.c_str(), 
        rowID, 
        0, 
        &blob
    );
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // get size of blob
    size = sqlite3_blob_bytes(blob);
    buffer = new char[size];
    // read blob into buffer
    err = sqlite3_blob_read(blob, buffer, size, 0);
    if (err != SQLITE_OK)
    {
        delete buffer;
        throw DatabaseError(err);
    }

    Map *m;
    // store buffer in membuf for use with istream
    auto cbuf = new membuf(buffer, size);
    auto is = new std::istream(cbuf);
    // load map using istream
    m = MapReader::load(is);

    // free up memory
    delete buffer;
    delete cbuf;
    delete is;

    // close blob
    err = sqlite3_blob_close(blob);
    if (err != SQLITE_OK)
    {
        delete m;
        throw DatabaseError(err);
    }

    return m;
}

DbMapWriter::DbMapWriter(Map *map, sqlite3 *db) : MapWriter(map), db(db)
{

}

bool DbMapWriter::isValidSQLInput(std::string input)
{
    // help prevent sql injection
    const std::string valid = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "-_";
    for (char c : input)
    {
        // if character is not in valid character
        if (valid.find(c) == valid.npos)
            return false;
    }
    return true;
}

void DbMapWriter::write(std::string table, std::string column)
{
    if (!isValidSQLInput(table))
        throw std::invalid_argument("table isn't a valid SQL-safe string");
    else if (!isValidSQLInput(column))
        throw std::invalid_argument("column isn't a valid SQL-safe string");

    int err, rowid;
    size_t size;
    char *buffer;
    sqlite3_stmt *stmt;
    std::string sql =
        "INSERT INTO `" + table + "`"
        "   (`" + column + "`)" 
        "VALUES (?);";

    // prep statement
    err = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, NULL);
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // write to buffer step
    size = MapWriter::getSize();
    buffer = new char[size];
    auto cbuf = new membuf(buffer, size);
    auto os = new std::ostream(cbuf);
    // write via ostream into buffer
    MapWriter::write(os);

    for (int i = 0; i < size; i++)
    {
        std::cout << (int)buffer[i] << std::endl;
    }

    // bind the blob
    err = sqlite3_bind_blob(stmt, 1, buffer, size, SQLITE_STATIC);
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // advance the statement
    err = sqlite3_step(stmt);

    // clean up
    delete buffer;
    delete cbuf;
    delete os;

    // check for error
    if (err != SQLITE_DONE)
        throw DatabaseError(err);

    // destroy statement
    err = sqlite3_finalize(stmt);
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // get last rowid
    rowid = sqlite3_last_insert_rowid(db);

    // write using update method
    // write(table, column, rowid);
}

void DbMapWriter::write(std::string table, std::string column, long rowid)
{
    int err;
    size_t size;
    char *buffer;
    sqlite3_blob *blob;

    // open the blob
    err = sqlite3_blob_open(
        db,
        "main",
        table.c_str(),
        column.c_str(),
        rowid,
        0,
        &blob
    );
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // get the size in bytes of the map file being produced
    size = MapWriter::getSize();
    buffer = new char[size];
    auto cbuf = new membuf(buffer, size);
    auto os = new std::ostream(cbuf);
    // write via ostream into buffer
    MapWriter::write(os);

    // write buffer to blob
    err = sqlite3_blob_write(blob, buffer, size, 0);

    // clean up
    delete buffer;
    delete cbuf;
    delete os;
    
    // delayed error check after clean up
    if (err != SQLITE_OK)
        throw DatabaseError(err);

    // close blob
    err = sqlite3_blob_close(blob);
    if (err != SQLITE_OK)
        throw DatabaseError(err);
}

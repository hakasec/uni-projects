#include <sqlite3.h>

#include "exception.hpp"

using namespace GrandMaze;

DatabaseError::DatabaseError(int errorCode) : errorCode(errorCode)
{

}

const char* DatabaseError::what() const noexcept
{
    // get sqlite3 error from error code
    return sqlite3_errstr(errorCode);
}
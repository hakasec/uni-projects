#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace GrandMaze
{
    class DatabaseError : public std::exception 
    {
    private:
        int errorCode;

    public:
        // Construct a DatabaseError with a SQLite error code
        DatabaseError(int errorCode); 

        const char* what() const noexcept;
    };
}

#endif

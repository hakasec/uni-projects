cmake_minimum_required(VERSION 2.8)
project(grandmaze-game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# get all cpp and hpp files
file(GLOB source main.cpp main.hpp "*.cpp" "*.hpp")

add_executable(grandmaze-game ${source})

target_link_libraries(grandmaze-game ncurses sqlite3 grandmaze)

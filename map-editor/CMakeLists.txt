cmake_minimum_required(VERSION 2.8)
project(maped)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

file(GLOB lib "lib/*.cpp" "lib/*.hpp")
set(source main.cpp main.hpp ${lib})

add_executable(maped ${source})
target_link_libraries(maped ncurses)
target_link_libraries(maped grandmaze)

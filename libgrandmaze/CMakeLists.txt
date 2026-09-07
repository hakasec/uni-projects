include(GenerateExportHeader)

cmake_minimum_required(VERSION 2.8)
project(grandmaze)

set(PROJECT_BINARY_DIR "bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_INSTALL_PREFIX "/usr")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

file(GLOB headers "lib/*.hpp")
file(GLOB sources "lib/*.cpp")

add_library(grandmaze SHARED ${sources} ${headers})
target_link_libraries(grandmaze ncurses sqlite3)

install(TARGETS grandmaze
        LIBRARY DESTINATION "lib")

install(FILES
        ${headers}
        DESTINATION "include/libgrandmaze")

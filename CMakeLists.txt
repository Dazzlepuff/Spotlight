cmake_minimum_required(VERSION 3.15)
project(BoardGamePrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES src/*.cpp)

add_executable(boardgame ${SOURCES})

set_target_properties(boardgame PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(boardgame PROPERTIES LINK_SEARCH_END_STATIC ON)
target_link_options(boardgame PRIVATE -static -static-libgcc -static-libstdc++)

target_include_directories(boardgame PRIVATE ${CMAKE_SOURCE_DIR}/include)
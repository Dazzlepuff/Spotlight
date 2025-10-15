cmake_minimum_required(VERSION 3.15)
project(BoardGamePrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES src/*.cpp)

add_executable(boardgame ${SOURCES})

target_include_directories(boardgame PRIVATE ${CMAKE_SOURCE_DIR}/include)
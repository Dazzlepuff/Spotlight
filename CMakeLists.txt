cmake_minimum_required(VERSION 3.15)
project(BoardGamePrototype LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# ======================================================
# Fetch SFML (statically linked)
# ======================================================
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(sfml)

# ======================================================
# Your Sources
# ======================================================
file(GLOB SOURCES src/*.cpp)

add_executable(boardgame ${SOURCES})

target_include_directories(boardgame PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(boardgame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# ======================================================
# Static Linking for Runtime Libraries (GCC/MinGW)
# ======================================================
if(MINGW)
    target_link_options(boardgame PRIVATE -static -static-libgcc -static-libstdc++)
endif()

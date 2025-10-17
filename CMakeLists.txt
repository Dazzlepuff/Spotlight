cmake_minimum_required(VERSION 3.15)
project(BoardGamePrototype LANGUAGES CXX)

# ======================================================
# Compiler and Build Settings
# ======================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======================================================
# Options for Flexibility
# ======================================================
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_STATIC_RUNTIME "Statically link libstdc++ and libgcc" ON)

# ======================================================
# Fetch SFML
# ======================================================
include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(sfml)

# ======================================================
# Your Sources
# ======================================================
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(boardgame ${SOURCES})
target_include_directories(boardgame PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(boardgame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# ======================================================
# Platform-Specific Settings
# ======================================================

if(MSVC)
    # Hide console window when using Windows GUI subsystem
    set_target_properties(boardgame PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
elseif(MINGW)
    # Static runtime linking if enabled
    if(BUILD_STATIC_RUNTIME)
        target_link_options(boardgame PRIVATE -static -static-libgcc -static-libstdc++)
    endif()
elseif(UNIX AND NOT APPLE)
    # Linux: link pthread if needed
    target_link_libraries(boardgame PRIVATE pthread)
endif()

# ======================================================
# Installation (optional)
# ======================================================
install(TARGETS boardgame RUNTIME DESTINATION bin)

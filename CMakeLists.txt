cmake_minimum_required(VERSION 3.15)
project(Spotlight LANGUAGES CXX)

# ======================================================
# Compiler and Build Settings
# ======================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======================================================
# Output Directories
# ======================================================
# Place compiled executables in /bin and libraries in /lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Ensure separate directories for Debug/Release if needed
foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
endforeach()

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
add_executable(Spotlight ${SOURCES})
target_include_directories(Spotlight PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Spotlight PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# ======================================================
# Platform-Specific Settings
# ======================================================
if(MSVC)
    set_target_properties(Spotlight PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
elseif(MINGW)
    if(BUILD_STATIC_RUNTIME)
        target_link_options(Spotlight PRIVATE -static -static-libgcc -static-libstdc++)
    endif()
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Spotlight PRIVATE pthread)
endif()
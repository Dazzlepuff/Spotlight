# Spotlight

A modular, object-oriented digital board game framework built with C++ and SFML. This project demonstrates structured game state management, I/O command handling, and support for complex card-based board game logic on a hexagonal tile grid.

---

## Features

- **Interactive Startup Menu** – Configure players and companies before gameplay
- **Real-Time Game Management** – Dynamic tile, resource, and company ownership tracking
- **Command-Based Control System** – Execute gameplay actions through a robust console interface
- **Resource and Card Management** – Track and manipulate player resources and card decks
- **Complete Documentation** – Includes Doxygen-generated API reference and comprehensive user manual

---

## Getting Started

### Prerequisites

- **MSYS2 / MinGW (UCRT64)** – Compiler and shell environment
- **CMake** (≥ 3.15) – Build configuration
- **Ninja** – Build system
- **SFML 2.6.x** – Automatically fetched by CMake

### Installation

Clone the repository:

```bash
git clone https://github.com/Dazzlepuff/Spotlight.git
cd Spotlight
```

### Building the Project

The repository includes a pre-built `build/` folder. You can use it directly or create a new build directory:

**Option A – Use existing build:**
```bash
cd build
cmake --build .
```

**Option B – Create a clean build:**
```bash
mkdir build-new
cd build-new
cmake -G "Ninja" ..
cmake --build .
```

### Running the Application

Run the executable from the `bin/` directory:
```bash
./bin/Spotlight.exe
```

Or navigate to the bin folder and run directly:
```bash
cd bin
./Spotlight.exe
```

---

## Gameplay

### Main Menu

Upon launch, the application presents three options:
1. **Start Game** – Begin gameplay with current configuration
2. **Settings** – Configure players, companies, and game parameters
3. **Exit** – Close the application

### Settings Configuration

Set up 2-6 players with associated companies and symbols:

```
Enter number of players (2-6): 6
Player 1 name: Lena
Company name: Orion Dynamics
Company symbol: $
```

Configuration is automatically saved to `config/settings.txt`.

### Console Commands

Control gameplay through text commands in the terminal. Type `help` to display all available commands.

**Example commands:**
```
set_color <x> <y> <z> <color>          # Set tile color
set_owner <x> <y> <z> <company_index>  # Assign tile ownership
build <x> <y> <z> <color> [player]     # Construct building
show_resources [player_index]          # Display player resources
draw_card <deck> <amount> [player]     # Draw cards
end_turn                               # End current turn
```

**Sample session:**
```
> list_players
1: Lena (Orion Dynamics)
2: Kai (Solaris Group)
3: Mira (Eclipse Labs)

> set_color 0 1 -1 red
Tile (0, 1, -1) color set to red.

> build 0 0 0 blue 1
Player 1 built a structure on tile (0, 0, 0).
```

---

## Documentation

### User Manual
Complete setup, gameplay flow, and system design documentation:
- **Location:** `docs/UserManual.pdf`

### API Reference
Comprehensive class and function documentation generated via Doxygen:
- **HTML Documentation:** `docs/Doxygen/html/index.html`
- **PDF Reference Manual:** `docs/Doxygen/refman.pdf`

---

## Project Structure

```
Spotlight/
├── src/                # Source code (.cpp, .hpp)
├── include/            # Header files (if separated)
├── assets/             # Game assets (textures, sounds, data files)
├── bin/                # Compiled executables and required DLLs
│   ├── Spotlight.exe
│   ├── *.dll
│   └── (optional asset subfolders)
├── build/              # CMake build directory (ignored by Git)
├── config/             # Configuration files
│   └── settings.txt
├── docs/               # Documentation
│   ├── UserManual.pdf
│   └── Doxygen/
│       ├── html/       # Auto-generated HTML documentation
│       └── refman.pdf  # Comprehensive PDF reference manual
├── CMakeLists.txt      # Build configuration
├── .gitignore
└── README.md
```

---

## Technologies

- **C++17** – Core programming language
- **SFML** – Graphics rendering and input handling
- **CMake** – Cross-platform build automation
- **Ninja** – High-performance build system
- **Doxygen** – API documentation generation
- **LaTeX** – Professional documentation formatting

---

## Author

**Owen Chilson**  
[GitHub Profile](https://github.com/Dazzlepuff)

---

## License

This project is available for educational and reference purposes.

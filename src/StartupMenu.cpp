#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "StartupMenu.hpp"
#include "Company.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

namespace fs = std::filesystem;

struct GameConfig {
    int playerCount = 2;
    std::vector<std::string> playerNames;
    std::vector<std::string> companyNames;
    std::vector<std::string> companySymbols;
};

static const std::string CONFIG_PATH = "config/settings.txt";

// --- Helper Functions ---
void saveConfig(const GameConfig& cfg) {
    fs::create_directories("config");
    std::ofstream file(CONFIG_PATH);
    if (!file.is_open()) return;

    file << cfg.playerCount << '\n';

    for (int i = 0; i < cfg.playerCount; ++i) {
        file << cfg.playerNames[i] << '\n';
        file << cfg.companyNames[i] << '\n';
        file << cfg.companySymbols[i] << '\n';
    }
}

GameConfig loadConfig() {
    GameConfig cfg;
    std::ifstream file(CONFIG_PATH);
    if (!file.is_open()) return cfg;

    file >> cfg.playerCount;
    file.ignore(); // skip newline

    cfg.playerNames.resize(cfg.playerCount);
    cfg.companyNames.resize(cfg.playerCount);
    cfg.companySymbols.resize(cfg.playerCount);

    for (int i = 0; i < cfg.playerCount; ++i) {
        std::getline(file, cfg.playerNames[i]);
        std::getline(file, cfg.companyNames[i]);
        std::getline(file, cfg.companySymbols[i]);
    }

    return cfg;
}

// --- Settings Menu ---
void settingsMenu(GameConfig& cfg) {
    std::cout << "\n=== SETTINGS ===\n";

    std::cout << "Enter number of players (2–6): ";
    std::cin >> cfg.playerCount;
    if (cfg.playerCount < 2) cfg.playerCount = 2;
    if (cfg.playerCount > 6) cfg.playerCount = 6;

    cfg.playerNames.resize(cfg.playerCount);
    cfg.companyNames.resize(cfg.playerCount);
    cfg.companySymbols.resize(cfg.playerCount);

    std::cin.ignore(); // flush newline

    for (int i = 0; i < cfg.playerCount; ++i) {
        std::cout << "\nPlayer " << i + 1 << " name: ";
        std::getline(std::cin, cfg.playerNames[i]);
        std::cout << "Company name: ";
        std::getline(std::cin, cfg.companyNames[i]);
        std::cout << "Company symbol: ";
        std::getline(std::cin, cfg.companySymbols[i]);
    }

    saveConfig(cfg);
    std::cout << "\nSettings saved!\n";
}

// --- Radius Auto-Selector ---
int getAutoRadius(int playerCount) {
    if (playerCount == 2) return 3;
    if (playerCount == 3) return 4;
    if (playerCount <= 5) return 5;
    return 6;
}

// --- Game Startup ---
int StartupMenu::StartMenuLoop() {
    GameConfig cfg = loadConfig();
    int choice = 0;

    while (true) {
        std::cout << "\n=== MAIN MENU ===\n";
        std::cout << "1. Start Game\n";
        std::cout << "2. Settings\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 1) break;
        else if (choice == 2) settingsMenu(cfg);
        else if (choice == 3) return 0;
        else std::cout << "Invalid choice.\n";
    }

    int radius = getAutoRadius(cfg.playerCount);
    std::cout << "\nStarting game with " << cfg.playerCount 
              << " players (Board radius: " << radius << ")\n";

    // Create Companies
    std::vector<Company> companies;
    for (int i = 0; i < cfg.playerCount; ++i) {
        companies.emplace_back(cfg.companyNames[i], cfg.companySymbols[i]);
    }

    // Setup Game
    Game game(radius, companies);
    for (int i = 0; i < cfg.playerCount; ++i) {
        game.addPlayer(cfg.playerNames[i], &companies[i]);
    }
    game.setup();

    // Setup Renderer
    sf::Font font;
    if (!font.loadFromFile("assets/consolas.ttf")) {
        std::cerr << "Error: Could not load font.\n";
        return -1;
    }

    game.mainLoop();

    return 0;
}

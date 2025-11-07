#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <limits>
#include <SFML/Graphics.hpp>
#include "StartupMenu.hpp"
#include "Company.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "PathUtils.hpp"

namespace fs = std::filesystem;

/**
 * @struct GameConfig
 * @brief Holds persistent player and company setup information.
 */
struct GameConfig {
    int playerCount = 2;                               /**< Number of players in the game (2-6). */
    std::vector<std::string> playerNames;               /**< Names of all players. */
    std::vector<std::string> companyNames;              /**< Names of companies linked to players. */
    std::vector<std::string> companySymbols;            /**< Company symbols or abbreviations. */
};

/**
 * @brief Saves the current game configuration to disk.
 * 
 * Creates the configuration directory if missing and writes settings to "settings.txt".
 * 
 * @param cfg The configuration to save.
 */
void saveConfig(const GameConfig& cfg) {
    auto configDir = PathUtils::getConfigPath();
    auto configFile = PathUtils::getConfigPath("settings.txt");

    fs::create_directories(configDir);
    std::ofstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not write to " << configFile << "\n";
        return;
    }

    file << cfg.playerCount << '\n';
    for (int i = 0; i < cfg.playerCount; ++i) {
        file << cfg.playerNames[i] << '\n';
        file << cfg.companyNames[i] << '\n';
        file << cfg.companySymbols[i] << '\n';
    }

    std::cout << "Saved config to: " << configFile << "\n";
}

/**
 * @brief Loads the game configuration from disk.
 * 
 * If no configuration file exists, defaults are used.
 * 
 * @return Loaded configuration object.
 */
GameConfig loadConfig() {
    GameConfig cfg;
    auto configFile = PathUtils::getConfigPath("settings.txt");

    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "No config found at " << configFile << ". Using defaults.\n";
        return cfg;
    }

    file >> cfg.playerCount;
    file.ignore();

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

/**
 * @brief Opens the settings menu allowing players to edit configuration.
 * 
 * Prompts user input for player count, names, company names, and symbols.
 * Automatically saves the configuration to disk.
 * 
 * @param cfg Reference to configuration to modify.
 */
void settingsMenu(GameConfig& cfg) {
    std::cout << "\n=== SETTINGS ===\n";

    std::cout << "Enter number of players (2-6): ";
    std::cin >> cfg.playerCount;
    
    // Handle invalid input
    if (std::cin.fail()) {
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input. Please enter a number.\n";
        return;
    }
    
    if (cfg.playerCount < 2) cfg.playerCount = 2;
    if (cfg.playerCount > 6) cfg.playerCount = 6;

    cfg.playerNames.resize(cfg.playerCount);
    cfg.companyNames.resize(cfg.playerCount);
    cfg.companySymbols.resize(cfg.playerCount);

    std::cin.ignore();

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

/**
 * @brief Determines the appropriate board radius based on player count.
 * 
 * @param playerCount Number of players in the game.
 * @return The recommended radius value.
 */
int getAutoRadius(int playerCount) {
    if (playerCount == 2) return 3;
    if (playerCount == 3) return 4;
    if (playerCount <= 5) return 5;
    return 6;
}

/**
 * @brief Runs the startup menu loop for the game.
 * 
 * Displays options to start the game, open settings, or exit.
 * Loads saved configuration, sets up game data, and starts the main game loop.
 * 
 * @return 0 when exiting the program.
 */
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

        // Handle invalid input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice.\n";
            continue;
        }

        if (choice == 1) break;
        else if (choice == 2) settingsMenu(cfg);
        else if (choice == 3) return 0;
        else std::cout << "Invalid choice.\n";
    }

    int radius = getAutoRadius(cfg.playerCount);
    std::cout << "\nStarting game with " << cfg.playerCount 
              << " players (Board radius: " << radius << ")\n";

    std::vector<Company> companies;
    for (int i = 0; i < cfg.playerCount; ++i) {
        companies.emplace_back(cfg.companyNames[i], cfg.companySymbols[i]);
    }

    Game game(radius, companies);
    for (int i = 0; i < cfg.playerCount; ++i) {
        game.addPlayer(cfg.playerNames[i], &companies[i]);
    }
    game.setup();
    game.mainLoop();

    return 0;
}
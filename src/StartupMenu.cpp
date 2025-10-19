#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <limits>
#include "StartupMenu.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "Company.hpp"
#include <SFML/Graphics.hpp>

struct MenuOption {
    std::string label;
    std::function<void()> action;
};

class Menu {
public:
    Menu(const std::string& title) : title(title) {}

    void addOption(const std::string& label, std::function<void()> action) {
        options.push_back({label, action});
    }

    void run() {
        while (true) {
            std::cout << "\n===== " << title << " =====\n";
            for (size_t i = 0; i < options.size(); ++i)
                std::cout << i + 1 << ". " << options[i].label << "\n";
            std::cout << "Select an option: ";

            int choice;
            std::cin >> choice;

            if (std::cin.fail() || choice < 1 || choice > static_cast<int>(options.size())) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Try again.\n";
                continue;
            }

            options[choice - 1].action();
        }
    }

private:
    std::string title;
    std::vector<MenuOption> options;
};

StartupMenu::StartupMenu() {}

int StartupMenu::StartMenuLoop() {
    bool running = true;

    Menu mainMenu("SPOTLIGHT");
    Menu optionsMenu("Options");

    optionsMenu.addOption("Audio Settings", []() {
        std::cout << "Audio Settings (placeholder)\n";
    });
    optionsMenu.addOption("Video Settings", []() {
        std::cout << "Video Settings (placeholder)\n";
    });
    optionsMenu.addOption("Back", []() {
        throw std::runtime_error("BACK");
    });

    mainMenu.addOption("Start Game", [&]() {
        std::cout << "Starting Spotlight...\n";

        std::vector<Company> companies = {
            Company("Aurora Industries", "@"),
            Company("NovaWorks", "&")
        };

        Game game(6);
        game.addPlayer("Owen", &companies[0]);
        game.addPlayer("Aaron", &companies[1]);
        game.setup();

        sf::Font font;
        if (!font.loadFromFile("assets/consolas.ttf")) {
            std::cerr << "Error loading font.\n";
            return;
        }

        Renderer renderer(game.board, font, 1600, 1200);
        renderer.run();
    });

    mainMenu.addOption("Options", [&]() {
        try {
            optionsMenu.run();
        } catch (const std::runtime_error& e) {
            if (std::string(e.what()) == "BACK") {
            } else {
                throw;
            }
        }
    });

    mainMenu.addOption("Exit", [&]() {
        std::cout << "Goodbye!\n";
        exit(0);
    });

    mainMenu.run();
    return 0;
}

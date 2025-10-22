#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <string>

#include "Renderer.hpp"
#include "CommandConsole.hpp"

Game::Game(int boardSize)
    : board(boardSize),
      window(sf::VideoMode(1600, 1200), "Hex Board"),
      font()
{
    if (!font.loadFromFile("assets/consolas.ttf")) {
        std::cerr << "Failed to load font!\n";
    }

    // Place console near bottom-left of the window (for example)
    sf::Vector2f consolePosition(20.f, 900.f);

    console = new CommandConsole(board, font, consolePosition);
    renderer = new Renderer(board, font);
}

Game::~Game(){
    delete renderer;
    delete console;
}

void Game::addPlayer(const std::string& name, Company* company) {
    players.emplace_back(name, company);
}

void Game::setup() {
    std::vector<std::string> colors = {"Red", "Yellow", "Blue", "Green", "Purple", "White"};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(0, colors.size() - 1);
    std::uniform_int_distribution<> ownerDist(0, static_cast<int>(players.size()) - 1);

    std::vector<CubeCoord> tileCoords;
    tileCoords.reserve(board.tiles.size());
    for (const auto& [coord, _] : board.tiles) {
        tileCoords.push_back(coord);
    }

    std::shuffle(tileCoords.begin(), tileCoords.end(), gen);

    size_t half = tileCoords.size() / 2;
    for (size_t i = 0; i < half; ++i) {
        auto& tile = board.tiles[tileCoords[i]];
        tile.setColor(colors[colorDist(gen)]);
        tile.setOwner(players[ownerDist(gen)].company);
    }

    for (size_t i = half; i < tileCoords.size(); ++i) {
        auto& tile = board.tiles[tileCoords[i]];
        tile.setColor("Gray");
        tile.setOwner(nullptr);
    }
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

void Game::mainLoop() {
    while (window.isOpen()) {
        renderer->handleEvents(window, *console);

        while (console->hasCommand()) {
            std::string cmd = console->nextCommand();
            executeCommand(cmd);
        }

        renderer->render(window, *console);
    }
}


void Game::executeCommand(const std::string& cmd) {
    std::istringstream ss(cmd);
    std::string action;
    ss >> action;

    if (action == "color") {
        int x, y, z;
        std::string color;
        ss >> x >> y >> z >> color;

        if (!ss.fail()) {
            board.setTileColor(x, y, z, color);
            std::cout << "Set tile (" << x << "," << y << "," << z << ") to " << color << "\n";
        }
    }
    else if (action == "list_players") {
        for (const auto& p : players)
            std::cout << p.name << " (" << p.company->getName() << ")\n";
    }
    else {
        std::cout << "Unknown command: " << action << "\n";
    }
}

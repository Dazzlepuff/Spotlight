#include "Game.hpp"
#include <iostream>
#include <random>
#include <vector>

Game::Game(int boardSize) : board(boardSize){

}

void Game::addPlayer(const std::string& name, Company* company) {
    players.emplace_back(name, company);
}

void Game::setup() {
    std::cout << "Setting up game..." << std::endl;

    std::vector<std::string> colors = {"Red", "Yellow", "Blue", "Green", "Black", "White", "Gray"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, colors.size() - 1);

    for (auto& [coord, tile] : board.tiles) {
        tile.setColor(colors[dist(gen)]);
    }

    if (players.size() >= 2) {
        CubeCoord tile1(0, 0, 0);
        CubeCoord tile2(1, -1, 0);
        board.assignTileOwner(tile1, players[0].company);
        board.assignTileOwner(tile2, players[1].company);
    }
}

void Game::start() {
    std::cout << "Game starting..." << std::endl;
    board.printBoard();

    CubeCoord coord(0, 0, 0);
    Tile* tile = board.getTile(coord);
    if (tile) tile->printInfo();
}

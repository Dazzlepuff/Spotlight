#include "Game.hpp"
#include <iostream>
#include <vector>

Game::Game(int boardSize) : board(boardSize){

}

void Game::addPlayer(const std::string& name, Company* company) {
    players.emplace_back(name, company);
}

void Game::setup() {
    std::cout << "Setting up game..." << std::endl;

    std::vector<Company> companies = {
        Company("company1"),
        Company("company2")
    };

    if (players.size() >= 2) {
        CubeCoord tile1(0, 0, 0);
        CubeCoord tile2(1, -1, 0);
        board.assignTileOwner(tile1, companies[0]);
        board.assignTileOwner(tile2, companies[1]);
    }
}

void Game::start() {
    std::cout << "Game starting..." << std::endl;
    board.printBoard();

    CubeCoord coord(0, 0, 0);
    Tile* tile = board.getTile(coord);
    if (tile) tile->printInfo();
}

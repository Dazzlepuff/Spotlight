#include "Board.hpp"
#include <iostream>
#include <cmath>

static const std::vector<CubeCoord> DIRECTIONS = {
    CubeCoord(1, -1, 0), CubeCoord(1, 0, -1), CubeCoord(0, 1, -1),
    CubeCoord(-1, 1, 0), CubeCoord(-1, 0, 1), CubeCoord(0, -1, 1)
};

Board::Board(int radius_) : radius(radius_) {
    generateBoard();
}

void Board::generateBoard() {
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            int z = -x - y;
            if (std::abs(z) <= radius) {
                tiles.emplace(CubeCoord(x, y, z), Tile{});
            }
        }
    }
}

Tile* Board::getTile(const CubeCoord& coord) {
    auto it = tiles.find(coord);
    if (it != tiles.end()) return &it->second;
    return nullptr;
}

std::vector<CubeCoord> Board::getNeighbors(const CubeCoord& coord) const {
    std::vector<CubeCoord> result;
    for (auto& dir : DIRECTIONS) {
        CubeCoord neighbor = coord + dir;
        if (tiles.find(neighbor) != tiles.end()) {
            result.push_back(neighbor);
        }
    }
    return result;
}

void Board::assignTileOwner(const CubeCoord& coord, Company* company) {
    Tile* tile = getTile(coord);
    if (tile) tile->setOwner(company);
}

void Board::printBoard() const {
    std::cout << "Board radius " << radius
              << " has " << tiles.size() << " tiles.\n";
}

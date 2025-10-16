#include "Board.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>

static std::map<std::string, std::string> colorCodes = {
    {"Red",     "\033[41m  \033[0m"},
    {"Yellow",  "\033[43m  \033[0m"},
    {"Blue",    "\033[44m  \033[0m"},
    {"Green",   "\033[42m  \033[0m"},
    {"Purple",  "\033[45m  \033[0m"},
    {"White",   "\033[47m  \033[0m"},
    {"Gray",    "\033[100m  \033[0m"},
    {"neutral", "\033[0m..."}
};

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
    std::cout << "\n=== Board View ===\n";

    for (int y = -radius; y <= radius; ++y) {
        int indent = std::abs(y);
        std::cout << std::string(indent , ' ');

        for (int x = -radius; x <= radius; ++x) {
            int z = -x - y;
            if (std::abs(z) <= radius) {
                CubeCoord coord(x, y, z);
                auto it = tiles.find(coord);
                if (it != tiles.end()) {
                    const std::string& c = it->second.getColor();
                    if (colorCodes.count(c))
                        std::cout << colorCodes[c];
                    else
                        std::cout << colorCodes["neutral"];
                }
            }
        }
        std::cout << '\n';
    }

    std::cout << "\n";
}

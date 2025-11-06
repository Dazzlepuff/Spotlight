#include "Board.hpp"
#include "Colors.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>

/**
 * @details
 * Predefined cube-coordinate direction vectors.
 * Used to compute neighboring tiles via coordinate addition.
 * These six directions correspond to the edges of a hexagon.
 */
static const std::vector<CubeCoord> DIRECTIONS = {
    CubeCoord(1, -1, 0), CubeCoord(1, 0, -1), CubeCoord(0, 1, -1),
    CubeCoord(-1, 1, 0), CubeCoord(-1, 0, 1), CubeCoord(0, -1, 1)
};

/** @brief Initializes a new Board and immediately generates its tiles. */
Board::Board(int radius_) : radius(radius_) {
    generateBoard();
}

/**
 * @details
 * Generates all cube coordinates where |x|, |y|, and |z| are within radius,
 * satisfying the invariant x + y + z = 0.
 * Each valid coordinate is inserted into the board’s tile map.
 */
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

/** @brief Returns a pointer to the Tile at the given cube coordinate, or nullptr if missing. */
Tile* Board::getTile(const CubeCoord& coord) {
    auto it = tiles.find(coord);
    if (it != tiles.end()) return &it->second;
    return nullptr;
}

/**
 * @details
 * Iterates through the six cube-coordinate directions to find
 * all existing adjacent tiles. Excludes invalid or out-of-bounds tiles.
 */
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

/**
 * @details
 * Updates the ownership of a specific tile if it exists.
 * Useful for territory claiming or resource control systems.
 */
void Board::setTileOwner(int x, int y, int z, Company* company) {
    CubeCoord coord(x, y, z);
    Tile* tile = getTile(coord);
    if (tile) tile->setOwner(company);
}

/**
 * @details
 * Validates the target coordinate and color before assignment.
 * Logs errors if either is invalid but continues execution safely.
 */
void Board::setTileColor(int x, int y, int z, const std::string& color) {
    CubeCoord coord(x, y, z);
    Tile* tile = getTile(coord);

    if (!tile) {
        std::cerr << "Tile (" << x << "," << y << "," << z << ") does not exist.\n";
        return;
    } 
    if (!Colors::isValid(color)) {
        std::cerr << "Color \"" << color << "\" is not a valid color.\n";
    }
    tile->setColor(color);
}

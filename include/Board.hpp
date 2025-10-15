#pragma once
#include "CubeCoord.hpp"
#include "Tile.hpp"
#include "Company.hpp"
#include <unordered_map>
#include <vector>

class Board {
public:
    Board(int radius);

    std::unordered_map<CubeCoord, Tile, CubeCoordHash> tiles;

    Tile* getTile(const CubeCoord& coord);
    std::vector<CubeCoord> getNeighbors(const CubeCoord& coord) const;

    void assignTileOwner(const CubeCoord& coord, Company* company);
    void printBoard() const;

private:
    int radius;
    void generateBoard();
};

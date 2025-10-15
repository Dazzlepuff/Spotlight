#include "Tile.hpp"
#include <iostream>

void Tile::printInfo() const {
    if (owner)
        std::cout << "Tile owned by " << owner->name
                  << " (" << color << ")\n";
    else
        std::cout << "Unowned tile (" << color << ")\n";
}

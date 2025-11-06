#include "Tile.hpp"
#include <iostream>

// Default constructor: no owner, neutral color.
Tile::Tile() : owner(nullptr), color("neutral") {}

Company* Tile::getOwner() const {
    return owner;
}

void Tile::setOwner(Company* newOwner) {
    owner = newOwner;
}

const std::string& Tile::getColor() const {
    return color;
}

void Tile::setColor(const std::string& newColor) {
    color = newColor;
}

// Simple console output for debugging and visualization.
void Tile::printInfo() const {
    if (owner)
        std::cout << "Tile owned by " << owner->getName()
                  << " (" << color << ")\n";
    else
        std::cout << "Unowned tile (" << color << ")\n";
}

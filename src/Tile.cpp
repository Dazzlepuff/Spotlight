#include "Tile.hpp"
#include <iostream>

Tile::Tile() : owner(nullptr), color("neutral") {}

void Tile::setOwner(Company* newOwner) {
    owner = newOwner;
}

Company* Tile::getOwner() const {
    return owner;
}

const std::string& Tile::getColor() const {
    return color;
}

void Tile::setColor(const std::string& newColor) {
    color = newColor;
}

void Tile::printInfo() const {
    if (owner)
        std::cout << "Tile owned by " << owner->getName()
                  << " (" << color << ")\n";
    else
        std::cout << "Unowned tile (" << color << ")\n";
}

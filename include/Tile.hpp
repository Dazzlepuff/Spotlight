#pragma once
#include <string>
#include "Company.hpp"

class Tile {
public:
    Tile();

    void setOwner(Company* newOwner);
    Company* getOwner() const;

    const std::string& getColor() const;
    void setColor(const std::string& newColor);

    void printInfo() const;

private:
    Company* owner;  
    std::string color;
};

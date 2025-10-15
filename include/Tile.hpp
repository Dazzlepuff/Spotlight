#pragma once
#include <string>
#include "Company.hpp"

class Tile {
public:
    Tile() : owner(nullptr), color("neutral") {

    }

    void setOwner(Company* newOwner){
        owner = newOwner;
    }

    Company* getOwner() const {
        return owner; 
    }
    
    const std::string& getColor() const {
        return color; 
    }

    void printInfo() const;

private:
    Company* owner;  
    std::string color;
};

#pragma once
#include <string>
#include "Company.hpp"

class Player {
public:
    std::string name;
    Company* company;

    Player(const std::string& n, Company* c) : name(n), company(c){

    }
};

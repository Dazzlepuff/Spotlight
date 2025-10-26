#pragma once
#include <string>

class Card {
public:
    std::string name;
    std::string description;

    Card(const std::string& n, const std::string& d)
        : name(n), description(d) {}
};
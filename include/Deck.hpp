#pragma once
#include <vector>
#include "Card.hpp"

class Deck {
public:
    std::vector<Card> drawPile;
    std::vector<Card> discardPile;

    void loadFromJsonFile(const std::string& filename);
};

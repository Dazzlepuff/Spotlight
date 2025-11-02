#pragma once
#include <vector>
#include <string>
#include "Card.hpp"

class Deck {
public:
    std::vector<Card> drawPile;
    std::vector<Card> discardPile;

    void loadFromJsonFile(const std::string& filename);
    Card drawCard();
    void discardCard(const Card& card);
    bool isEmpty() const;
};

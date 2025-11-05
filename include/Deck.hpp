#pragma once
#include <vector>
#include <string>
#include "Card.hpp"

class Deck {
public:
    Deck(const std::string& deckName = "") : name(deckName) {}   

    std::string name;

    std::vector<Card> cards;

    void loadFromJsonFile(const std::string& filename);

    void addCard(const Card& card);

    Card drawCard();
    void moveCardTo(const Card& card, Deck& deck);
    void shuffle();

    size_t size() const;
    bool empty() const;
};

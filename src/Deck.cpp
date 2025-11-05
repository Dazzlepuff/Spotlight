#include "Deck.hpp"
#include "PathUtils.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <random>

void Deck::loadFromJsonFile(const std::string& filename) {
    std::filesystem::path path = PathUtils::getAssetPath(filename);

    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: Could not find " << path << "\n";
        return;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << path << "\n";
        return;
    }

    nlohmann::json data;
    try {
        file >> data;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON from " << path << ": " << e.what() << "\n";
        return;
    }

    cards.clear();
    for (const auto& cardData : data) {
        int copies = 1;
        if (cardData.contains("copies") && cardData["copies"].is_number_integer()) {
            copies = cardData["copies"];
        }

        for (int i = 0; i < copies; ++i) {
            cards.emplace_back(cardData);
        }
    }

    std::cout << "Loaded " << cards.size() << " cards from " << path << "\n";
}

void Deck::addCard(const Card& card){
    cards.push_back(card);
}

Card Deck::drawCard() {
    if (cards.empty()) {
        std::cerr << "Error: Attempted to draw a card from an empty deck ('" << name << "').\n";
        return Card();
    }

    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::moveCardTo(const Card& card, Deck& deck) {
    deck.addCard(card);
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

size_t Deck::size() const {
    return cards.size();
}

bool Deck::empty() const {
    return cards.empty();
}
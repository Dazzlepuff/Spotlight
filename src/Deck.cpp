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

    drawPile.clear();
    for (const auto& cardData : data) {
        drawPile.emplace_back(cardData);
    }

    std::cout << "Loaded " << drawPile.size() << " cards from " << path << "\n";
}

Card Deck::drawCard() {
    if (drawPile.empty()) {
        // Reshuffle discard pile into draw pile
        if (!discardPile.empty()) {
            std::cout << "Reshuffling discard pile...\n";
            drawPile = discardPile;
            discardPile.clear();
            std::shuffle(drawPile.begin(), drawPile.end(), std::mt19937{std::random_device{}()});
        } else {
            std::cerr << "Error: No cards left in deck!\n";
            return Card(); // return an empty/default card
        }
    }

    Card card = drawPile.back();
    drawPile.pop_back();
    return card;
}

void Deck::discardCard(const Card& card) {
    discardPile.push_back(card);
}

bool Deck::isEmpty() const {
    return drawPile.empty() && discardPile.empty();
}

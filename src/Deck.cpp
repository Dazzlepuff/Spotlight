#include "Deck.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

void Deck::loadFromJsonFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return;
    }

    nlohmann::json data;
    file >> data;

    drawPile.clear();
    for (const auto& cardData : data) {
        drawPile.emplace_back(cardData);
    }

    std::cout << "Loaded " << drawPile.size() << " cards from " << filename << "\n";
}

#include "Deck.hpp"
#include "PathUtils.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <random>

/**
 * @details
 * Uses PathUtils::getAssetPath() to resolve the full path of the file.
 * Cards are loaded from JSON, supporting an optional "copies" field for duplication.
 * Any parsing or I/O errors are printed to stderr.
 */
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

    /** @note Default copies = 1 unless explicitly defined in JSON. */
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

/** @brief Adds a card directly to the deck vector. */
void Deck::addCard(const Card& card){
    cards.push_back(card);
}

/**
 * @details
 * Returns the last element of the vector to simulate drawing from the top.
 * Logs an error if called on an empty deck and returns a default card.
 */
Card Deck::drawCard() {
    if (cards.empty()) {
        std::cerr << "Error: Attempted to draw a card from an empty deck ('" << name << "').\n";
        return Card();
    }

    Card card = cards.back();
    cards.pop_back();
    return card;
}

/**
 * @details
 * Only appends the card to the destination deck — it does not remove it locally.
 * This enables flexible gameplay behavior (e.g., cloning, discarding).
 */
void Deck::moveCardTo(const Card& card, Deck& deck) {
    deck.addCard(card);
}

/**
 * @details
 * Uses std::shuffle with Mersenne Twister (mt19937) seeded from random_device.
 * Ensures fair shuffling each time it’s called.
 */
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

/** @brief Returns the current card count in the deck. */
size_t Deck::size() const {
    return cards.size();
}

/** @brief Returns true if there are no cards remaining in the deck. */
bool Deck::empty() const {
    return cards.empty();
}

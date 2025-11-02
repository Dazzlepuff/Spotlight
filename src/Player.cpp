#include "Player.hpp"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& n, Company* c) : name(n), company(c) {
    resources["talent"] = 0;
    resources["staff"]  = 0;
    resources["gear"]   = 0;
    resources["funds"]  = 0;
}

void Player::addResource(const std::string& type, int amount) {
    resources[type] += amount;
}

bool Player::spendResource(const std::string& type, int amount) {
    auto it = resources.find(type);
    if (it == resources.end() || it->second < amount) {
        return false;
    }
    it->second -= amount;
    return true;
}

int Player::getResource(const std::string& type) const {
    auto it = resources.find(type);
    return (it != resources.end()) ? it->second : 0;
}

void Player::addHeldCard(const Card& card) {
    heldCards.push_back(card);
}

bool Player::playCard(const std::string& cardName) {
    auto it = std::find_if(heldCards.begin(), heldCards.end(),
                           [&](const Card& c) { return c.name == cardName; });

    if (it == heldCards.end()) {
        return false;
    }

    playedCards.push_back(*it);
    heldCards.erase(it);
    return true;
}

bool Player::removeHeldCard(const std::string& cardName) {
    auto it = std::remove_if(heldCards.begin(), heldCards.end(),
                             [&](const Card& c) { return c.name == cardName; });
    if (it != heldCards.end()) {
        heldCards.erase(it, heldCards.end());
        return true;
    }
    return false;
}

bool Player::removePlayedCard(const std::string& cardName) {
    auto it = std::remove_if(playedCards.begin(), playedCards.end(),
                             [&](const Card& c) { return c.name == cardName; });
    if (it != playedCards.end()) {
        playedCards.erase(it, playedCards.end());
        return true;
    }
    return false;
}

void Player::addScore(int amount) {
    score += amount;
}

void Player::resetScore() {
    score = 0;
}

void Player::printStatus() const {
    std::cout << "\n=== Player: " << name << " ===\n";
    std::cout << "Company: " << (company ? company->getName() : "None") << "\n";
    std::cout << "Score: " << score << "\n";

    std::cout << "Resources:\n";
    for (const auto& [key, val] : resources)
        std::cout << "  - " << key << ": " << val << "\n";

    std::cout << "Held Cards:\n";
    for (const auto& card : heldCards)
        std::cout << "  - " << card.name << "\n";

    std::cout << "Played Cards:\n";
    for (const auto& card : playedCards)
        std::cout << "  - " << card.name << "\n";
}

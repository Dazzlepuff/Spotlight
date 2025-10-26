#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Company.hpp"
#include "Card.hpp"

class Player {
public:
    std::string name;
    Company* company;

    int score = 0;

    std::unordered_map<std::string, int> resources;
    std::vector<Card> cards;

    Player(const std::string& n, Company* c);

    void addResource(const std::string& type, int amount);
    bool spendResource(const std::string& type, int amount);
    int getResource(const std::string& type) const;

    void addCard(const Card& card);
    bool removeCard(const std::string& cardName);

    void addScore(int amount);
    void resetScore();

    void printStatus() const;
};

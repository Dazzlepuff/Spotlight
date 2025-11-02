// Card.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

class Card {
public:
    std::string name;
    std::string description;

    // Each trigger maps to a list of action objects
    // e.g. { "onPlay": [ { "action": "addResource", "type": "funds", "amount": 2 } ] }
    std::unordered_map<std::string, std::vector<nlohmann::json>> triggers;

    Card() = default;
    Card(const nlohmann::json& data);

    // Run all actions tied to a given trigger (e.g., "onPlay", "onStartOfDay")
    void executeTrigger(const std::string& trigger, class Player& player) const;
};

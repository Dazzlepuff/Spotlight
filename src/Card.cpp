#include "Card.hpp"
#include "Player.hpp"
#include <iostream>

// Construct a Card from JSON data.
Card::Card(const nlohmann::json& data) {
    name = data.value("name", "Unnamed Card");
    description = data.value("description", "");

    // Parse triggers if present.
    if (data.contains("triggers") && data["triggers"].is_object()) {
        for (auto& [triggerName, actions] : data["triggers"].items()) {
            if (actions.is_array()) {
                triggers[triggerName] = actions.get<std::vector<nlohmann::json>>();
            }
        }
    }
}

// Execute all actions associated with the given trigger.
void Card::executeTrigger(const std::string& trigger, Player& player) const {
    auto it = triggers.find(trigger);
    if (it == triggers.end()) return;  // No actions tied to this trigger.

    for (const auto& action : it->second) {
        std::string actionType = action.value("action", "");
        
        if (actionType == "addResource") {
            std::string type = action.value("type", "");
            int amount = action.value("amount", 0);
            player.addResource(type, amount);
            std::cout << player.name << " gains " << amount << " " << type
                      << " from " << name << " (" << trigger << ")\n";
        }

        // Future action types (e.g., removeResource, drawCard, etc.) can be added here.
    }
}

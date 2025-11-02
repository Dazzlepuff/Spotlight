#include "Card.hpp"
#include "Player.hpp"
#include <iostream>

Card::Card(const nlohmann::json& data) {
    name = data.value("name", "Unnamed Card");
    description = data.value("description", "");
    type = data.value("type", "");
    trigger = data.value("trigger", "");

    if (data.contains("effect")) {
        effectResource = data["effect"].value("resource", "");
        effectAmount = data["effect"].value("amount", 0);
    }
}

void Card::applyEffectToPlayer(Player& player) const {
    if (!effectResource.empty() && effectAmount != 0) {
        player.addResource(effectResource, effectAmount);
        std::cout << player.name << " gains " << effectAmount 
                  << " " << effectResource 
                  << " from " << name << "!\n";
    }
}

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

/**
 * @class Card
 * @brief Represents a playable card with triggers and associated actions.
 *
 * Each card contains a name, description, and a set of triggers that define
 * when certain actions occur (e.g., "onPlay", "onStartOfDay").
 * Actions are stored as JSON objects that can be executed dynamically.
 */
class Card {
public:
    std::string name;   /**< The display name of the card. */
    std::string description;  /**< Text description or effect summary. */

    /**
     * @brief Maps trigger event names to a list of action definitions.
     *
     * Example format:
     * @code
     * {
     *   "onPlay": [
     *     { "action": "addResource", "type": "funds", "amount": 2 }
     *   ]
     * }
     * @endcode
     */
    std::unordered_map<std::string, std::vector<nlohmann::json>> triggers;

    /**
     * @brief Default constructor. Initializes an empty card.
     */
    Card() = default;

    /**
     * @brief Constructs a card from parsed JSON data.
     * @param data JSON object containing card properties and triggers.
     */
    Card(const nlohmann::json& data);

    /**
     * @brief Executes all actions tied to a given trigger.
     * @param trigger The trigger event name (e.g. "onPlay").
     * @param player The player affected by this card’s actions.
     */
    void executeTrigger(const std::string& trigger, class Player& player) const;
};

/**
 * @file Player.hpp
 * @brief Defines the Player class, representing a participant with resources, cards, and a company affiliation.
 *
 * Handles resource management, card play/removal, and scoring for an individual player in the game.
 */

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Company.hpp"
#include "Card.hpp"

/**
 * @class Player
 * @brief Represents a player, managing their resources, cards, score, and associated company.
 */
class Player {
public:
    /** @brief The player’s display name. */
    std::string name;

    /** @brief Pointer to the company this player is associated with. */
    Company* company;

    /** @brief Current score value. */
    int score = 0;

    /** @brief Map of resource type to quantity (e.g., "funds", "gear"). */
    std::unordered_map<std::string, int> resources;

    /** @brief Cards currently held in hand. */
    std::vector<Card> heldCards;

    /** @brief Cards that have been played. */
    std::vector<Card> playedCards;

    /**
     * @brief Constructs a new Player.
     * @param n The player’s name.
     * @param c Pointer to the company associated with the player.
     */
    Player(const std::string& n, Company* c);

    /**
     * @brief Adds a specified amount of a resource type.
     * @param type The resource key.
     * @param amount The quantity to add.
     */
    void addResource(const std::string& type, int amount);

    /**
     * @brief Attempts to spend a specified amount of a resource.
     * @param type The resource key.
     * @param amount The quantity to spend.
     * @return True if the player had enough resources; false otherwise.
     */
    bool spendResource(const std::string& type, int amount);

    /**
     * @brief Retrieves the current amount of a resource.
     * @param type The resource key.
     * @return The quantity of the specified resource.
     */
    int getResource(const std::string& type) const;

    /**
     * @brief Adds a card to the player’s hand.
     * @param card The card to add.
     */
    void addHeldCard(const Card& card);

    /**
     * @brief Moves a card from hand to played cards.
     * @param cardName The name of the card to play.
     * @return True if the card was found and played; false otherwise.
     */
    bool playCard(const std::string& cardName);

    /**
     * @brief Removes a played card by name.
     * @param cardName The name of the card to remove.
     * @return True if a card was removed; false otherwise.
     */
    bool removePlayedCard(const std::string& cardName);

    /**
     * @brief Removes a held card by name.
     * @param cardName The name of the card to remove.
     * @return True if a card was removed; false otherwise.
     */
    bool removeHeldCard(const std::string& cardName);

    /**
     * @brief Increases the player’s score by a specified amount.
     * @param amount The number of points to add.
     */
    void addScore(int amount);

    /** @brief Resets the player’s score to zero. */
    void resetScore();

    /** @brief Prints a formatted summary of the player’s current status to the console. */
    void printStatus() const;
};

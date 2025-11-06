/**
 * @file Deck.hpp
 * @brief Declares the Deck class used for managing collections of Card objects.
 *
 * @details
 * The Deck class represents a container of Card instances. It provides
 * functionality for loading cards from JSON files, shuffling, drawing,
 * transferring cards between decks, and querying deck state.
 *
 * @note This class assumes that the Card type supports construction from JSON objects.
 * @see Card, PathUtils
 *
 * @date 2025-11-06
 * @version 1.0
 * @author
 * Owen Chilson
 */

#pragma once
#include <vector>
#include <string>
#include "Card.hpp"

/**
 * @class Deck
 * @brief Represents a collection of cards and provides utility operations for card management.
 *
 * @details
 * The Deck class encapsulates a vector of Card objects along with a name identifier.
 * It supports loading cards from external JSON data, shuffling the deck,
 * drawing cards, and transferring them to other decks.
 * The design is extensible for various game contexts that require deck-based operations.
 */
class Deck {
public:
    /**
     * @brief Constructs a deck with an optional name.
     * @param[in] deckName Optional name identifier for the deck.
     * @post Deck is initialized with an empty card list.
     */
    Deck(const std::string& deckName = "") : name(deckName) {}

    /** @brief The deck’s name, used for identification or debugging. */
    std::string name;

    /** @brief The internal storage of cards contained within the deck. */
    std::vector<Card> cards;

    /**
     * @brief Loads cards into the deck from a JSON file.
     *
     * @details
     * The JSON file must contain an array of card definitions compatible
     * with the Card class constructor. Each card may optionally define a
     * `"copies"` field indicating how many duplicates to create.
     *
     * @param[in] filename The name or relative path of the JSON file to load.
     * @pre The file must exist and contain valid JSON.
     * @post The deck’s card list is cleared and replaced with the loaded cards.
     * @throws std::exception If JSON parsing fails (caught internally and logged).
     * @warning Logs errors to stderr if the file cannot be found or opened.
     * @see Card::Card(const nlohmann::json&)
     */
    void loadFromJsonFile(const std::string& filename);

    /**
     * @brief Adds a card to the deck.
     * @param[in] card The card to be added.
     * @post The card is appended to the end of the internal vector.
     */
    void addCard(const Card& card);

    /**
     * @brief Draws the topmost card from the deck.
     *
     * @details
     * Removes the last card from the internal vector (LIFO order) and returns it.
     * If the deck is empty, an empty Card is returned instead.
     *
     * @pre The deck should contain at least one card.
     * @post The deck’s size decreases by one, unless empty.
     * @return The drawn Card object.
     * @warning Returns a default-constructed Card if the deck is empty.
     */
    Card drawCard();

    /**
     * @brief Transfers a card to another deck.
     *
     * @param[in] card The card to move.
     * @param[out] deck The destination deck that receives the card.
     * @post The specified card is added to the destination deck.
     * @note This operation does not remove the card from the source deck.
     */
    void moveCardTo(const Card& card, Deck& deck);

    /**
     * @brief Randomly shuffles the order of cards in the deck.
     *
     * @details
     * Uses a Mersenne Twister pseudo-random generator seeded from std::random_device.
     * Suitable for gameplay randomness but not cryptographically secure.
     *
     * @post All cards remain present, but in randomized order.
     */
    void shuffle();

    /**
     * @brief Gets the number of cards currently in the deck.
     * @return The count of cards.
     * @post Does not modify the deck.
     */
    size_t size() const;

    /**
     * @brief Checks whether the deck is empty.
     * @return True if the deck has no cards, false otherwise.
     * @post Does not modify the deck.
     */
    bool empty() const;
};

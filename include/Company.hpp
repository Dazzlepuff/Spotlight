/**
 * @file Company.hpp
 * @brief Declares the Company class representing a business entity or player faction.
 *
 * @details
 * The Company class encapsulates the identifying information of a corporate or
 * player-controlled entity within the game. Each company is characterized by a
 * unique name and symbol, which may be used for display, ownership marking,
 * or data serialization.
 *
 * @see Tile, Board
 *
 * @date 2025-11-06
 * @version 1.0
 * @author
 * Owen Chilson
 */

#pragma once
#include <string>

/**
 * @class Company
 * @brief Represents a company, corporation, or in-game faction.
 *
 * @details
 * The Company class stores immutable identifiers that describe an entity’s
 * identity and branding in the simulation or game world. Its instances can be
 * linked to tiles, assets, or scores via references or pointers.
 */
class Company {
public:
    /**
     * @brief Constructs a new Company with the specified name and symbol.
     *
     * @param[in] name The full display name of the company.
     * @param[in] symbol A short identifying symbol or abbreviation.
     * @pre Both name and symbol should be non-empty strings.
     * @post Initializes the company with the provided identifiers.
     */
    Company(const std::string& name, const std::string& symbol);

    /**
     * @brief Retrieves the company’s full name.
     * @return A constant reference to the company’s name.
     * @post Does not modify the object.
     */
    const std::string& getName() const;

    /**
     * @brief Retrieves the company’s identifying symbol.
     * @return A constant reference to the company’s symbol string.
     * @post Does not modify the object.
     */
    const std::string& getSymbol() const;

    /**
     * @brief Updates the company’s name.
     * @param[in] newName The new name to assign.
     * @post The internal name string is replaced with newName.
     * @warning Should be used cautiously if company instances are referenced elsewhere.
     */
    void setName(const std::string& newName);

    /**
     * @brief Updates the company’s identifying symbol.
     * @param[in] newSymbol The new symbol to assign.
     * @post The internal symbol string is replaced with newSymbol.
     * @warning Use carefully if the symbol is used as a unique key elsewhere.
     */
    void setSymbol(const std::string& newSymbol);

private:
    /** @brief The company’s full display name. */
    std::string name;

    /** @brief The company’s identifying abbreviation or logo symbol. */
    std::string symbol;
};

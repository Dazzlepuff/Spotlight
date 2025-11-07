/**
 * @file Tile.hpp
 * @brief Declares the Tile class, representing a single board tile that can be owned and colored.
 *
 * @details
 * Each Tile tracks its ownership by a Company and a color state, providing methods
 * for setting and retrieving this information. Tiles default to being unowned and "neutral" in color.
 * 
 * @date 2025-11-06
 * @version 1.0
 * @author
 * Owen Chilson
 */

#pragma once
#include <string>
#include "Company.hpp"

/**
 * @class Tile
 * @brief Represents a single board tile that can be owned by a company and display a color.
 *
 * Each tile may have an owner (a pointer to a Company) and an associated color.
 * Tiles start unowned and "neutral" in color by default.
 */
class Tile {
public:
    /**
     * @brief Constructs a neutral, unowned tile.
     */
    Tile();

    /**
     * @brief Sets the owner of this tile.
     * @param newOwner Pointer to the company that now owns the tile. Can be nullptr for unowned.
     */
    void setOwner(Company* newOwner);

    /**
     * @brief Retrieves the company that owns this tile.
     * @return Pointer to the owning company, or nullptr if unowned.
     */
    Company* getOwner() const;

    /**
     * @brief Gets the tile's current color.
     * @return Reference to the color string.
     */
    const std::string& getColor() const;

    /**
     * @brief Sets the tile's color.
     * @param newColor New color name (string identifier).
     */
    void setColor(const std::string& newColor);

    /**
     * @brief Prints a formatted line describing ownership and color to the console.
     */
    void printInfo() const;

private:
    Company* owner;          /**< Pointer to the owning company (nullptr if unowned). */
    std::string color;       /**< Color of the tile, defaults to "neutral". */
};

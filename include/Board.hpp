/**
 * @file Board.hpp
 * @brief Declares the Board class which represents a hexagonal grid of tiles.
 *
 * @details
 * The Board class manages a collection of Tile objects arranged using cube coordinates.
 * It provides functions for tile retrieval, neighbor lookup, ownership and color assignment,
 * and overall grid initialization.
 *
 * The cube coordinate system ensures consistent hex-grid relationships via the constraint:
 * \f$x + y + z = 0\f$.
 *
 * @see CubeCoord, Tile, Company
 *
 * @date 2025-11-06
 * @version 1.0
 * @autho
 * Owen Chilson
 */

#pragma once
#include "CubeCoord.hpp"
#include "Tile.hpp"
#include "Company.hpp"
#include <unordered_map>
#include <vector>

/**
 * @class Board
 * @brief Represents a hexagonal game board composed of cube-coordinate tiles.
 *
 * @details
 * The Board is implemented as an unordered map keyed by CubeCoord, enabling
 * constant-time lookup of tiles. Each tile may store its owner (Company) and
 * visual color data. The board supports neighbor detection and manipulation of
 * tile attributes.
 *
 * The design is suitable for strategy or simulation games utilizing hex grids.
 */
class Board {
public:
    /**
     * @brief Constructs a new hexagonal board with the given radius.
     *
     * @param[in] radius The radius of the hex grid (number of tiles from center to edge).
     * @pre radius > 0
     * @post Initializes a complete set of valid cube coordinates within the given radius.
     */
    Board(int radius);

    /** 
     * @brief Map of cube coordinates to their corresponding Tile instances. 
     *
     * @details
     * The unordered_map allows efficient random access and modification of tiles.
     * @see CubeCoord, Tile
     */
    std::unordered_map<CubeCoord, Tile, CubeCoordHash> tiles;

    /**
     * @brief Retrieves a pointer to the tile at the specified cube coordinate.
     *
     * @param[in] coord The cube coordinate to locate.
     * @return Pointer to the Tile if it exists, or nullptr if out of bounds.
     * @pre The coordinate must satisfy \f$x + y + z = 0\f$ for valid hex geometry.
     * @post Does not modify board state.
     */
    Tile* getTile(const CubeCoord& coord);

    /**
     * @brief Returns all valid neighboring coordinates of the specified tile.
     *
     * @param[in] coord The cube coordinate for which to find adjacent tiles.
     * @return Vector of CubeCoord objects representing neighboring tiles.
     * @post The returned vector contains only coordinates that exist within the board radius.
     * @see CubeCoord
     */
    std::vector<CubeCoord> getNeighbors(const CubeCoord& coord) const;

    /**
     * @brief Assigns a company as the owner of a tile at the given coordinates.
     *
     * @param[in] x Cube X-coordinate.
     * @param[in] y Cube Y-coordinate.
     * @param[in] z Cube Z-coordinate.
     * @param[in,out] company Pointer to the Company to assign as owner.
     * @pre The specified coordinate must exist on the board.
     * @post The tile’s ownership is updated.
     * @warning Does nothing if the coordinate is invalid.
     */
    void setTileOwner(int x, int y, int z, Company* company);

    /**
     * @brief Assigns a color to the tile at the specified coordinates.
     *
     * @param[in] x Cube X-coordinate.
     * @param[in] y Cube Y-coordinate.
     * @param[in] z Cube Z-coordinate.
     * @param[in] color String representation of the desired color.
     * @pre The tile at (x, y, z) must exist and the color string must be valid.
     * @post The tile’s color is updated.
     * @warning Prints an error if the tile does not exist or color is invalid.
     * @see Colors::isValid()
     */
    void setTileColor(int x, int y, int z, const std::string& color);

    /**
     * @brief Prints a visual or textual representation of the board.
     *
     * @details
     * Implementation-defined visualization; typically for debugging or display.
     * May include coordinates, colors, or ownership data.
     * @post Does not modify the board state.
     */
    void printBoard() const;

private:
    /** @brief The board’s radius, defining its geometric bounds. */
    int radius;

    /**
     * @brief Populates the board with valid cube-coordinate tiles.
     *
     * @details
     * Iterates through cube coordinates satisfying \f$|x|, |y|, |z| \le radius\f$
     * and inserts corresponding Tile instances into the tile map.
     *
     * @post The board is filled with all valid coordinates for the defined radius.
     */
    void generateBoard();
};

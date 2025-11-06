/**
 * @file CubeCoord.hpp
 * @brief Defines cube coordinates and hashing utilities for hex-grid operations.
 *
 * This file provides the CubeCoord structure used to represent positions in a
 * 3D cube coordinate system (commonly used for hexagonal grids) and a custom hash
 * functor to enable their use as keys in unordered containers.
 */

#pragma once
#include <functional>

/**
 * @struct CubeCoord
 * @brief Represents a position in a cube coordinate system.
 *
 * Each cube coordinate satisfies x + y + z = 0 in a hex grid context.
 * Provides equality and addition operators for coordinate manipulation.
 */
struct CubeCoord {
    int x; ///< X-axis coordinate.
    int y; ///< Y-axis coordinate.
    int z; ///< Z-axis coordinate.

    /**
     * @brief Constructs a CubeCoord with the specified coordinates.
     * @param x_ The x coordinate.
     * @param y_ The y coordinate.
     * @param z_ The z coordinate.
     */
    CubeCoord(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}

    /**
     * @brief Compares two CubeCoords for equality.
     * @param other The coordinate to compare against.
     * @return True if both coordinates are equal.
     */
    bool operator==(const CubeCoord& other) const;

    /**
     * @brief Adds two CubeCoords component-wise.
     * @param other The coordinate to add.
     * @return The resulting CubeCoord.
     */
    CubeCoord operator+(const CubeCoord& other) const;
};

/**
 * @struct CubeCoordHash
 * @brief Hash functor for CubeCoord, allowing use in unordered containers.
 */
struct CubeCoordHash {
    /**
     * @brief Generates a hash for a CubeCoord.
     * @param c The coordinate to hash.
     * @return The hash value.
     */
    std::size_t operator()(const CubeCoord& c) const noexcept;
};

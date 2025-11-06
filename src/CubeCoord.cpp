#include "CubeCoord.hpp"

/**
 * @brief Compares two CubeCoords for equality.
 * @param other The coordinate to compare against.
 * @return True if all components (x, y, z) are equal.
 */
bool CubeCoord::operator==(const CubeCoord& other) const {
    return x == other.x && y == other.y && z == other.z; ///< Return true only if all coordinates match.
}

/**
 * @brief Adds two CubeCoords component-wise.
 * @param other The coordinate to add.
 * @return New CubeCoord representing the sum of the two coordinates.
 */
CubeCoord CubeCoord::operator+(const CubeCoord& other) const {
    return CubeCoord(x + other.x, y + other.y, z + other.z); ///< Component-wise addition.
}

/**
 * @brief Generates a hash for a CubeCoord to be used in unordered containers.
 * @param c The coordinate to hash.
 * @return Combined hash value for x, y, z.
 *
 * Combines individual hashes using bit shifting to reduce collisions.
 */
std::size_t CubeCoordHash::operator()(const CubeCoord& c) const noexcept {
    std::size_t h1 = std::hash<int>{}(c.x); ///< Hash x component.
    std::size_t h2 = std::hash<int>{}(c.y); ///< Hash y component.
    std::size_t h3 = std::hash<int>{}(c.z); ///< Hash z component.
    return h1 ^ (h2 << 1) ^ (h3 << 2);      ///< Combine hashes with bit shifts.
}

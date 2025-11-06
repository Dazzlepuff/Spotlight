#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <SFML/Graphics/Color.hpp>

/**
 * @class Colors
 * @brief Centralized static utility for color name validation and mapping.
 *
 * Provides a list of all valid color names, their ANSI escape codes for
 * console output, and their corresponding SFML color values for rendering.
 */
class Colors {
public:
    /**
     * @brief Maps color names to their ANSI terminal escape codes.
     * 
     * Example: "Red" → "\033[41m  \033[0m"
     */
    static const std::map<std::string, std::string> colorCodes;

    /**
     * @brief Maps color names to their SFML color equivalents.
     */
    static const std::map<std::string, sf::Color> sfmlColors;

    /**
     * @brief List of all valid color names.
     */
    static const std::vector<std::string> all;

    /**
     * @brief Checks whether a color name is valid.
     * @param color The color name to check.
     * @return True if the color exists in @ref all, false otherwise.
     */
    static bool isValid(const std::string& color);

    /**
     * @brief Retrieves the SFML color object associated with a color name.
     * @param color The color name to retrieve.
     * @return A reference to the matching sf::Color, or the "Neutral" color if not found.
     */
    static const sf::Color& getSfmlColor(const std::string& color);
};

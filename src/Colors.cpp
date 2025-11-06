#include "Colors.hpp"

// Full list of recognized color names.
const std::vector<std::string> Colors::all = {
    "Red", "Yellow", "Blue", "Green", "Purple", "White", "Gray", "Neutral"
};

// Console-friendly ANSI color codes for quick visualization.
const std::map<std::string, std::string> Colors::colorCodes = {
    {"Red",     "\033[41m  \033[0m"},
    {"Yellow",  "\033[43m  \033[0m"},
    {"Blue",    "\033[44m  \033[0m"},
    {"Green",   "\033[42m  \033[0m"},
    {"Purple",  "\033[45m  \033[0m"},
    {"White",   "\033[47m  \033[0m"},
    {"Gray",    "\033[100m  \033[0m"},
    {"Neutral", "\033[0m..."}
};

// SFML color equivalents for in-game rendering.
const std::map<std::string, sf::Color> Colors::sfmlColors = {
    {"Red",     sf::Color(0xD9, 0x7B, 0x66)},
    {"Yellow",  sf::Color(0xE3, 0xC5, 0x67)},
    {"Blue",    sf::Color(0x6C, 0x8E, 0xBF)},
    {"Green",   sf::Color(0x7C, 0xA9, 0x82)},
    {"Purple",  sf::Color(0xA8, 0x8E, 0xC6)},
    {"White",   sf::Color(0xF2, 0xE9, 0xE4)},
    {"Gray",    sf::Color(0xB0, 0xA8, 0xB9)},
    {"Neutral", sf::Color(0x4B, 0x4A, 0x54)}
};

// Returns true if the color string exists in Colors::all.
bool Colors::isValid(const std::string& color) {
    return std::find(all.begin(), all.end(), color) != all.end();
}

// Returns the matching SFML color, or Neutral as a fallback.
const sf::Color& Colors::getSfmlColor(const std::string& color) {
    auto it = sfmlColors.find(color);
    if (it != sfmlColors.end())
        return it->second;
    return sfmlColors.at("Neutral");
}

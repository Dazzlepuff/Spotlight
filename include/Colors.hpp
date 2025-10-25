#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <SFML/Graphics/Color.hpp>

class Colors {
public:
    static const std::map<std::string, std::string> colorCodes;
    static const std::map<std::string, sf::Color> sfmlColors;

    static const std::vector<std::string> all;
    static bool isValid(const std::string& color);
    static const sf::Color& getSfmlColor(const std::string& color);
};

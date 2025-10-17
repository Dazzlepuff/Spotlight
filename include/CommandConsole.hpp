#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Board.hpp"

class CommandConsole {
public:
    CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos = {10.f, 1100.f});

    void handleEvent(const sf::Event& e);
    void draw(sf::RenderWindow& window);

private:
    Board& board;
    sf::Font& font;
    sf::Vector2f position;
    std::string buffer;
    sf::Text text;

    void processCommand(const std::string& cmd);
};

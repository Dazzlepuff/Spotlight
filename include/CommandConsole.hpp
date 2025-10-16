#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Board.hpp"

class CommandConsole {
public:
    CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos = {10.f, 550.f});

    // Call this from the SFML event loop
    void handleEvent(const sf::Event& e);

    // Call this in your render loop
    void draw(sf::RenderWindow& window);

private:
    Board& board;
    sf::Font& font;
    sf::Vector2f position;
    std::string buffer;
    sf::Text text;

    // Process the command string
    void processCommand(const std::string& cmd);
};

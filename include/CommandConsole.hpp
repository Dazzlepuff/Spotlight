#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <queue>
#include "Board.hpp"

class CommandConsole {
public:
    CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos = {20.f, 900.f});

    void draw(sf::RenderWindow& window);

    void handleEvent(const sf::Event& e);
    void print(const std::string& line);

    bool hasCommand() const { return !pendingCommands.empty(); }
    std::string nextCommand();

private:
    Board& board;
    sf::Font& font;
    sf::Text text;
    std::string buffer;
    std::queue<std::string> pendingCommands;
    sf::Vector2f position;

    std::vector<std::string> outputLines;
    const size_t maxLines = 10;
};

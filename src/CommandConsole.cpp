#include "CommandConsole.hpp"
#include <cctype>

CommandConsole::CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos)
    : board(b), font(f), position(pos)
{
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(position);
}

void CommandConsole::handleEvent(const sf::Event& e) {
    if (e.type == sf::Event::TextEntered) {
        char c = static_cast<char>(e.text.unicode);

        if (c == '\r' || c == '\n') {
            processCommand(buffer);
            buffer.clear();
        } else if (c == 8 && !buffer.empty()) {
            buffer.pop_back();
        } else if (std::isprint(c)) {
            buffer += c;
        }
    }
}

void CommandConsole::draw(sf::RenderWindow& window) {
    text.setString("> " + buffer);
    window.draw(text);
}

void CommandConsole::processCommand(const std::string& cmd) {
    std::istringstream ss(cmd);
    std::string action;
    ss >> action;

    if (action == "color") {
        int x, y, z;
        std::string color;
        ss >> x >> y >> z >> color;

        if (!ss.fail()) {
            board.setTileColor(x, y, z, color);
        }
    }
    //Move this to game. Makes more sense for game to hold the Renderer, Command Console, Board, players company, etc...
    //and it also should handle all the command execution.
}

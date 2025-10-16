#include "CommandConsole.hpp"
#include <cctype> // for isprint

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

        if (c == '\r' || c == '\n') { // Enter key
            processCommand(buffer);
            buffer.clear();
        } else if (c == 8 && !buffer.empty()) { // Backspace
            buffer.pop_back();
        } else if (std::isprint(c)) { // Printable characters
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

    // TODO: add more commands here as needed
}

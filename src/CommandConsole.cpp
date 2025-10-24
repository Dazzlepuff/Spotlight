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

void CommandConsole::draw(sf::RenderWindow& window) {
    float y = position.y;

    for (auto it = outputLines.rbegin(); it != outputLines.rend(); ++it) {
        const auto& line = *it;
        y -= 25.f;
        text.setString(line);
        text.setPosition(position.x, y);
        window.draw(text);
    }

    text.setString(">" + buffer);
    text.setPosition(position.x, position.y);
    window.draw(text);
}


void CommandConsole::handleEvent(const sf::Event& e) {
    if (e.type == sf::Event::TextEntered) {
        if (e.text.unicode == '\b') {
            if (!buffer.empty()) buffer.pop_back();
        } else if (e.text.unicode == '\r') {
            if (!buffer.empty()) {
                pendingCommands.push(buffer);
                buffer.clear();
            }
        } else if (e.text.unicode >= 32 && e.text.unicode < 127) {
            buffer += static_cast<char>(e.text.unicode);
        }

        text.setString("> " + buffer);
    }
}

void CommandConsole::print(const std::string& line) {
    outputLines.push_back(line);
    if (outputLines.size() > maxLines)
        outputLines.erase(outputLines.begin());
}

std::string CommandConsole::nextCommand() {
    std::string cmd = pendingCommands.front();
    pendingCommands.pop();
    return cmd;
}

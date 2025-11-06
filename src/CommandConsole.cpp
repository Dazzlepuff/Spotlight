#include "CommandConsole.hpp"
#include <cctype>
#include <algorithm>

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

    text.setString("> " + buffer);
    text.setPosition(position.x, position.y);
    window.draw(text);
}

void CommandConsole::handleEvent(const sf::Event& e) {
    if (e.type == sf::Event::KeyPressed) {
        // ↑ arrow → previous command
        if (e.key.code == sf::Keyboard::Up) {
            if (!commandHistory.empty()) {
                if (historyIndex == -1)
                    historyIndex = static_cast<int>(commandHistory.size()) - 1;
                else if (historyIndex > 0)
                    historyIndex--;
                buffer = commandHistory[historyIndex];
            }
        }

        // ↓ arrow → next command
        else if (e.key.code == sf::Keyboard::Down) {
            if (!commandHistory.empty() && historyIndex != -1) {
                if (historyIndex < static_cast<int>(commandHistory.size()) - 1)
                    historyIndex++;
                else
                    historyIndex = -1; // clear to new entry
                buffer = (historyIndex == -1 ? "" : commandHistory[historyIndex]);
            }
        }
    }

    if (e.type == sf::Event::TextEntered) {
        if (e.text.unicode == '\b') { // Backspace
            if (!buffer.empty()) buffer.pop_back();
        } 
        else if (e.text.unicode == '\r') { // Enter
            if (!buffer.empty()) {
                pendingCommands.push(buffer);
                commandHistory.push_back(buffer);
                historyIndex = -1;
                buffer.clear();
            }
        } 
        else if (e.text.unicode >= 32 && e.text.unicode < 127) { // Regular character
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

void CommandConsole::printPaged(const std::vector<std::string>& lines) {
    pagedBuffer = lines;
    pageIndex = 0;
    awaitingNextPage = false;
    showNextPage();
}

void CommandConsole::showNextPage() {
    if (pagedBuffer.empty()) {
        print("Nothing to show.");
        return;
    }

    size_t end = std::min(pageIndex + 10, pagedBuffer.size());
    for (size_t i = pageIndex; i < end; ++i)
        print(pagedBuffer[i]);

    pageIndex = end;

    if (pageIndex < pagedBuffer.size()) {
        print("--- More (type 'next' to continue) ---");
        awaitingNextPage = true;
    } else {
        awaitingNextPage = false;
        pagedBuffer.clear();
    }
}

std::string CommandConsole::nextCommand() {
    std::string cmd = pendingCommands.front();
    pendingCommands.pop();
    return cmd;
}

void CommandConsole::clear() {
    outputLines.clear();
}
#include "CommandConsole.hpp"
#include <cctype>
#include <algorithm>

// Initialize text rendering and reference data.
CommandConsole::CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos)
    : board(b), font(f), position(pos)
{
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(position);
}

// Draw console history and input prompt.
void CommandConsole::draw(sf::RenderWindow& window) {
    float y = position.y;

    for (auto it = outputLines.rbegin(); it != outputLines.rend(); ++it) {
        const auto& line = *it;
        y -= 25.f;  // line spacing
        text.setString(line);
        text.setPosition(position.x, y);
        window.draw(text);
    }

    text.setString("> " + buffer);
    text.setPosition(position.x, position.y);
    window.draw(text);
}

// Process key events for input, history navigation, and command submission.
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
        else if (e.text.unicode >= 32 && e.text.unicode < 127) { // Printable ASCII
            buffer += static_cast<char>(e.text.unicode);
        }

        text.setString("> " + buffer);
    }
}

// Append a single line of text to output, removing oldest when full.
void CommandConsole::print(const std::string& line) {
    outputLines.push_back(line);
    if (outputLines.size() > maxLines)
        outputLines.erase(outputLines.begin());
}

// Begin a new paged output sequence.
void CommandConsole::printPaged(const std::vector<std::string>& lines) {
    pagedBuffer = lines;
    pageIndex = 0;
    awaitingNextPage = false;
    showNextPage();
}

// Display the next set of lines in a paginated output sequence.
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

// Fetch and remove the next pending command.
std::string CommandConsole::nextCommand() {
    std::string cmd = pendingCommands.front();
    pendingCommands.pop();
    return cmd;
}

// Clear all visible console output.
void CommandConsole::clear() {
    outputLines.clear();
}

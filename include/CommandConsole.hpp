#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <string>
#include "Board.hpp"

class CommandConsole {
public:
    CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& e);
    void print(const std::string& line);
    void printPaged(const std::vector<std::string>& lines);
    void showNextPage();
    std::string nextCommand();
    void clear();

    bool hasCommand() const { return !pendingCommands.empty(); }

    bool awaitingNextPage = false;

private:
    Board& board;
    sf::Font& font;
    sf::Text text;
    sf::Vector2f position;

    std::string buffer;
    std::vector<std::string> outputLines;
    std::queue<std::string> pendingCommands;

    // Paging
    std::vector<std::string> pagedBuffer;
    size_t pageIndex = 0;

    // History
    std::vector<std::string> commandHistory;
    int historyIndex = -1;

    // Config
    const size_t maxLines = 10;
};

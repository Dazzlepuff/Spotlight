#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Tile.hpp"

class CommandConsole;

class Renderer {
public:
    Renderer(Board& board, sf::Font& font);

    void handleEvents(sf::RenderWindow& window, CommandConsole& console);
    void render(sf::RenderWindow& window, CommandConsole& console);

private:
    void drawBoard(sf::RenderWindow& window);

    Board& board;
    sf::Font& font;
};

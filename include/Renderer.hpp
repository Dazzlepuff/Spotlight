#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "CommandConsole.hpp"

class Renderer {
public:
    Renderer(Board& board, sf::Font& font, int windowWidth = 800, int windowHeight = 600);

    void run();  // Main render loop

private:
    Board& board;
    sf::RenderWindow window;
    sf::Font& font;    
    CommandConsole console;  // <-- New console member

    void drawBoard();
    void handleEvents();
};

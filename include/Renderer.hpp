#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"

class Renderer {
public:
    Renderer(Board& board, int windowWidth = 800, int windowHeight = 600);

    void run();  // Main render loop

private:
    Board& board;
    sf::RenderWindow window;

    void drawBoard();
    void handleEvents();
};

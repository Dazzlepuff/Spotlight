/**
 * @file Renderer.hpp
 * @brief Declares the Renderer class responsible for drawing the game board and handling SFML rendering.
 *
 * Handles visual updates of the board, tiles, and console interface within the main SFML window.
 */

#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Tile.hpp"

class CommandConsole;

/**
 * @class Renderer
 * @brief Handles all visual rendering and window event management for the game.
 *
 * The Renderer is responsible for drawing tiles, board layout, and console output to the SFML window.
 */
class Renderer {
public:
    /**
     * @brief Constructs a Renderer object.
     * @param board Reference to the Board instance to render.
     * @param font Reference to the font used for tile text and console output.
     */
    Renderer(Board& board, sf::Font& font);

    /**
     * @brief Processes SFML window events and forwards input to the console.
     * @param window The render window to poll for events.
     * @param console Reference to the CommandConsole for input handling.
     */
    void handleEvents(sf::RenderWindow& window, CommandConsole& console);

    /**
     * @brief Draws all visible elements, including the board and console.
     * @param window The render window where graphics are drawn.
     * @param console Reference to the console for on-screen text rendering.
     */
    void render(sf::RenderWindow& window, CommandConsole& console);

private:
    /**
     * @brief Renders all hex tiles on the board, their colors, owners, and coordinates.
     * @param window The render window used for drawing.
     */
    void drawBoard(sf::RenderWindow& window);

    /** @brief Reference to the game board instance being rendered. */
    Board& board;

    /** @brief Font used for rendering symbols and coordinate labels. */
    sf::Font& font;
};

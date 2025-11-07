/**
 * @file CommandConsole.hpp
 * @brief Declares the CommandConsole class which allows for user IO.
 *
 * @details
 * A key factor in the functionality of the program at whole is allowing
 * the user to interact and receive output. This is done through the command
 * console which only handles IO and passes all commands to the Game class to
 * process.
 *
 * @see Board, Renderer, Game
 *
 * @date 2025-11-06
 * @version 1.0
 * @author
 * Owen Chilson
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <string>
#include "Board.hpp"

/**
 * @class CommandConsole
 * @brief In-game text console handling user command input, output history, and paging.
 *
 * The CommandConsole manages user input through keyboard events, maintains command history,
 * supports paginated output, and displays text using SFML rendering.
 */
class CommandConsole {
public:
    /**
     * @brief Constructs a command console linked to a board.
     * @param b Reference to the Board this console can interact with.
     * @param f Reference to the SFML font used for text rendering.
     * @param pos Screen position where the console text begins.
     */
    CommandConsole(Board& b, sf::Font& f, sf::Vector2f pos);

    /**
     * @brief Renders the console contents to the window.
     * @param window Reference to the SFML render window.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Handles keyboard input events to build commands and navigate history.
     * @param e The SFML event to process.
     */
    void handleEvent(const sf::Event& e);

    /**
     * @brief Prints a single line of text to the console output.
     * @param line Text line to display.
     */
    void print(const std::string& line);

    /**
     * @brief Displays multiple lines of text using a paginated system.
     * @param lines A vector of lines to show, split into multiple pages if necessary.
     */
    void printPaged(const std::vector<std::string>& lines);

    /**
     * @brief Advances to and displays the next page of paginated text, if available.
     */
    void showNextPage();

    /**
     * @brief Retrieves the next user-entered command.
     * @return The command string at the front of the pending command queue.
     */
    std::string nextCommand();

    /**
     * @brief Clears all console output lines.
     */
    void clear();

    /**
     * @brief Returns whether there are commands pending to process.
     * @return True if one or more commands are waiting in the queue.
     */
    bool hasCommand() const { return !pendingCommands.empty(); }

    bool awaitingNextPage = false;  /**< True if more paginated output remains. */

private:
    Board& board;                       /**< Reference to the active Board instance. */
    sf::Font& font;                     /**< Font used for rendering console text. */
    sf::Text text;                      /**< SFML text object for on-screen display. */
    sf::Vector2f position;              /**< Screen position for text rendering. */

    std::string buffer;                 /**< Current text being typed by the player. */
    std::vector<std::string> outputLines; /**< Lines currently displayed on screen. */
    std::queue<std::string> pendingCommands; /**< Commands awaiting execution. */

    // Paging
    std::vector<std::string> pagedBuffer; /**< Lines awaiting paginated display. */
    size_t pageIndex = 0;                 /**< Current index in paged output. */

    // History
    std::vector<std::string> commandHistory; /**< Past entered commands for recall. */
    int historyIndex = -1;                  /**< Index into command history (-1 = none). */

    // Config
    const size_t maxLines = 10;             /**< Maximum number of visible console lines. */
};

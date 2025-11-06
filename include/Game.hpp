/**
 * @file Game.hpp
 * @brief Core game engine managing turn-based hex board gameplay with card and resource mechanics.
 * @details
 * The Game class orchestrates all gameplay systems including player management, turn sequencing,
 * resource economy, card interactions, and command-driven game state manipulation. It integrates
 * rendering, user input via console commands, and maintains the game loop.
 * 
 * @author [Your Name/Team]
 * @date [Current Date]
 * @version 1.0
 */

#pragma once
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Board.hpp"
#include "Company.hpp"
#include "Deck.hpp"

class Renderer;
class CommandConsole;

/**
 * @class Game
 * @brief Central game controller managing all gameplay mechanics and state.
 * @details
 * Game acts as the orchestrator for a turn-based hex board strategy game. It maintains:
 * - Game state (current day, active player)
 * - Player roster and their associated companies
 * - Hex board and tile ownership
 * - Card decks (draw and discard)
 * - Resource economy and transactions
 * - Command-based interface for game actions
 * - Rendering and event handling via SFML
 * 
 * The class follows a command pattern for user interactions, mapping string commands
 * to handler functions that manipulate game state. This design allows for scripting,
 * replay systems, and network play extensions.
 * 
 * @note The game uses a -1 convention for playerIndex to indicate "current active player"
 * @see Player, Board, Company, Deck
 */
class Game {
public:
    /**
     * @brief Constructs a new Game instance with specified board and company configuration.
     * @details
     * Initializes the game environment including:
     * - Hexagonal board of given size
     * - SFML rendering window (1600x1200)
     * - Font loading from assets
     * - Renderer and command console subsystems
     * - Company roster for player assignment
     * - Command handler registration
     * 
     * @param[in] boardSize Radius of the hexagonal board (number of rings from center)
     * @param[in] companyList Vector of available companies for player assignment
     * 
     * @pre boardSize must be positive
     * @pre companyList should not be empty for meaningful gameplay
     * @post Game object ready for player addition via addPlayer()
     * @post Window created but not yet displayed
     * 
     * @warning Exits with error message if font file "consolas.ttf" cannot be loaded
     * @see addPlayer(), setup()
     */
    Game(int boardSize, std::vector<Company> companyList);
    
    /**
     * @brief Destructs the Game instance, releasing dynamically allocated resources.
     * @details Ensures proper cleanup of renderer and console subsystems to prevent memory leaks.
     * @post All heap-allocated members freed
     */
    ~Game();
    
    /**
     * @brief Registers a new player with their associated company.
     * @details
     * Adds a player to the game roster. Players are assigned turn order based on
     * registration sequence. The company pointer establishes ownership relationships
     * for tiles built by this player.
     * 
     * @param[in] name Display name for the player
     * @param[in] company Pointer to the company this player represents (must outlive Game)
     * 
     * @pre company pointer must be valid and point to an existing Company object
     * @pre Must be called before setup()
     * @post Player added to internal roster with zero-initialized resources
     * 
     * @note Player index is implicitly assigned based on insertion order
     * @see setup(), getCurrentActivePlayerIndex()
     */
    void addPlayer(const std::string& name, Company* company);
    
    /**
     * @brief Initializes the game board and deals starting cards.
     * @details
     * Performs initial game setup:
     * - Randomly assigns colors and owners to half the board tiles
     * - Sets remaining tiles to neutral (unowned)
     * - Loads card deck from "cards.json"
     * - Shuffles the draw deck
     * - Deals one card to the first player
     * 
     * This method implements the game's starting conditions as per design rules.
     * 
     * @pre At least one player must be added via addPlayer()
     * @pre "cards.json" must exist and contain valid card definitions
     * @post Board tiles initialized with random colors and owners
     * @post Draw and discard decks created
     * @post First player has one card in hand
     * 
     * @warning Neutral color (index 0 in Colors::all) is excluded from random selection
     * @see addPlayer(), mainLoop()
     */
    void setup();
    
    /**
     * @brief Enters the main game loop, processing events and rendering until window closes.
     * @details
     * Core game loop that:
     * 1. Handles window events (close, input) via renderer
     * 2. Processes queued console commands via executeCommand()
     * 3. Renders the current game state (board, console output)
     * 
     * This loop runs indefinitely until the SFML window is closed by the user.
     * 
     * @pre setup() must have been called to initialize game state
     * @post Window closed and game terminated
     * 
     * @note Blocks execution until game exit
     * @see executeCommand(), Renderer::handleEvents(), Renderer::render()
     */
    void mainLoop();

    /**
     * @brief Retrieves the immutable list of all registered players.
     * @details Provides read-only access to the player roster for querying game state.
     * @return Const reference to the internal player vector
     * @see Player
     */
    const std::vector<Player>& getPlayers() const;

    /**
     * @brief Executes start-of-day triggers for all played cards across all players.
     * @details
     * Iterates through each player's active (played) cards and invokes their
     * "onStartOfDay" trigger. This implements card effects that activate at the
     * beginning of each game day (round).
     * 
     * @post All cards' onStartOfDay effects executed
     * @note Called automatically when all players end their turn (day transition)
     * @see endTurn(), Card::executeTrigger()
     */
    void startNewDay();

    /**
     * @brief Draws specified number of cards from a deck into a player's hand.
     * @details
     * Transfers cards from the given deck to the player's held cards. Logs each
     * draw to the console. Stops early if deck is exhausted.
     * 
     * @param[in,out] deck Deck to draw from (modified by removing cards)
     * @param[in,out] player Player receiving the cards (hand is expanded)
     * @param[in] amount Number of cards to attempt drawing
     * 
     * @pre deck must be a valid Deck reference
     * @pre player must be a valid Player reference
     * @post Up to 'amount' cards transferred from deck to player hand
     * @post Console messages logged for each drawn card
     * 
     * @note If deck empties mid-draw, stops and logs a message
     * @see Deck::drawCard(), Player::addHeldCard()
     */
    void drawCardForPlayer(Deck& deck, Player& player, int amount);
    
    /**
     * @brief Grants a specified resource quantity to a player.
     * @details
     * Increases the player's resource pool for the given resource type. If the
     * resource doesn't exist in the player's inventory, initializes it to the given amount.
     * 
     * @param[in] playerIndex Index of target player (-1 for current active player)
     * @param[in] resource Name of the resource (e.g., "gold", "wood", "influence")
     * @param[in] amount Quantity to add (can be negative to subtract)
     * @param[in] logToConsole Whether to print transaction to console (default: true)
     * 
     * @pre playerIndex must be valid or -1
     * @post Player's resource pool increased by amount
     * @post Console message logged if logToConsole is true
     * 
     * @note Resources are string-keyed and dynamically created on first use
     * @see spendResourceFromPlayer()
     */
    void giveResourceToPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);
    
    /**
     * @brief Attempts to deduct a resource quantity from a player's inventory.
     * @details
     * Validates that the player has sufficient resources before deducting. If insufficient,
     * logs an error and returns false without modifying the player's inventory.
     * 
     * @param[in] playerIndex Index of target player (-1 for current active player)
     * @param[in] resource Name of the resource to spend
     * @param[in] amount Quantity to deduct
     * @param[in] logToConsole Whether to print transaction/errors to console (default: true)
     * 
     * @return true if transaction succeeded, false if insufficient resources
     * 
     * @pre playerIndex must be valid or -1
     * @post If successful, player's resource reduced by amount
     * @post Console message logged if logToConsole is true
     * 
     * @warning Returns false if resource doesn't exist in player's inventory
     * @see giveResourceToPlayer()
     */
    bool spendResourceFromPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);
    
    /**
     * @brief Constructs a stage (building) on a hex tile with specified color and ownership.
     * @details
     * Changes tile ownership to the player's company and sets the tile's color. This is
     * the primary mechanism for claiming territory and scoring points in the game.
     * 
     * @param[in] playerIndex Index of the player performing the build action
     * @param[in] x X-coordinate of the target hex tile (cube coordinates)
     * @param[in] y Y-coordinate of the target hex tile (cube coordinates)
     * @param[in] z Z-coordinate of the target hex tile (cube coordinates)
     * @param[in] color Color name for the stage (must be valid per Colors::all)
     * 
     * @pre playerIndex must be valid (not -1)
     * @pre Coordinates must reference an existing tile on the board
     * @pre color must be a recognized color string
     * @post Tile ownership transferred to player's company
     * @post Tile color updated
     * 
     * @note Does not validate color or coordinates; caller must ensure validity
     * @see Board::setTileOwner(), Board::setTileColor()
     */
    void buildStage(int playerIndex, int x, int y, int z, const std::string& color);
    
    /**
     * @brief Advances to the next player's turn, or starts a new day if all players finished.
     * @details
     * Increments the active player index. If the last player in turn order finishes,
     * wraps to player 0 and increments the current day counter, triggering start-of-day effects.
     * 
     * @param[in] logToConsole Whether to log turn transitions to console (default: true)
     * 
     * @post currentActivePlayerIndex advanced (or wrapped to 0)
     * @post If day boundary crossed, currentDay incremented and startNewDay() called
     * @post Console messages logged if logToConsole is true
     * 
     * @see startNewDay(), getCurrentActivePlayerIndex()
     */
    void endTurn(bool logToConsole = true);
    
    /**
     * @brief Plays a card from a player's hand into their active play area.
     * @details
     * Validates the player has the card in hand, transfers it to played cards, and
     * executes the card's "onPlay" trigger immediately.
     * 
     * @param[in] playerIndex Index of the player (-1 for current active player)
     * @param[in] cardName Exact name of the card to play
     * @param[in] logToConsole Whether to log action/errors to console (default: true)
     * 
     * @return true if card successfully played, false if not found in hand
     * 
     * @pre playerIndex must be valid or -1
     * @post If successful, card moved from hand to playedCards
     * @post Card's onPlay trigger executed
     * @post Console message logged if logToConsole is true
     * 
     * @see removePlayedCardForPlayer(), Player::playCard()
     */
    bool playCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole = true);
    
    /**
     * @brief Removes a card from a player's active play area (discards it).
     * @details
     * Used for card destruction, end-of-turn cleanup, or card replacement mechanics.
     * 
     * @param[in] playerIndex Index of the player (-1 for current active player)
     * @param[in] cardName Exact name of the card to remove
     * @param[in] logToConsole Whether to log action/errors to console (default: true)
     * 
     * @return true if card found and removed, false if not in play
     * 
     * @pre playerIndex must be valid or -1
     * @post If successful, card removed from playedCards
     * @post Console message logged if logToConsole is true
     * 
     * @see playCardForPlayer(), Player::removePlayedCard()
     */
    bool removePlayedCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole = true);
    
    /**
     * @brief Removes a card directly from a player's hand (discards without playing).
     * @details
     * Used for discard mechanics, hand management, or card destruction effects.
     * 
     * @param[in] playerIndex Index of the player (-1 for current active player)
     * @param[in] cardName Exact name of the card to remove
     * @param[in] logToConsole Whether to log action/errors to console (default: true)
     * 
     * @return true if card found and removed, false if not in hand
     * 
     * @pre playerIndex must be valid or -1
     * @post If successful, card removed from heldCards
     * @post Console message logged if logToConsole is true
     * 
     * @see playCardForPlayer(), Player::removeHeldCard()
     */
    bool removeHeldCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole = true);

    /**
     * @brief Retrieves the current game day (round number).
     * @return Integer representing the current day, starting from 0
     * @see startNewDay(), endTurn()
     */
    int getCurrentDay();
    
    /**
     * @brief Retrieves the index of the player whose turn is currently active.
     * @return Integer index into the players vector (0-based)
     * @see endTurn(), validateAndSetPlayerIndex()
     */
    int getCurrentActivePlayerIndex();

private:
    /**
     * @brief Parses and dispatches a command string to the appropriate handler function.
     * @details
     * Splits the command into an action keyword and arguments, then invokes the
     * corresponding lambda from commandHandlers map. Logs error for unknown commands.
     * 
     * @param[in] cmd Complete command string (e.g., "draw_card drawDeck 3")
     * 
     * @post Corresponding handler executed if command recognized
     * @post Error logged to console if command unknown
     * 
     * @see initializeCommandHandlers()
     */
    void executeCommand(const std::string& cmd);
    
    /**
     * @brief Populates the commandHandlers map with all available game commands.
     * @details
     * Registers lambda functions for each command keyword. This centralized initialization
     * ensures all commands are available at game start and simplifies command addition.
     * 
     * @post commandHandlers map fully populated with all game commands
     * @see executeCommand()
     */
    void initializeCommandHandlers();
    
    /**
     * @brief Validates a player index and converts -1 to the current active player.
     * @details
     * Implements the convention where -1 represents "current player". Checks bounds
     * and logs errors for invalid indices.
     * 
     * @param[in,out] playerIndex Player index to validate (modified if -1)
     * @param[in] logToConsole Whether to log validation errors (default: true)
     * 
     * @return true if index valid (after conversion), false if out of bounds
     * 
     * @post If playerIndex was -1, replaced with currentActivePlayerIndex
     * @post Error logged if index invalid and logToConsole is true
     * 
     * @note This function modifies the playerIndex parameter by reference
     */
    bool validateAndSetPlayerIndex(int& playerIndex, bool logToConsole = true);
    
    /**
     * @brief Extracts a multi-word card name and optional player index from input stream.
     * @details
     * Handles card names with spaces (e.g., "Ancient Ruins") by treating all tokens
     * as part of the card name except the last token if it's a valid integer (player index).
     * 
     * @param[in,out] ss Input string stream containing command arguments
     * @param[out] cardName Reconstructed card name (spaces included)
     * @param[out] playerIndex Extracted player index, or -1 if not provided
     * 
     * @return true if card name successfully parsed, false if stream empty
     * 
     * @post cardName contains the complete card name
     * @post playerIndex set to extracted value or -1
     * 
     * @note Ambiguous for card names ending in numbers (treated as player index)
     */
    bool parseCardNameWithOptionalPlayerIndex(std::istringstream& ss, std::string& cardName, int& playerIndex);
    
    // ========================================
    // Command Handler Functions
    // ========================================
    // These private methods implement the logic for each console command.
    // They parse arguments from the input stream, validate inputs, and execute
    // the corresponding game action. Most log results to the console.
    // ========================================
    
    /**
     * @brief Handles "set_color" command to change a tile's color directly.
     * @details Parses coordinates and color, validates color, then updates the tile.
     * @param[in] ss Input stream: "<x> <y> <z> <color>"
     * @see Board::setTileColor()
     */
    void handleSetColor(std::istringstream& ss);
    
    /**
     * @brief Handles "set_owner" command to assign tile ownership by company index.
     * @details Parses coordinates and company index, validates bounds, updates tile owner.
     * @param[in] ss Input stream: "<x> <y> <z> <company_index>"
     * @see Board::setTileOwner()
     */
    void handleSetOwner(std::istringstream& ss);
    
    /**
     * @brief Handles "build" command for player-initiated stage construction.
     * @details Validates player, coordinates, and color before calling buildStage().
     * @param[in] ss Input stream: "<x> <y> <z> <color> [player_index]"
     * @see buildStage()
     */
    void handleBuild(std::istringstream& ss);
    
    /**
     * @brief Handles "list_players" command to display all registered players.
     * @details Outputs player names and their associated companies to console.
     */
    void handleListPlayers();
    
    /**
     * @brief Handles "show_resources" command to display a player's resource inventory.
     * @details Iterates through player's resource map and logs each resource type/amount.
     * @param[in] ss Input stream: "[player_index]"
     */
    void handleShowResources(std::istringstream& ss);
    
    /**
     * @brief Handles "show_cards" command to list cards in a player's hand.
     * @details Outputs paginated list of held cards via console's printPaged().
     * @param[in] ss Input stream: "[player_index]"
     */
    void handleShowCards(std::istringstream& ss);
    
    /**
     * @brief Handles "get_card_count" command to report cards remaining in a deck.
     * @details Queries deck by name and logs its current size.
     * @param[in] ss Input stream: "<deck_name>"
     */
    void handleGetCardCount(std::istringstream& ss);
    
    /**
     * @brief Handles "draw_card" command to draw cards from a deck into a player's hand.
     * @details Validates deck exists and player index before calling drawCardForPlayer().
     * @param[in] ss Input stream: "<deck_name> <amount> [player_index]"
     * @see drawCardForPlayer()
     */
    void handleDrawCard(std::istringstream& ss);
    
    /**
     * @brief Handles "give_resource" command to grant resources to a player.
     * @details Parses resource type and amount, then calls giveResourceToPlayer().
     * @param[in] ss Input stream: "<resource> <amount> [player_index]"
     * @see giveResourceToPlayer()
     */
    void handleGiveResource(std::istringstream& ss);
    
    /**
     * @brief Handles "spend_resource" command to deduct resources from a player.
     * @details Parses resource type and amount, then calls spendResourceFromPlayer().
     * @param[in] ss Input stream: "<resource> <amount> [player_index]"
     * @see spendResourceFromPlayer()
     */
    void handleSpendResource(std::istringstream& ss);
    
    /**
     * @brief Handles "play_card" command to play a card from hand.
     * @details Uses parseCardNameWithOptionalPlayerIndex() for multi-word card names.
     * @param[in] ss Input stream: "<card_name> [player_index]"
     * @see playCardForPlayer()
     */
    void handlePlayCard(std::istringstream& ss);
    
    /**
     * @brief Handles "remove_played_card" command to discard a card from play.
     * @details Uses parseCardNameWithOptionalPlayerIndex() for multi-word card names.
     * @param[in] ss Input stream: "<card_name> [player_index]"
     * @see removePlayedCardForPlayer()
     */
    void handleRemovePlayedCard(std::istringstream& ss);
    
    /**
     * @brief Handles "remove_held_card" command to discard a card from hand.
     * @details Uses parseCardNameWithOptionalPlayerIndex() for multi-word card names.
     * @param[in] ss Input stream: "<card_name> [player_index]"
     * @see removeHeldCardForPlayer()
     */
    void handleRemoveHeldCard(std::istringstream& ss);
    
    /**
     * @brief Handles "help" command to display all available commands and their syntax.
     * @details Outputs paginated list of commands with usage examples via console.
     */
    void handleHelp();

    // ========================================
    // Member Variables
    // ========================================
    
    Board board;                      ///< Hexagonal game board containing all tiles
    std::vector<Player> players;      ///< Roster of all registered players in turn order
    std::vector<Company> companies;   ///< Available companies for player assignment
    std::vector<Deck> decks;          ///< Collection of card decks (draw, discard, etc.)
    
    int currentDay = 0;               ///< Current game day (round number), starts at 0
    int currentActivePlayerIndex = 0; ///< Index of player whose turn is active
    
    sf::Font font;                    ///< SFML font resource for text rendering
    sf::RenderWindow window;          ///< SFML window for graphical output (1600x1200)
    Renderer* renderer;               ///< Subsystem for rendering game visuals
    CommandConsole* console;          ///< Subsystem for command input/output UI
    
    /**
     * @brief Map of command strings to their handler functions.
     * @details Populated by initializeCommandHandlers(). Enables O(1) command dispatch
     * and simplifies command addition/removal.
     */
    std::unordered_map<std::string, std::function<void(std::istringstream&)>> commandHandlers;
    
    /**
     * @brief Locates a deck by name within the decks vector.
     * @param[in] deckName Name identifier of the target deck
     * @return Pointer to the deck if found, nullptr otherwise (logs error)
     * @note Used internally to resolve deck names in commands
     */
    Deck* getDeckByName(const std::string& deckName);
};
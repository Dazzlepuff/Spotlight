#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <sstream>

#include "Renderer.hpp"
#include "CommandConsole.hpp"
#include "Colors.hpp"
#include "PathUtils.hpp"

// Exclude neutral color from random selection
constexpr size_t NEUTRAL_COLOR_OFFSET = 1;

Game::Game(int boardSize, std::vector<Company> companyList)
    : board(boardSize),
      window(sf::VideoMode(1600, 1200), "Hex Board"),
      currentDay(0),
      currentActivePlayerIndex(0)
{
    auto fontPath = PathUtils::getAssetPath("consolas.ttf");
    if (!font.loadFromFile(fontPath.string())) {
        std::cerr << "Error: Could not load font at " << fontPath << "\n";
    }

    sf::Vector2f consolePosition(20.f, 1160.f);

    console = new CommandConsole(board, font, consolePosition);
    renderer = new Renderer(board, font);

    companies = companyList;
    
    initializeCommandHandlers();
}

Game::~Game() {
    delete renderer;
    delete console;
}

void Game::addPlayer(const std::string& name, Company* company) {
    players.emplace_back(name, company);
}

void Game::setup() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // -1 to exclude neutral color from random selection
    std::uniform_int_distribution<> colorDist(0, Colors::all.size() - NEUTRAL_COLOR_OFFSET - 1);
    std::uniform_int_distribution<> ownerDist(0, static_cast<int>(players.size()) - 1);

    std::vector<CubeCoord> tileCoords;
    tileCoords.reserve(board.tiles.size());
    for (const auto& [coord, _] : board.tiles) {
        tileCoords.push_back(coord);
    }

    std::shuffle(tileCoords.begin(), tileCoords.end(), gen);

    size_t half = tileCoords.size() / 2;
    for (size_t i = 0; i < half; ++i) {
        auto& tile = board.tiles[tileCoords[i]];
        tile.setColor(Colors::all[colorDist(gen)]);
        tile.setOwner(players[ownerDist(gen)].company);
    }

    for (size_t i = half; i < tileCoords.size(); ++i) {
        auto& tile = board.tiles[tileCoords[i]];
        tile.setColor("Neutral");
        tile.setOwner(nullptr);
    }

    Deck drawDeck("drawDeck");
    drawDeck.loadFromJsonFile("cards.json");
    drawDeck.shuffle();

    Deck discardDeck("discardDeck");

    decks.push_back(std::move(drawDeck));
    decks.push_back(std::move(discardDeck));

    if (!getDeckByName("drawDeck")->empty()) {
        players[0].addHeldCard(getDeckByName("drawDeck")->drawCard());
    }
}

void Game::mainLoop() {
    while (window.isOpen()) {
        renderer->handleEvents(window, *console);

        while (console->hasCommand()) {
            std::string cmd = console->nextCommand();
            executeCommand(cmd);
        }

        renderer->render(window, *console);
    }
}

Deck* Game::getDeckByName(const std::string& deckName) {
    for (auto& deck : decks) {
        if (deck.name == deckName)
            return &deck;
    }
    std::cerr << "Error: Deck '" << deckName << "' not found.\n";
    return nullptr;
}

const std::vector<Player>& Game::getPlayers() const {
    return players;
}

int Game::getCurrentActivePlayerIndex() {
    return currentActivePlayerIndex;
}

int Game::getCurrentDay() {
    return currentDay;
}

void Game::startNewDay() {
    for (auto& player : players) {
        for (auto& card : player.playedCards) {
            card.executeTrigger("onStartOfDay", player);
        }
    }
}

void Game::drawCardForPlayer(Deck& deck, Player& player, int amount) {
    if (deck.empty()) {
        console->print("The deck is empty! No more cards to draw.");
        return;
    }

    for (int i = 0; i < amount && !deck.empty(); ++i) {
        Card drawn = deck.drawCard();
        player.addHeldCard(drawn);
        console->print(player.name + " drew a card: " + drawn.name);
    }
}

void Game::giveResourceToPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole) {
    if (!validateAndSetPlayerIndex(playerIndex, logToConsole))
        return;

    Player& player = players[playerIndex];
    player.resources[resource] += amount;

    if (logToConsole)
        console->print("Gave " + std::to_string(amount) + " " + resource + " to " + player.name + ".");
}

bool Game::spendResourceFromPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole) {
    if (!validateAndSetPlayerIndex(playerIndex, logToConsole))
        return false;

    Player& player = players[playerIndex];
    auto it = player.resources.find(resource);
    if (it == player.resources.end() || it->second < amount) {
        if (logToConsole)
            console->print("Error: Not enough " + resource + " for " + player.name + ".");
        return false;
    }

    it->second -= amount;
    if (logToConsole)
        console->print(player.name + " spent " + std::to_string(amount) + " " + resource + ".");
    return true;
}

void Game::buildStage(int x, int y, int z, const std::string& color, int playerIndex) {
    if (playerIndex == -1)
        playerIndex = getCurrentActivePlayerIndex();
    
    Player& activePlayer = players[playerIndex];

    board.setTileOwner(x, y, z, activePlayer.company);
    board.setTileColor(x, y, z, color);
}

void Game::endTurn(bool logToConsole) {
    console->print(players[currentActivePlayerIndex].name + " ended their turn.");
    currentActivePlayerIndex++;
    console->print(players[currentActivePlayerIndex].name + " starts their turn.");
    if (currentActivePlayerIndex > players.size() - 1) {
        currentActivePlayerIndex = 0;
        currentDay++;
        if (logToConsole)
            console->print("Last player finished turn. Starting day: " + std::to_string(currentDay));
        startNewDay();
    }
}

bool Game::validateAndSetPlayerIndex(int& playerIndex, bool logToConsole) {
    if (playerIndex == -1)
        playerIndex = getCurrentActivePlayerIndex();
    
    if (playerIndex < 0 || playerIndex >= players.size()) {
        if (logToConsole)
            console->print("Error: Player index " + std::to_string(playerIndex) +
                        " is out of range. Max valid index: " + std::to_string(players.size() - 1));
        return false;
    }
    return true;
}

bool Game::parseCardNameWithOptionalPlayerIndex(std::istringstream& ss, std::string& cardName, int& playerIndex) {
    cardName.clear();
    playerIndex = -1;
    
    std::vector<std::string> tokens;
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        return false;
    }
    
    // Check if the last token is a number (player index)
    bool hasPlayerIndex = false;
    if (!tokens.empty()) {
        try {
            playerIndex = std::stoi(tokens.back());
            hasPlayerIndex = true;
            tokens.pop_back();
        } catch (...) {
            // Last token is not a number, it's part of the card name
        }
    }
    
    // Join remaining tokens as card name
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0) cardName += " ";
        cardName += tokens[i];
    }
    
    if (cardName.empty()) {
        return false;
    }
    
    if (!hasPlayerIndex) {
        playerIndex = -1;
    }
    
    return true;
}

void Game::initializeCommandHandlers() {
    commandHandlers["next"] = [this](std::istringstream&) {
        if (console->awaitingNextPage) {
            console->showNextPage();
        } else {
            console->print("No more pages to show.");
        }
    };

    commandHandlers["clear"] = [this](std::istringstream&) {
        console->clear();
    };

    commandHandlers["end_turn"] = [this](std::istringstream&) {
        endTurn(true);
    };

    commandHandlers["set_color"] = [this](std::istringstream& ss) {
        handleSetColor(ss);
    };

    commandHandlers["set_owner"] = [this](std::istringstream& ss) {
        handleSetOwner(ss);
    };

    commandHandlers["build"] = [this](std::istringstream& ss) {
        handleBuild(ss);
    };

    commandHandlers["list_players"] = [this](std::istringstream&) {
        handleListPlayers();
    };

    commandHandlers["show_resources"] = [this](std::istringstream& ss) {
        handleShowResources(ss);
    };

    commandHandlers["show_cards"] = [this](std::istringstream& ss) {
        handleShowCards(ss);
    };

    commandHandlers["get_card_count"] = [this](std::istringstream& ss) {
        handleGetCardCount(ss);
    };

    commandHandlers["draw_card"] = [this](std::istringstream& ss) {
        handleDrawCard(ss);
    };

    commandHandlers["give_resource"] = [this](std::istringstream& ss) {
        handleGiveResource(ss);
    };

    commandHandlers["spend_resource"] = [this](std::istringstream& ss) {
        handleSpendResource(ss);
    };

    commandHandlers["play_card"] = [this](std::istringstream& ss) {
        handlePlayCard(ss);
    };

    commandHandlers["remove_played_card"] = [this](std::istringstream& ss) {
        handleRemovePlayedCard(ss);
    };

    commandHandlers["remove_held_card"] = [this](std::istringstream& ss) {
        handleRemoveHeldCard(ss);
    };

    commandHandlers["help"] = [this](std::istringstream&) {
        handleHelp();
    };
}

void Game::handleSetColor(std::istringstream& ss) {
    int x, y, z;
    std::string color;
    ss >> x >> y >> z >> color;

    if (ss.fail()) {
        console->print("Usage: set_color <x> <y> <z> <color>");
        return;
    }
    
    if (!Colors::isValid(color)) {
        console->print(color + " is not a valid color. Valid colors: ");
        for (const auto& c : Colors::all)
            console->print("   " + c);
        return;
    }

    board.setTileColor(x, y, z, color);
    console->print("Set tile (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ") to " + color);
}

void Game::handleSetOwner(std::istringstream& ss) {
    int x, y, z;
    int companyIndex;
    ss >> x >> y >> z >> companyIndex;

    if (ss.fail()) {
        console->print("Usage: set_owner <x> <y> <z> <company_index>");
        return;
    }
    
    if (companyIndex < 0 || companyIndex >= companies.size()) {
        console->print("Error: Company index " + std::to_string(companyIndex) + 
                      " is out of range. Max valid index: " + std::to_string(companies.size() - 1));
        return;
    }

    board.setTileOwner(x, y, z, &companies[companyIndex]);
    console->print("Set tile (" + std::to_string(x) + ", " + std::to_string(y) + ", " + 
                  std::to_string(z) + ") to " + companies[companyIndex].getName() + 
                  ": " + companies[companyIndex].getSymbol());
}

void Game::handleBuild(std::istringstream& ss) {
    int x, y, z;
    std::string color;
    int playerIndex = -1;

    ss >> x >> y >> z >> color;
    
    if (ss.fail() || color.empty()) {
        console->print("Usage: build <x> <y> <z> <color> [player_index]");
        return;
    }

    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    if (!validateAndSetPlayerIndex(playerIndex))
        return;

    if (!Colors::isValid(color)) {
        console->print(color + " is not a valid color. Valid colors:");
        for (const auto& c : Colors::all)
            console->print("   " + c);
        return;
    }

    buildStage(x, y, z, color, playerIndex);
    console->print(players[playerIndex].name + " (" + players[playerIndex].company->getName() + 
                  ") built a " + color + " stage at " +
                  std::to_string(x) + std::to_string(y) + std::to_string(z));
}

void Game::handleListPlayers() {
    if (players.empty()) {
        console->print("No players available.");
    } else {
        console->print("Players:");
        for (const auto& p : players)
            console->print(" - " + p.name + " (" + p.company->getName() + ": " + p.company->getSymbol() + ")");
    }
}

void Game::handleShowResources(std::istringstream& ss) {
    int playerIndex = -1;
    
    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    if (!validateAndSetPlayerIndex(playerIndex))
        return;

    Player& player = players[playerIndex];
    console->print("Resources for player " + player.name + ":");
    for (const auto& [resource, amount] : player.resources)
        console->print("  " + resource + ": " + std::to_string(amount));
}

void Game::handleShowCards(std::istringstream& ss) {
    int playerIndex = -1;

    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    if (!validateAndSetPlayerIndex(playerIndex))
        return;

    Player& player = players[playerIndex];
    std::vector<std::string> lines;

    lines.push_back("Cards held by " + player.name + ":");
    if (player.heldCards.empty())
        lines.push_back("  (no cards)");
    else
        for (const auto& card : player.heldCards)
            lines.push_back("  - " + card.name);

    console->printPaged(lines);
}

void Game::handleGetCardCount(std::istringstream& ss) {
    std::string deckName;
    ss >> deckName;

    if (ss.fail()) {
        console->print("Usage: get_card_count <deck_name>");
        return;
    }

    Deck* deck = getDeckByName(deckName);
    if (!deck) {
        console->print("Error: Deck '" + deckName + "' not found.");
        return;
    }

    int cardCount = deck->size();
    console->print(deckName + " has " + std::to_string(cardCount) + " card" + (cardCount == 1 ? "" : "s"));
}

void Game::handleDrawCard(std::istringstream& ss) {
    std::string deckName;
    int amount;
    int playerIndex = -1;

    ss >> deckName >> amount;
    
    if (ss.fail() || deckName.empty()) {
        console->print("Usage: draw_card <deck_name> <amount> [player_index]");
        return;
    }

    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    if (!validateAndSetPlayerIndex(playerIndex))
        return;

    Deck* deck = getDeckByName(deckName);
    if (!deck) {
        console->print("Error: Deck '" + deckName + "' not found.");
        return;
    }

    drawCardForPlayer(*deck, players[playerIndex], amount);
}

void Game::handleGiveResource(std::istringstream& ss) {
    std::string resource;
    int amount;
    int playerIndex = -1;

    ss >> resource >> amount;
    
    if (ss.fail() || resource.empty()) {
        console->print("Usage: give_resource <resource> <amount> [player_index]");
        return;
    }

    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    giveResourceToPlayer(playerIndex, resource, amount);
}

void Game::handleSpendResource(std::istringstream& ss) {
    std::string resource;
    int amount;
    int playerIndex = -1;

    ss >> resource >> amount;
    
    if (ss.fail() || resource.empty()) {
        console->print("Usage: spend_resource <resource> <amount> [player_index]");
        return;
    }

    if (!(ss >> playerIndex)) {
        playerIndex = -1;
    }

    spendResourceFromPlayer(playerIndex, resource, amount);
}

void Game::handlePlayCard(std::istringstream& ss) {
    std::string cardName;
    int playerIndex = -1;
    
    if (!parseCardNameWithOptionalPlayerIndex(ss, cardName, playerIndex)) {
        console->print("Usage: play_card <card_name> [player_index]");
        return;
    }
    
    playCardForPlayer(playerIndex, cardName);
}

void Game::handleRemovePlayedCard(std::istringstream& ss) {
    std::string cardName;
    int playerIndex = -1;
    
    if (!parseCardNameWithOptionalPlayerIndex(ss, cardName, playerIndex)) {
        console->print("Usage: remove_played_card <card_name> [player_index]");
        return;
    }
    
    removePlayedCardForPlayer(playerIndex, cardName);
}

void Game::handleRemoveHeldCard(std::istringstream& ss) {
    std::string cardName;
    int playerIndex = -1;
    
    if (!parseCardNameWithOptionalPlayerIndex(ss, cardName, playerIndex)) {
        console->print("Usage: remove_held_card <card_name> [player_index]");
        return;
    }
    
    removeHeldCardForPlayer(playerIndex, cardName);
}

bool Game::playCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole) {
    if (!validateAndSetPlayerIndex(playerIndex, logToConsole))
        return false;

    Player& player = players[playerIndex];
    if (player.playCard(cardName)) {
        auto it = std::find_if(player.playedCards.begin(), player.playedCards.end(),
                               [&](const Card& c) { return c.name == cardName; });
        if (it != player.playedCards.end()) {
            it->executeTrigger("onPlay", player);
        }
        
        if (logToConsole)
            console->print(player.name + " played card: " + cardName);
        return true;
    } else {
        if (logToConsole)
            console->print("Error: " + player.name + " does not have card '" + cardName + "' in hand.");
        return false;
    }
}

bool Game::removePlayedCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole) {
    if (!validateAndSetPlayerIndex(playerIndex, logToConsole))
        return false;

    Player& player = players[playerIndex];
    if (player.removePlayedCard(cardName)) {
        if (logToConsole)
            console->print("Removed played card '" + cardName + "' from " + player.name);
        return true;
    } else {
        if (logToConsole)
            console->print("Error: " + player.name + " does not have card '" + cardName + "' in play.");
        return false;
    }
}

bool Game::removeHeldCardForPlayer(int playerIndex, const std::string& cardName, bool logToConsole) {
    if (!validateAndSetPlayerIndex(playerIndex, logToConsole))
        return false;

    Player& player = players[playerIndex];
    if (player.removeHeldCard(cardName)) {
        if (logToConsole)
            console->print("Removed card '" + cardName + "' from " + player.name + "'s hand.");
        return true;
    } else {
        if (logToConsole)
            console->print("Error: " + player.name + " does not have card '" + cardName + "' in hand.");
        return false;
    }
}

void Game::handleHelp() {
    std::vector<std::string> lines = {
        "Available commands:",
        "  set_color <x> <y> <z> <color>  - Sets the color of a tile.",
        "  set_owner <x> <y> <z> <company_index>  - Assigns tile ownership.",
        "  build <x> <y> <z> <color> [player_index]  - Builds a stage.",
        "  list_players  - Lists all players.",
        "  show_resources [player_index]  - Shows player resources.",
        "  give_resource <resource> <amount> [player_index]  - Gives resources.",
        "  spend_resource <resource> <amount> [player_index]  - Spends resources.",
        "  show_cards [player_index]  - Lists player cards.",
        "  get_card_count <deck_name>  - Shows total amount of cards left in a deck.",
        "  draw_card <deck_name> <amount> [player_index]  - Draws cards for a player.",
        "  play_card <card_name> [player_index]  - Plays a card from hand.",
        "  remove_played_card <card_name> [player_index]  - Removes a card from play.",
        "  remove_held_card <card_name> [player_index]  - Removes a card from hand.",
        "  end_turn  - Ends the current player's turn.",
        "  next  - Shows the next page of text (for long outputs).",
        "  clear  - Clears the currently displayed output lines.",
        "  help  - Displays this help message."
    };

    console->printPaged(lines);
}

void Game::executeCommand(const std::string& cmd) {
    std::istringstream ss(cmd);
    std::string action;
    ss >> action;

    auto it = commandHandlers.find(action);
    if (it != commandHandlers.end()) {
        it->second(ss);
    } else {
        console->print("Unknown command: " + action);
    }
}
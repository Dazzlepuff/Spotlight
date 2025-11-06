#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <string>

#include "Renderer.hpp"
#include "CommandConsole.hpp"
#include "Colors.hpp"
#include "PathUtils.hpp"

Game::Game(int boardSize, std::vector<Company> companyList)
    : board(boardSize),
      window(sf::VideoMode(1600, 1200), "Hex Board")
{
    auto fontPath = PathUtils::getAssetPath("consolas.ttf");
    if (!font.loadFromFile(fontPath.string())) {
        std::cerr << "Error: Could not load font at " << fontPath << "\n";
    }

    sf::Vector2f consolePosition(20.f, 1160.f);

    console = new CommandConsole(board, font, consolePosition);
    renderer = new Renderer(board, font);

    companies = companyList;
}

Game::~Game(){
    delete renderer;
    delete console;
}

void Game::addPlayer(const std::string& name, Company* company) {
    players.emplace_back(name, company);
}

void Game::setup() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(0, Colors::all.size() - 2);
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

int Game::getCurrentActivePlayerIndex(){
    return currentActivePlayerIndex;
}

int Game::getCurrentDay(){
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
    for (int i = 0; i < amount; ++i) {
        if (deck.empty()) {
            console->print("The deck is empty! No more cards to draw.");
            return;
        }

        Card drawn = deck.drawCard();
        player.addHeldCard(drawn);

        console->print(player.name + " drew a card: " + drawn.name);
    }
}

void Game::giveResourceToPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole) {
    if (playerIndex < 0 || playerIndex >= players.size()) {
        if (logToConsole) console->print("Error: Invalid player index.");
        return;
    }
    Player& player = players[playerIndex];
    player.resources[resource] += amount;
    if (logToConsole) console->print("Gave " + std::to_string(amount) + " " + resource + " to " + player.name + ".");
}

bool Game::spendResourceFromPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole) {
    if (playerIndex < 0 || playerIndex >= players.size()) {
        if (logToConsole) console->print("Error: Invalid player index.");
        return false;
    }

    Player& player = players[playerIndex];
    auto it = player.resources.find(resource);
    if (it == player.resources.end() || it->second < amount) {
        if (logToConsole) console->print("Error: Not enough " + resource + " for " + player.name + ".");
        return false;
    }

    it->second -= amount;
    if (logToConsole) console->print(player.name + " spent " + std::to_string(amount) + " " + resource + ".");
    return true;
}

void Game::buildStage(int x, int y, int z, const std::string& color, int playerIndex){
    if(playerIndex = -1){
        playerIndex = getCurrentActivePlayerIndex();
    }
    Player activePlayer = players[playerIndex];

    board.setTileOwner(x, y, z, activePlayer.company);
    board.setTileColor(x, y, z, color);

    
}

void Game::endTurn(bool logToConsole){
    currentActivePlayerIndex++;
    if (currentActivePlayerIndex > players.size() - 1)
    {
        currentActivePlayerIndex = 0;
        currentDay++;
        if (logToConsole) console->print("Last player finished turn. Starting day: " + currentDay);
        startNewDay();
    }
}

void Game::executeCommand(const std::string& cmd) {
    std::istringstream ss(cmd);
    std::string action;
    ss >> action;

    if (action == "next") {
        if (console->awaitingNextPage) {
            console->showNextPage();
        } else {
            console->print("No more pages to show.");
        }
        return;
    }
    else if (action == "clear") {
        console->clear();
    }
    else if (action == "set_color") {
        int x, y, z;
        std::string color;
        ss >> x >> y >> z >> color;

        if (ss.fail()) {
            console->print("Usage: set_color <x> <y> <z> <color>");
            return;
        } 
        if (!Colors::isValid(color)) {
            console->print(color + " is not a valid color. Valid colors: ");
            for (const auto& c : Colors::all){
                console -> print("   " + c);
            }
            return;
        }

        board.setTileColor(x, y, z, color);
        console->print("Set tile (" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ") to " + color);
    }
    else if (action == "set_owner") {
        int x, y, z;
        int companyIndex;
        ss >> x >> y >> z >> companyIndex;

        if (ss.fail()) {
            console->print("Usage: set_owner <x> <y> <z> <company_index>");
            return;
        }
        if (companyIndex < 0 || companyIndex >= companies.size()) {
            console->print("Error: Company index " + std::to_string(companyIndex) + " is out of range. Max valid index: " + std::to_string(companies.size() - 1));
            return;
        }

        board.setTileOwner(x, y, z, &companies[companyIndex]);
        console->print("Set tile (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ") to " + companies[companyIndex].getName() + ": " + companies[companyIndex].getSymbol());
    }
    else if (action == "build") {
        int x, y, z;
        std::string color;
        int playerIndex;

        ss >> x >> y >> z >> color >> playerIndex;
        
        if (ss.fail()){
            console->print("Usage: build <x> <y> <z> <color> <player_index>");
            return;
        }
        if (playerIndex < 0 || playerIndex >= players.size()) {
            console->print("Error: Player index " + std::to_string(playerIndex) + 
                        " is out of range. Max valid index: " + std::to_string(players.size() - 1));
            return;
        }
        if (!Colors::isValid(color)) {
            console->print(color + " is not a valid color. Valid colors: ");
            for (const auto& c : Colors::all){
                console -> print("   " + c);
            }
            return;
        }

        buildStage(x, y, z, color, playerIndex);
        console->print(players[playerIndex].name + " (" + players[playerIndex].company->getName() + ") " "built a " + color + "stage at " + std::to_string(x) + std::to_string(y) + std::to_string(z));
    }
    else if (action == "list_players") {
        if (players.empty()) {
            console->print("No players available.");
        } else {
            console->print("Players:");
            for (const auto& p : players)
                console->print(" - " + p.name + " (" + p.company->getName() + ": " + p.company->getSymbol() + ")");
        }
    }
    else if (action == "show_resources") {
        int playerIndex;
        ss >> playerIndex;

        if (ss.fail()) {
            console->print("Usage: show_resources <player_index>");
            return;
        }
        if (playerIndex < 0 || playerIndex >= players.size()) {
            console->print("Error: Player index " + std::to_string(playerIndex) + 
                        " is out of range. Max valid index: " + std::to_string(players.size() - 1));
            return;
        }

        Player& player = players[playerIndex];
        console->print("Resources for player " + player.name + ":");

        for (const auto& [resource, amount] : player.resources) {
            console->print("  " + resource + ": " + std::to_string(amount));
        }
    }
    else if (action == "show_cards") {
        int playerIndex;
        ss >> playerIndex;

        if (ss.fail()) {
            console->print("Usage: show_cards <player_index>");
            return;
        }
        if (playerIndex < 0 || playerIndex >= players.size()) {
            console->print("Error: Player index " + std::to_string(playerIndex) +
                        " is out of range. Max valid index: " + std::to_string(players.size() - 1));
            return;
        }

        Player& player = players[playerIndex];
        console->print("Cards held by " + player.name + ":");

        if (player.heldCards.empty()) {
            console->print("  (no cards)");
        } else {
            for (const auto& card : player.heldCards) {
                console->print("  - " + card.name);
            }
        }
    }
    else if (action == "get_card_count") {
        std::string deckName;
        ss >> deckName;

        if (ss.fail()) {
            console->print("Usage: get_card_count <deck_name>");
            return;
        }

        int cardCount = getDeckByName(deckName)->size();

        console->print(deckName + " has " + std::to_string(cardCount) + " card" + (cardCount == 1 ? "" : "s"));
    }
    else if (action == "draw_card") {
        int playerIndex;
        std::string deckName;
        int amount;

        ss >> playerIndex >> deckName >> amount;

        if (ss.fail()) {
            console->print("Usage: draw_card <player_index> <deck_name> <amount>");
            return;
        }

        if (playerIndex < 0 || playerIndex >= players.size()) {
            console->print("Error: Player index " + std::to_string(playerIndex) +
                           " is out of range. Max valid index: " + std::to_string(players.size() - 1));
            return;
        }

        Deck* deck = getDeckByName(deckName);
        if (!deck) {
            console->print("Error: Deck '" + deckName + "' not found.");
            return;
        }

        drawCardForPlayer(*deck, players[playerIndex], amount);
    }
    else if (action == "give_resource") {
        int playerIndex;
        std::string resource;
        int amount;
        ss >> playerIndex >> resource >> amount;
        if (ss.fail()) {
            console->print("Usage: give_resource <player_index> <resource> <amount>");
            return;
        }
        giveResourceToPlayer(playerIndex, resource, amount);
    }
    else if (action == "spend_resource") {
        int playerIndex;
        std::string resource;
        int amount;
        ss >> playerIndex >> resource >> amount;
        if (ss.fail()) {
            console->print("Usage: spend_resource <player_index> <resource> <amount>");
            return;
        }
        spendResourceFromPlayer(playerIndex, resource, amount);
    }
    else if (action == "help") {
        std::vector<std::string> lines = {
            "Available commands:",
            "  set_color <x> <y> <z> <color>  - Sets the color of a tile.",
            "  set_owner <x> <y> <z> <company_index>  - Assigns tile ownership.",
            "  build <x> <y> <z> <color> <player_index>  - Builds a stage.",
            "  list_players  - Lists all players.",
            "  show_resources <player_index>  - Shows player resources.",
            "  give_resource <player_index> <resource> <amount>  - Gives resources.",
            "  spend_resource <player_index> <resource> <amount>  - Spends resources.",
            "  show_cards <player_index>  - Lists player cards.",
            "  get_card_count <deck_name>  - Shows cards left in a deck.",
            "  draw_card <player_index> <deck_name> <amount>  - Draws cards for a player.",
            "  end_turn  - Ends the current player's turn.",
            "  next  - Shows the next page of text (for long outputs).",
            "  help  - Displays this help message."
        };

        console->printPaged(lines);
    }
    else {
        console->print("Unknown command: " + action);
    }
}

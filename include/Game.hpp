#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Board.hpp"
#include "Company.hpp"
#include "Deck.hpp"

class Renderer;
class CommandConsole;

class Game {
public:
    Game(int boardSize, std::vector<Company> companyList);
    ~Game();
    
    void addPlayer(const std::string& name, Company* company);
    void setup();
    void mainLoop();

    const std::vector<Player>& getPlayers() const;

    void startNewDay();

    void drawCardForPlayer(Deck& deck, Player& player, int amount);
    void giveResourceToPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);
    bool spendResourceFromPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);
    void buildStage(int x, int y, int z, const std::string& color, int playerIndex = -1);
    void endTurn(bool logToConsole = true);

    int getCurrentDay();
    int getCurrentActivePlayerIndex();

private:
    void executeCommand(const std::string& cmd);

    Board board;
    std::vector<Player> players;
    std::vector<Company> companies;

    std::vector<Deck> decks;
    Deck* getDeckByName(const std::string& deckName);


    int currentDay;
    int currentActivePlayerIndex;

    sf::Font font;
    sf::RenderWindow window;
    Renderer* renderer;
    CommandConsole* console;
};

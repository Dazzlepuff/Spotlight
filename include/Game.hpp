#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Board.hpp"
#include "Company.hpp"

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

    void giveResourceToPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);
    bool spendResourceFromPlayer(int playerIndex, const std::string& resource, int amount, bool logToConsole = true);

private:
    void executeCommand(const std::string& cmd);

    Board board;
    std::vector<Player> players;
    std::vector<Company> companies;
    sf::Font font;
    sf::RenderWindow window;
    Renderer* renderer;
    CommandConsole* console;
};

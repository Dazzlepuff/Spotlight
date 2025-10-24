#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Board.hpp"
#include "Company.hpp"

// Forward declarations
class Renderer;
class CommandConsole;

class Game {
public:
    Game(int boardSize);
    ~Game();
    void addPlayer(const std::string& name, Company* company);
    void setup();
    void mainLoop();

    const std::vector<Player>& getPlayers() const;

private:
    void executeCommand(const std::string& cmd);

    Board board;
    std::vector<Player> players;
    sf::Font font;
    sf::RenderWindow window;
    Renderer* renderer;
    CommandConsole* console;
};

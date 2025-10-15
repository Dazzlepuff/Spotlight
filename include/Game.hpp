#pragma once
#include <vector>
#include <string>
#include "Player.hpp"
#include "Board.hpp"
#include "Company.hpp"

class Game {
private:
    Board board;
    std::vector<Player> players;
    std::vector<Company> companies;

public:
    Game(int boardSize);

    void addPlayer(const std::string& name, Company* company);
    void setup();
    void start();
};

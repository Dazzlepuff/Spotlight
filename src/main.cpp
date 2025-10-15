#include "Game.hpp"
#include "Company.hpp"

int main() {
    Game game(3);

    game.addPlayer("Owen", 1);
    game.addPlayer("Aaron", 2);

    game.setup();
    game.start();

    return 0;
}

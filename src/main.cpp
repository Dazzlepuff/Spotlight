#include "Game.hpp"
#include "Company.hpp"
#include "Renderer.hpp"

int main() {

    std::vector<Company> companies = {
        Company("company1"),
        Company("company2")
    };

    Game game(3);

    game.addPlayer("Owen", &companies[0]);
    game.addPlayer("Aaron", &companies[1]);

    game.setup();

    Renderer renderer(game.board);
    renderer.run();

    game.start();

    return 0;
}

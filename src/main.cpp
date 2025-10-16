#include "Game.hpp"
#include "Company.hpp"
#include "Renderer.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    std::vector<Company> companies = {
        Company("company1"),
        Company("company2")
    };

    Game game(3);

    game.addPlayer("Owen", &companies[0]);
    game.addPlayer("Aaron", &companies[1]);

    game.setup();

    sf::Font font;
    if (!font.loadFromFile("assets/consolas.ttf")) {
        return -1;
    }

    Renderer renderer(game.board, font, 800, 600);

    renderer.run();

    game.start();

    return 0;
}

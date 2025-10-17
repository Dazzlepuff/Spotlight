#include <iostream>
#include <SFML/Graphics.hpp>
#include "StartupMenu.hpp"
#include "Company.hpp"
#include "Game.hpp"
#include "Renderer.hpp"

StartupMenu::StartupMenu() {

}

int StartupMenu::StartMenuLoop(){

    std::cout << "Creating companies";
    
    std::vector<Company> companies = {
        Company("company1", "@"),
        Company("company2", "&")
    };
    
    std::cout << "Creating game object";

    Game game(6);

    std::cout << "Adding players";

    game.addPlayer("Owen", &companies[0]);
    game.addPlayer("Aaron", &companies[1]);

    std::cout << "Setting up game";
    
    game.setup();

    std::cout << "Creating game window";

    sf::Font font;
    if (!font.loadFromFile("assets/consolas.ttf")) {
        return -1;
    }

    Renderer renderer(game.board, font, 1600, 1200);
    renderer.run();

    return 0;
}
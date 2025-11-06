#include "Renderer.hpp"
#include "CommandConsole.hpp"
#include "Colors.hpp"
#include <cmath>

#include <iostream>

Renderer::Renderer(Board& b, sf::Font& f)
    : board(b), font(f)
{}


void Renderer::handleEvents(sf::RenderWindow& window, CommandConsole& console) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        console.handleEvent(event);
    }
}

void Renderer::render(sf::RenderWindow& window, CommandConsole& console) {
    window.clear(sf::Color(30, 30, 30));
    drawBoard(window);
    console.draw(window);
    window.display();
}


void Renderer::drawBoard(sf::RenderWindow& window) {
    const float hexRadius = 50.f;
    const float hexHeight = std::sqrt(3.f) * hexRadius;
    const sf::Vector2f center(800, 600);

    for (const auto& [coord, tile] : board.tiles) {
        float q = static_cast<float>(coord.x);
        float r = static_cast<float>(coord.z);

        float x = hexRadius * 1.5f * q + center.x;
        float y = hexHeight * (r + q / 2.f) + center.y;

        sf::CircleShape hex(hexRadius, 6);
        hex.setOrigin(hexRadius, hexRadius);
        hex.setPosition(x, y);
        hex.setRotation(30);

        const std::string& colorName = tile.getColor();
        hex.setFillColor(Colors::getSfmlColor(colorName));

        window.draw(hex);
        Company* owner = tile.getOwner();
        if (owner != nullptr) {

            sf::Text symbolText;
            symbolText.setFont(font);
            symbolText.setString(owner->getSymbol());
            symbolText.setCharacterSize(24);
            symbolText.setFillColor(sf::Color::Black);

            sf::FloatRect symBounds = symbolText.getLocalBounds();
            symbolText.setOrigin(symBounds.width / 2.f, symBounds.height / 2.f);
            symbolText.setPosition(x, y - 12);

            window.draw(symbolText);
        }

        sf::Text coordText;
        coordText.setFont(font);
        coordText.setString("(" + std::to_string(coord.x) + ", " +
                                 std::to_string(coord.y) + ", " +
                                 std::to_string(coord.z) + ")");
        coordText.setCharacterSize(12);
        coordText.setFillColor(sf::Color(50, 50, 50));

        sf::FloatRect bounds = coordText.getLocalBounds();
        coordText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        coordText.setPosition(x, y + 16);

        window.draw(coordText);
    }
}

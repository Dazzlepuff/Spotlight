#include "Renderer.hpp"
#include <cmath>

Renderer::Renderer(Board& b, int width, int height)
    : board(b), window(sf::VideoMode(width, height), "Hex Board") {}

void Renderer::run() {
    while (window.isOpen()) {
        handleEvents();

        window.clear(sf::Color(30, 30, 30)); // dark background
        drawBoard();
        window.display();
    }
}

void Renderer::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Renderer::drawBoard() {
    const float hexRadius = 30.f;
    const float hexHeight = std::sqrt(3) * hexRadius;
    const sf::Vector2f center(400, 300);

    for (const auto& [coord, tile] : board.tiles) {
        float q = coord.x;
        float r = coord.y;

        float x = hexRadius * 1.5f * q + center.x;
        float y = hexHeight * (r + q / 2.f) + center.y;

        sf::CircleShape hex(hexRadius, 6);
        hex.setOrigin(hexRadius, hexRadius);
        hex.setPosition(x, y);
        hex.setRotation(30);

        // Choose color based on tile color name
        std::string c = tile.getColor();
        if (c == "Red") hex.setFillColor(sf::Color::Red);
        else if (c == "Blue") hex.setFillColor(sf::Color::Blue);
        else if (c == "Green") hex.setFillColor(sf::Color::Green);
        else if (c == "Yellow") hex.setFillColor(sf::Color::Yellow);
        else if (c == "Purple") hex.setFillColor(sf::Color(180, 0, 255)); // custom purple
        else if (c == "Gray") hex.setFillColor(sf::Color(120, 120, 120));
        else if (c == "White") hex.setFillColor(sf::Color::White);
        else hex.setFillColor(sf::Color(60, 60, 60)); // neutral

        window.draw(hex);
    }
}

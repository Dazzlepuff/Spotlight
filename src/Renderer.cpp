#include "Renderer.hpp"
#include <cmath>

Renderer::Renderer(Board& b, sf::Font& f, int width, int height)
    : board(b), font(f), window(sf::VideoMode(width, height), "Hex Board"), console(b, f)
{}

void Renderer::run() {
    while (window.isOpen()) {
        handleEvents();

        window.clear(sf::Color(30, 30, 30));
        drawBoard();
        console.draw(window);
        window.display();
    }
}

void Renderer::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        console.handleEvent(event);
    }
}

void Renderer::drawBoard() {
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

        std::string c = tile.getColor();
        if (c == "Red")        hex.setFillColor(sf::Color(0xD9, 0x7B, 0x66));
        else if (c == "Blue")  hex.setFillColor(sf::Color(0x6C, 0x8E, 0xBF));
        else if (c == "Green") hex.setFillColor(sf::Color(0x7C, 0xA9, 0x82));
        else if (c == "Yellow")hex.setFillColor(sf::Color(0xE3, 0xC5, 0x67));
        else if (c == "Purple")hex.setFillColor(sf::Color(0xA8, 0x8E, 0xC6));
        else if (c == "Gray")  hex.setFillColor(sf::Color(0xB0, 0xA8, 0xB9));
        else if (c == "White") hex.setFillColor(sf::Color(0xF2, 0xE9, 0xE4));
        else                   hex.setFillColor(sf::Color(0x4B, 0x4A, 0x54));

        window.draw(hex);

        sf::Text text;
        text.setFont(font);
        text.setString("(" + std::to_string(coord.x) + ", " +
                             std::to_string(coord.y) + ", " +
                             std::to_string(coord.z) + ")");
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        text.setPosition(x, y - 6);

        window.draw(text);
    }
}
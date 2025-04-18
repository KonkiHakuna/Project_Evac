#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({ 600, 700 }), "SFML",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{ .antiAliasingLevel = 6 });

    auto circle = sf::CircleShape(70);
    circle.setPosition({ 70, 50 });
    circle.setFillColor(sf::Color(0xAABBAA));
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    circle.setFillColor(sf::Color(0xFFBBCC));
                }
            }
            else if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::A) {
                }
            }
        }
        window.clear(sf::Color::White);

        window.draw(circle);
        if (circle.getPosition().x + circle.getRadius() * 2 < window.getSize().x)
            circle.move({ 0.1f, 0.1f });

        window.display();
    }

    return 0;
}

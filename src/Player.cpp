
#include "Player.h"

Player::Player(sf::RenderWindow& window) {
	shape.setRadius(20);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin({shape.getRadius(), shape.getRadius()});
	shape.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
}

void Player::movement() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		shape.move({0,-3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		shape.move({0,3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		shape.move({-3,0});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		shape.move({3,0});
	}
}

void Player::draw(sf::RenderWindow& window) const{
	window.draw(shape);
}

sf::Vector2f Player::getPosition() const{
	return shape.getPosition();
}

void Player::resetPosition(sf::RenderWindow& window) {
	shape.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });
}


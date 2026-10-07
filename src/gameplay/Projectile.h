#pragma once

#include <SFML/Graphics.hpp>

class Projectile {
public:
	Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition, int damage);

	void update(float deltaTime);
	void draw(sf::RenderWindow& window) const;

	sf::FloatRect getGlobalBounds() const;
	int getDamage() const;

	sf::Vector2f getPosition() const;

private:
	sf::CircleShape projectile;
	sf::Vector2f direction;

	float speed = 1000.f;
	int damage;
};
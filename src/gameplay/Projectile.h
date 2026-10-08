#pragma once

#include <SFML/Graphics.hpp>


class Projectile {
public:
	Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition,int damage);

	// Moves the projectile using frame-rate independent movement.
	void update(float deltaTime);

	void draw(sf::RenderWindow& window) const;

	// Used for collision detection with enemies.
	sf::FloatRect getGlobalBounds() const;

	int getDamage() const;

	sf::Vector2f getPosition() const;

private:
	sf::CircleShape projectile;

	// Normalized direction from the spawn position toward the target.
	sf::Vector2f direction;

	float speed = 1000.f;
	int damage;
};
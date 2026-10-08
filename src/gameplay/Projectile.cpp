#include "Projectile.h"

#include <cmath>


Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition,int damage) : damage(damage) {

	projectile.setRadius(16.f);

	// Center the origin so the projectile position
	// represents the middle of the circle.
	projectile.setOrigin({ 16.f, 16.f });

	projectile.setPosition(startPosition);
	projectile.setFillColor(sf::Color::Yellow);


	// Calculate the vector pointing from the player
	// toward the position that was clicked.
	sf::Vector2f difference =
		targetPosition - startPosition;


	// Calculate the length of the direction vector.
	float length = std::sqrt(
		difference.x * difference.x
		+ difference.y * difference.y
	);


	// Normalize the vector so direction only represents
	// the direction of travel, not the distance to the target.
	if (length != 0.f) {

		direction = {
			difference.x / length,
			difference.y / length
		};
	}
}


void Projectile::update(float deltaTime) {

	// Multiplying by deltaTime makes movement depend on elapsed time
	// instead of the number of rendered frames.
	projectile.move({
		direction.x * speed * deltaTime,
		direction.y * speed * deltaTime
		});
}


void Projectile::draw(sf::RenderWindow& window) const {
	window.draw(projectile);
}


sf::FloatRect Projectile::getGlobalBounds() const {
	return projectile.getGlobalBounds();
}


int Projectile::getDamage() const {
	return damage;
}


sf::Vector2f Projectile::getPosition() const {
	return projectile.getPosition();
}
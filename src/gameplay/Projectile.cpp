#include "Projectile.h"

#include <cmath>

Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition,int damage) : damage(damage) {

	projectile.setRadius(16.f);
	projectile.setOrigin({ 16.f, 16.f });
	projectile.setPosition(startPosition);
	projectile.setFillColor(sf::Color::Yellow);

	sf::Vector2f difference = targetPosition - startPosition;

	float length = std::sqrt(difference.x * difference.x + difference.y * difference.y);

	if (length != 0.f) {
		direction = {
			difference.x / length,
			difference.y / length
		};
	}
}

void Projectile::update(float deltaTime) {
	projectile.move({direction.x * speed * deltaTime,direction.y * speed * deltaTime});
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
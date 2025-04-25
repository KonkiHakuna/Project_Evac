#pragma once

#include <SFML/Graphics.hpp>

class Player {
public:
	Player(sf::RenderWindow& window);
	void movement();
	void draw(sf::RenderWindow& window) const;
	void attack();
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;
private:
	sf::Vector2f velocity;
	sf::CircleShape shape;
};

//class Inventory {};
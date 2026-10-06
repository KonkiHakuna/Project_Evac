#pragma once

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "gameplay/Item.h"

class Item;
class Inventory;
class Enemy;

class Player {
public:
	Player(sf::RenderWindow& window);
	void movement();
	void movementBounds();
	void dash();
	void draw(sf::RenderWindow& window);
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;
	int getHealth();
	void setHealth(int value, char op);
	int getMana();
	void setMana(int value, char op);
	void setDefence(int value);
	int getDefence() const;
	void attack(sf::Event::MouseButtonPressed const& e, Inventory& inventory, std::vector<std::unique_ptr<Enemy>>& enemies);
	sf::FloatRect getGlobalBounds() const;


private:
	int health;
	int mana;
	int defence;
	sf::Vector2f velocity;
	sf::CircleShape player;
	sf::CircleShape attackPlayerHitbox;
	sf::Clock attackTimer;
	bool drawAttackPlayerHitbox = false;
};
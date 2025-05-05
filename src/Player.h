#pragma once

#include <SFML/Graphics.hpp>

#include "Item.h"

class Player {
public:
	Player(sf::RenderWindow& window);
	void movement();
	void draw(sf::RenderWindow& window) const;
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;
	int getHealth();
	void setHealth(int value, char op);
	int getMana();
	void setMana(int value, char op);
private:
	int health;
	int mana;
	sf::Vector2f velocity;
	sf::CircleShape shape;
};

class Inventory {
public:
	Inventory();
	void addItem(std::unique_ptr<Item> item);
	Item* getCurrentWeapon();
	void setCurrentWeapon(Item* weapon);
	int getPlayerGold();
	void setPlayerGold(int value, char op);
	std::vector<Item*> getItems();
private:
	int gold;
	Item* currentWeapon;
	std::vector<std::unique_ptr<Item>> items;
};
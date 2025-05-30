#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "Item.h"

class Item;
class Inventory;
class Enemy;

class Player {
public:
	Player(sf::RenderWindow& window);
	void movement();
	void movementBounds();
	void draw(sf::RenderWindow& window);
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;
	int getHealth();
	void setHealth(int value, char op);
	int getMana();
	void setMana(int value, char op);
	void setDefense(int value);
	int getDefence();
	void attack(sf::Event::MouseButtonPressed const& e, Inventory& inventory, std::vector<std::unique_ptr<Enemy>>& enemies);
	sf::FloatRect getGlobalBounds() const;


private:
	int health;
	int mana;
	int defense;
	sf::Vector2f velocity;
	sf::CircleShape player;
	sf::CircleShape attackPlayerHitbox;
	sf::Clock attackTimer;
	bool drawAttackPlayerHitbox = false;
};

class Inventory {
public:
	Inventory();
	void addItem(std::unique_ptr<Item> item);
	void removeItem(Item* item);
	Item* getCurrentWeapon();
	void setCurrentWeapon(Item* weapon);
	Item* getCurrentArmor();
	void setCurrentArmor(Item* armor);
	Item* getCurrentSpell();
	void setCurrentSpell(Item* spell);
	int getPlayerGold();
	void setPlayerGold(int value, char op);
	std::vector<Item*> getItems();
	void clearItems();

private:
	int gold;
	Item* currentWeapon;
	Item* currentArmor;
	Item* currentSpell;
	std::vector<std::unique_ptr<Item>> items;
};
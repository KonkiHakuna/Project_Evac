#pragma once

#include <SFML/Graphics.hpp>

#include "entities/Enemy.h"
#include "Item.h"

class Item;
class Inventory;
class Enemy;

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
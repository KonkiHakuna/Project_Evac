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

	// Transfers ownership of a new item to the inventory.
	void addItem(std::unique_ptr<Item> item);

	// Removes an item using its non-owning raw pointer.
	void removeItem(Item* item);

	Item* getCurrentWeapon();
	void setCurrentWeapon(Item* weapon);

	Item* getCurrentArmor();
	void setCurrentArmor(Item* armor);

	Item* getCurrentSpell();
	void setCurrentSpell(Item* spell);

	int getPlayerGold();

	// Changes the player's gold using the provided operation.
	void setPlayerGold(int value, char op);

	// Returns non-owning pointers to the items stored by the inventory.
	std::vector<Item*> getItems();

	void clearItems();

private:
	int gold;

	// These pointers only reference equipped items.
	// Ownership remains inside the items vector.
	Item* currentWeapon;
	Item* currentArmor;
	Item* currentSpell;

	// unique_ptr makes Inventory the owner of all stored items.
	std::vector<std::unique_ptr<Item>> items;
};
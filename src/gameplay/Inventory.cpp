#include "Inventory.h"
#include "entities/Player.h"


Inventory::Inventory() : gold(50), currentArmor(nullptr), currentSpell(nullptr) {

	// Give the player a basic weapon when the inventory is created.
	addItem(
		std::make_unique<Weapon>(
			"Stick",
			"assets/items/weapons/stick.png",
			50,
			true
		)
	);

	// get() returns a raw pointer without transferring ownership.
	currentWeapon = items[0].get();
}


void Inventory::addItem(std::unique_ptr<Item> item) {

	// unique_ptr cannot be copied, so ownership is moved
	// from the function parameter into the inventory vector.
	items.push_back(std::move(item));
}


void Inventory::removeItem(Item* item) {

	// Clear equipped references before deleting the owned item.
	// Otherwise they could point to an object that no longer exists.
	if (item == currentWeapon) {
		currentWeapon = nullptr;
	}

	if (item == currentArmor) {
		currentArmor = nullptr;
	}

	if (item == currentSpell) {
		currentSpell = nullptr;
	}


	// Find the unique_ptr that owns the requested Item.
	for (auto it = items.begin(); it != items.end(); ++it) {

		// it points to a unique_ptr, while get() gives access
		// to the raw Item pointer stored inside it.
		if (it->get() == item) {

			// Erasing the unique_ptr also destroys the owned Item.
			items.erase(it);
			break;
		}
	}


	// If the equipped weapon was removed,
	// try to equip another weapon from the inventory.
	if (!currentWeapon) {

		for (auto& it : items) {

			// Check whether the stored Item is actually a Weapon.
			if (dynamic_cast<Weapon*>(it.get())) {
				currentWeapon = it.get();
				break;
			}
		}


		// If no weapon exists, create a default Stick
		// so the player always has something equipped.
		if (!currentWeapon) {

			addItem(
				std::make_unique<Weapon>(
					"Stick",
					"assets/items/weapons/stick.png",
					20,
					true
				)
			);

			currentWeapon = items.back().get();
		}
	}
}


Item* Inventory::getCurrentWeapon() {
	return currentWeapon;
}


void Inventory::setCurrentWeapon(Item* weapon) {
	currentWeapon = weapon;
}


Item* Inventory::getCurrentArmor() {
	return currentArmor;
}


void Inventory::setCurrentArmor(Item* armor) {
	currentArmor = armor;
}


Item* Inventory::getCurrentSpell() {
	return currentSpell;
}


void Inventory::setCurrentSpell(Item* spell) {
	currentSpell = spell;
}


int Inventory::getPlayerGold() {
	return gold;
}


void Inventory::setPlayerGold(int value, char op) {

	// Use one method for adding, subtracting or directly setting gold.
	switch (op) {

	case '+':
		gold += value;
		break;

	case '-':
		gold -= value;
		break;

	case '=':
		gold = value;
		break;

	default:
		throw std::exception("Invalid operator");
	}
}


std::vector<Item*> Inventory::getItems() {

	// The inventory keeps ownership through unique_ptr,
	// while the returned vector contains only non-owning raw pointers.
	std::vector<Item*> tempItems;


	for (auto& item : items) {
		tempItems.push_back(item.get());
	}


	return tempItems;
}


sf::FloatRect Player::getGlobalBounds() const {
	return player.getGlobalBounds();
}


void Inventory::clearItems() {

	// Clearing the unique_ptr vector automatically destroys all stored items.
	items.clear();

	// Equipped pointers must also be reset because
	// the objects they referenced no longer exist.
	currentWeapon = nullptr;
	currentArmor = nullptr;
	currentSpell = nullptr;
}
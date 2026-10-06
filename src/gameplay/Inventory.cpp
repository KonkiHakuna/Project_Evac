#include "Inventory.h"
#include "entities/Player.h"

Inventory::Inventory() : gold(50), currentArmor(nullptr), currentSpell(nullptr) {
	addItem(std::make_unique<Weapon>("Stick", "assets/items/weapons/stick.png", 50, true));
	currentWeapon = items[0].get();
}

void Inventory::addItem(std::unique_ptr<Item> item) {
	items.push_back(std::move(item));
}

void Inventory::removeItem(Item* item) {
	if (item == currentWeapon) {
		currentWeapon = nullptr;
	}
	if (item == currentArmor) {
		currentArmor = nullptr;
	}
	if (item == currentSpell) {
		currentSpell = nullptr;
	}
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (it->get() == item) {
			items.erase(it);
			break;
		}
	}
	if (!currentWeapon) {
		for (auto& it : items) {
			if (dynamic_cast<Weapon*>(it.get())) {
				currentWeapon = it.get();
				break;
			}
		}
		if (!currentWeapon) {
			addItem(std::make_unique<Weapon>("Stick", "assets/items/weapons/stick.png", 20, true));
			currentWeapon = items[0].get();
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
	items.clear();
	currentWeapon = nullptr;
	currentArmor = nullptr;
	currentSpell = nullptr;
}
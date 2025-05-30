#include "Save.h"
#include <fstream>
#include <string>

void Save::SaveGame(Player& player, Inventory& inventory) {
	std::ofstream saveFile("save.txt");
	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}
	saveFile << player.getHealth() << "\n";
	saveFile << player.getMana() << "\n";
	saveFile << player.getDefence() << "\n";
	saveFile << inventory.getPlayerGold() << "\n";

	for (auto& item : inventory.getItems()) {
		if (auto* weapon = dynamic_cast<Weapon*>(item)) {
			saveFile << "Weapon " << weapon->getName() << " " << weapon->getDamage() << " " << weapon->isMelee() << " " << weapon->getPath().string() << "\n";
		}
		else if (auto* armor = dynamic_cast<Armor*>(item)) {
			saveFile << "Armor " << armor->getName() << " " << armor->getDefense() << " " << armor->getPath().string() << "\n";
		}
		else if (auto* potion = dynamic_cast<Potion*>(item)) {
			saveFile << "Potion " << potion->getName() << " " << potion->getPower() << " " << static_cast<int>(potion->getType()) << " " << potion->getPath().string() << "\n";
		}
		else if (auto* spell = dynamic_cast<Spell*>(item)) {
			saveFile << "Spell " << spell->getName() << " " << spell->getDamage() << " " << spell->getManaCost() << " " << spell->getPath().string() << "\n";
		}
	}
}

void Save::LoadGame(Player& player, Inventory& inventory) {
	std::ifstream saveFile("save.txt");
	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}

	int health, mana, defense, gold;
	saveFile >> health >> mana >> defense >> gold;

	player.setHealth(health, '=');
	player.setMana(mana, '=');
	player.setDefense(defense);
	inventory.setPlayerGold(gold, '=');
	inventory.clearItems();

	std::string type;
	while (saveFile >> type) {
		std::string name;
		saveFile >> name;

		if (type == "Weapon") {
			int dmg;
			bool melee;
			std::string path;
			saveFile >> dmg >> melee >> path;
			inventory.addItem(std::make_unique<Weapon>(name, path, dmg, melee));
		}
		else if (type == "Armor") {
			int def;
			std::string path;
			saveFile >> def >> path;
			inventory.addItem(std::make_unique<Armor>(name, path, def));
		}
		else if (type == "Potion") {
			int power, typeInt;
			std::string path;
			saveFile >> power >> typeInt >> path;
			inventory.addItem(std::make_unique<Potion>(name, path, power, static_cast<TypeOfPotion>(typeInt)));
		}
		else if (type == "Spell") {
			int dmg, manaCost;
			std::string path;
			saveFile >> dmg >> manaCost >> path;
			inventory.addItem(std::make_unique<Spell>(name, path, dmg, manaCost));
		}
	}

	saveFile.close();
}

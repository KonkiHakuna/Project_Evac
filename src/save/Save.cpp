#include "Save.h"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "gameplay/Inventory.h"

using json = nlohmann::json;

void Save::SaveGame(Player& player, Inventory& inventory) {
	json saveData;
	
	saveData["player"] = {
		{"health", player.getHealth()},
		{"mana", player.getMana()}
	};
	saveData["inventory"]["gold"] = inventory.getPlayerGold();
	saveData["inventory"]["items"] = json::array();

	for (auto& item : inventory.getItems()) {
		json itemData;
		if (auto* weapon = dynamic_cast<Weapon*>(item)) {
			itemData = {
				{"type", "weapon"},
				{"name", weapon->getName()},
				{"damage", weapon->getDamage()},
				{"melee", weapon->isMelee()},
				{"path", weapon->getPath().string()}
			};
		}
		else if (auto* armor = dynamic_cast<Armor*>(item)) {
			itemData = {
				{"type", "armor"},
				{"name", armor->getName()},
				{"defence", armor->getDefence()},
				{"path", armor->getPath().string()}
			};
		}
		else if (auto* potion = dynamic_cast<Potion*>(item)) {
			itemData = {
				{"type", "potion"},
				{"name", potion->getName()},
				{"power", potion->getPower()},
				{"potionType", static_cast<int>(potion->getType())},
				{"path", potion->getPath().string()}
			};
		}
		else if (auto* spell = dynamic_cast<Spell*>(item)) {
			itemData = {
				{"type", "spell"},
				{"name", spell->getName()},
				{"damage", spell->getDamage()},
				{"manaCost", spell->getManaCost()},
				{"path", spell->getPath().string()}
			};
		}
		else{
			continue;
		}
		saveData["inventory"]["items"].push_back(itemData);
	}
	std::ofstream saveFile("save.json");
	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}
	saveFile << saveData.dump(4) << std::endl;
	saveFile.close();
}

void Save::LoadGame(Player& player, Inventory& inventory) {
	std::ifstream saveFile("save.json");
	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}

	json saveData;
	saveFile >> saveData;
	saveFile.close();

	player.setHealth(saveData.at("player").at("health").get<int>());
	player.setMana(saveData.at("player").at("mana").get<int>());
	inventory.setPlayerGold(saveData.at("inventory").at("gold").get<int>(), '=');
	inventory.clearItems();

	for (const auto& itemData : saveData.at("inventory").at("items")) {
		std::string type = itemData.at("type").get<std::string>();
		std::string name = itemData.at("name").get<std::string>();
		std::string path = itemData.at("path").get<std::string>();
		if (type == "weapon") {
			int damage = itemData.at("damage").get<int>();
			bool melee = itemData.at("melee").get<bool>();
			inventory.addItem(std::make_unique<Weapon>(name, path, damage, melee));
		}
		else if (type == "armor") {
			int defence = itemData.at("defence").get<int>();
			inventory.addItem(std::make_unique<Armor>(name, path, defence));
		}
		else if (type == "potion") {
			int power = itemData.at("power").get<int>();
			TypeOfPotion potionType = static_cast<TypeOfPotion>(itemData.at("potionType").get<int>());
			inventory.addItem(std::make_unique<Potion>(name, path, power, potionType));
		}
		else if (type == "spell") {
			int damage = itemData.at("damage").get<int>();
			int manaCost = itemData.at("manaCost").get<int>();
			inventory.addItem(std::make_unique<Spell>(name, path, damage, manaCost));
		}
		else {
			throw std::exception("Unknown item type in save file");
		}
	}
}
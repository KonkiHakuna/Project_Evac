#include "Save.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "gameplay/Inventory.h"


// Shorter alias for the JSON type provided by nlohmann/json.
using json = nlohmann::json;


void Save::SaveGame(Player& player, Inventory& inventory) {

	// Root JSON object containing the complete save data.
	json saveData;


	// Save the player's basic state.
	saveData["player"] = {
		{"health", player.getHealth()},
		{"mana", player.getMana()}
	};


	// Save inventory information.
	saveData["inventory"]["gold"] = inventory.getPlayerGold();

	// Items are stored as a JSON array because the inventory
	// can contain multiple objects of different derived Item types.
	saveData["inventory"]["items"] = json::array();


	// getItems() returns non-owning Item pointers.
	// Inventory still owns the actual objects through unique_ptr.
	auto items = inventory.getItems();


	// -1 represents an equipment slot with no equipped item.
	int equippedWeapon = -1;
	int equippedArmor = -1;
	int equippedSpell = -1;


	// Save equipped items by their indexes in the inventory.
	// Raw pointer comparison is enough because equipped pointers
	// reference the same Item objects stored inside Inventory.
	for (int i = 0; i < items.size(); ++i) {

		if (items[i] == inventory.getCurrentWeapon()) {
			equippedWeapon = i;
		}

		if (items[i] == inventory.getCurrentArmor()) {
			equippedArmor = i;
		}

		if (items[i] == inventory.getCurrentSpell()) {
			equippedSpell = i;
		}
	}


	saveData["inventory"]["equipped"] = {
		{"weapon", equippedWeapon},
		{"armor", equippedArmor},
		{"spell", equippedSpell}
	};


	// Serialize every item according to its actual runtime type.
	for (auto& item : items) {

		json itemData;


		// dynamic_cast checks whether the base Item pointer
		// actually points to a Weapon object.
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

				// Enum values are stored as integers so they can
				// be written directly into the JSON file.
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


		else {

			// Skip unknown Item-derived objects that cannot
			// be represented by the current save format.
			continue;
		}


		saveData["inventory"]["items"].push_back(itemData);
	}


	// Open the save file for writing.
	std::ofstream saveFile("save.json");


	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}


	// dump(4) formats the JSON with four-space indentation,
	// making the save file easier to read manually.
	saveFile << saveData.dump(4) << std::endl;

	saveFile.close();
}


void Save::LoadGame(Player& player, Inventory& inventory) {

	// Open the previously created save file.
	std::ifstream saveFile("save.json");


	if (!saveFile) {
		throw std::exception("Failed to open save file");
	}


	json saveData;

	// Parse the JSON directly from the input stream.
	saveFile >> saveData;

	saveFile.close();


	// Restore basic player values.
	player.setHealth(
		saveData.at("player").at("health").get<int>()
	);

	player.setMana(
		saveData.at("player").at("mana").get<int>()
	);


	// Restore gold and remove the inventory state
	// that existed before loading the save.
	inventory.setPlayerGold(
		saveData.at("inventory").at("gold").get<int>(),
		'='
	);

	inventory.clearItems();


	// Reconstruct every saved item as its original derived type.
	for (const auto& itemData : saveData.at("inventory").at("items")) {

		std::string type =
			itemData.at("type").get<std::string>();

		std::string name =
			itemData.at("name").get<std::string>();

		std::string path =
			itemData.at("path").get<std::string>();


		if (type == "weapon") {

			int damage =
				itemData.at("damage").get<int>();

			bool melee =
				itemData.at("melee").get<bool>();


			// make_unique creates the new object and Inventory
			// receives ownership when addItem() is called.
			inventory.addItem(
				std::make_unique<Weapon>(
					name,
					path,
					damage,
					melee
				)
			);
		}


		else if (type == "armor") {

			int defence =
				itemData.at("defence").get<int>();

			inventory.addItem(
				std::make_unique<Armor>(
					name,
					path,
					defence
				)
			);
		}


		else if (type == "potion") {

			int power =
				itemData.at("power").get<int>();


			// Convert the integer stored in JSON
			// back into the TypeOfPotion enum.
			TypeOfPotion potionType =
				static_cast<TypeOfPotion>(
					itemData.at("potionType").get<int>()
					);


			inventory.addItem(
				std::make_unique<Potion>(
					name,
					path,
					power,
					potionType
				)
			);
		}


		else if (type == "spell") {

			int damage =
				itemData.at("damage").get<int>();

			int manaCost =
				itemData.at("manaCost").get<int>();


			inventory.addItem(
				std::make_unique<Spell>(
					name,
					path,
					damage,
					manaCost
				)
			);
		}


		else {

			throw std::exception(
				"Unknown item type in save file"
			);
		}
	}


	// After recreating all items, retrieve pointers
	// to the newly loaded objects.
	auto loadedItems = inventory.getItems();


	// Read the indexes of previously equipped items.
	int weaponIndex =
		saveData.at("inventory")
		.at("equipped")
		.at("weapon")
		.get<int>();


	int armorIndex =
		saveData.at("inventory")
		.at("equipped")
		.at("armor")
		.get<int>();


	int spellIndex =
		saveData.at("inventory")
		.at("equipped")
		.at("spell")
		.get<int>();


	// Restore equipment only when the saved index
	// points to an item that actually exists.
	if (
		weaponIndex >= 0
		&& weaponIndex < loadedItems.size()
		) {

		inventory.setCurrentWeapon(
			loadedItems[weaponIndex]
		);
	}


	if (
		armorIndex >= 0
		&& armorIndex < loadedItems.size()
		) {

		inventory.setCurrentArmor(
			loadedItems[armorIndex]
		);
	}


	if (
		spellIndex >= 0
		&& spellIndex < loadedItems.size()
		) {

		inventory.setCurrentSpell(
			loadedItems[spellIndex]
		);
	}
}
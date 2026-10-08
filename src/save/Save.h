#pragma once

#include "entities/Player.h"


class Save {
public:
	// Serializes the current player and inventory state to save.json.
	void SaveGame(Player& player, Inventory& inventory);

	// Restores the player and inventory state from save.json.
	void LoadGame(Player& player, Inventory& inventory);
};
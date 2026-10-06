#pragma once

#include "entities/Player.h"

class Save {
public:
	void SaveGame(Player& player, Inventory& inventory);
	void LoadGame(Player& player, Inventory& inventory);
};
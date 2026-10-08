#pragma once

#include <SFML/Graphics.hpp>

#include "entities/Player.h"
#include "save/Save.h"
#include "ui/shop.h"
#include "ui/pausemenu.h"
#include "ui/StartScreen.h"
#include "ui/hud.h"
#include "ui/InventoryUI.h"
#include "gameplay/Inventory.h"
#include "ui/inventoryItemOptions.h"
#include "world/cave.h"
#include "gameplay/Projectile.h"

// Defines every main state in which the game can currently be.
// The active state decides which input, update and drawing code is executed.
enum class GameState {
	startScreen,
	lobby,
	cave,
	paused
};


class Game {
public:
	// Creates the main game window.
	Game();
	// Runs the main game loop until the window is closed.
	void run();
private:
	sf::RenderWindow window;
	sf::VideoMode mode;

	// Load the font once when the Game object is created.
	sf::Font caveatFont = []{
		sf::Font caveatFontTemp;
		if (!caveatFontTemp.openFromFile("assets/fonts/Caveat_font.ttf")) {
			throw std::exception("Failed to load font");
		}
		return caveatFontTemp;
		}();

	// currentGameState variable is used to determine which input, update and drawing code is executed.
	// previousGameState is used to return to the correct state after pausing.
	GameState currentGameState = GameState::startScreen;
	GameState previousGameState = GameState::startScreen;

	// Menus and UI.
	PauseMenu pauseMenu{ window, caveatFont };
	StartScreen startScreen{ window, caveatFont };

	Lobby lobby{window};
	Shop shop{ window,caveatFont };

	Player player{window};
	Inventory inventory;
	HUD hud{ window,caveatFont };
	InventoryItemOptions inventoryItemOptions{ window,caveatFont };
	InventoryUI inventoryUI{window,caveatFont,inventoryItemOptions};

	// Cave contains the current level and all the entities in it. It is created after the player has selected a level in the lobby.
	Cave cave{ window ,caveatFont};

	Save save;

	// Projectiles that are currently active in the game. They are updated and drawn in the cave state.
	std::vector<Projectile> projectiles;
};


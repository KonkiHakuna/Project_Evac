#pragma once

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "UI.h"
#include "Player.h"
#include "Save.h"

enum class GameState {
	startScreen,
	lobby,
	cave,
	paused
};

class Game {
public:
	Game();
	void run();
private:
	sf::RenderWindow window;
	sf::VideoMode mode;
	sf::Font caveatFont = []{
		sf::Font caveatFontTemp;
		if (!caveatFontTemp.openFromFile("assets/fonts/Caveat_font.ttf")) {
			throw std::exception("Failed to load font");
		}
		return caveatFontTemp;
		}();

	GameState currentGameState = GameState::startScreen;
	GameState previousGameState = GameState::startScreen;

	PauseMenu pauseMenu{ window, caveatFont };
	StartScreen startScreen{ window, caveatFont };

	Lobby lobby{window};
	Shop shop{ window,caveatFont };

	Player player{window};
	Inventory inventory;
	HUD hud{ window,caveatFont };
	InventoryItemOptions inventoryItemOptions{ window,caveatFont };
	InventoryUI inventoryUI{window,caveatFont,inventoryItemOptions};
	Cave cave{ window ,caveatFont};

	Save save;
};


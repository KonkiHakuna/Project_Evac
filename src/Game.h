#pragma once

#include <SFML/Graphics.hpp>

#include "Map.h"
#include "UI.h"
#include "Player.h"

enum class GameState {
	startScreen,
	lobby,
	mine,
	paused
};

class Game {
public:
	Game();
	void run();
private:
	sf::RenderWindow window;
	sf::VideoMode mode;
	sf::Font caveatFont;

	GameState currentGameState = GameState::startScreen;
	GameState previousGameState = GameState::startScreen;

	PauseMenu pauseMenu{ window, caveatFont };
	StartScreen startScreen{ window, caveatFont };

	Lobby lobby{window};
	Shop shop{ window,caveatFont };
	MineEntrance mineEntrance;

	Player player{window};
	Inventory inventory;
	HUD hud{ window,caveatFont };
	InventoryUI inventoryUI{window,caveatFont};

};


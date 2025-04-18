#pragma once

#include <SFML/Graphics.hpp>

#include "UI.h"

class Game {
public:
	Game();
	void run();
private:
	sf::RenderWindow window;
	sf::VideoMode mode;
	sf::Font caveatFont;

	StartScreen startScreen{window,caveatFont};
	bool isStartScreen = true;

	PauseMenu pauseMenu{window,caveatFont};
	bool isPaused = false;
};


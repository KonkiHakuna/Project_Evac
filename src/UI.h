#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Map.h"

class StartScreen {
public:
	StartScreen(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window);
private:
	sf::Text title;
	sf::Text pressSpace;
};

class PauseMenu {
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window) const;
	void mouseClick(sf::Event::MouseButtonPressed const& e);
	void mouseMove(sf::Event::MouseMoved const& e);
	void resetDecision() { decision = NULL; }
	int getDecision() const { return decision; }
private:
	std::vector<std::string> options;
	std::vector<sf::Text> menuOptions;
	int decision=NULL;
};

class Shop {
public:
	Shop(sf::RenderWindow& window);
	void interact(sf::RenderWindow& window, const sf::Font& font);
	void keyPressed();
	void draw(sf::RenderWindow& window);
	bool shoppingStatus(LobbyLocation location);
	void mouseClick(sf::Event::MouseButtonPressed const& e);
private:
	bool isOpen = false;
	sf::RectangleShape shopOverlay;
	sf::RectangleShape shopCloseButton;
};

//class HUD{};


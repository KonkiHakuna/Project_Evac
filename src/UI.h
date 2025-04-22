#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

class StartScreen {
public:
	StartScreen(sf::RenderWindow& window, sf::Font& font);
	void drawStartScreen(sf::RenderWindow& window);
private:
	sf::Text title;
	sf::Text pressSpace;
	bool isPressed = false;
};

class PauseMenu {
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	void drawPauseMenu(sf::RenderWindow& window) const;
	void mouseClick(sf::Event::MouseButtonPressed const& e);
	void mouseMove(sf::Event::MouseMoved const& e);
	void resetDecision() { decision = NULL; }
	int getDecision() const { return decision; }
private:
	std::vector<sf::Text> menuOptions;
	int decision=NULL;
};

class Shop {
public:
	void interact(sf::RenderWindow& window, const sf::Font& font);
	void keyPressed();
private:
	bool isOpen = false;
};

//class HUD{};


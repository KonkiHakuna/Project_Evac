#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

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
	void interact(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window, sf::Font& font, std::string name);
	void mouseClick(sf::Event::MouseButtonPressed const& e);
	bool getShoppingStatus();
	void setShoppingStatus(bool status);
private:
	bool isOpen = false;
	sf::RectangleShape shopOverlay;
	sf::RectangleShape shopCloseButton;
	sf::RectangleShape shopItemFrame_1;
	sf::RectangleShape shopItemFrame_2;
};

class MineEntrance {
public:
	void interact(sf::RenderWindow& window, sf::Font& font);
private:
};

//class HUD{};


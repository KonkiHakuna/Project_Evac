#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

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
	sf::Text saveInformations;
	int decision = NULL;
};
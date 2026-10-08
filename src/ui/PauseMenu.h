#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class PauseMenu {
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);

	void draw(sf::RenderWindow& window) const;

	// Checks whether one of the pause menu options was clicked.
	void mouseClick(sf::Event::MouseButtonPressed const& e);

	// Updates option colors when the mouse moves over them.
	void mouseMove(sf::Event::MouseMoved const& e);

	// Clears the previously selected menu option.
	void resetDecision() { decision = NULL; }

	// Returns the index of the selected option.
	int getDecision() const { return decision; }

private:
	// Text labels used to create the menu entries.
	std::vector<std::string> options;

	// Renderable versions of the pause menu options.
	std::vector<sf::Text> menuOptions;

	// Displays the save and load keyboard shortcuts.
	sf::Text saveInformations;

	// Stores the index of the clicked option:
	// 0 = Resume, 1 = Quit.
	int decision = NULL;
};
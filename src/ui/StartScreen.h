#pragma once

#include <SFML/Graphics.hpp>


class StartScreen {
public:
	StartScreen(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window) const;
private:
	sf::Text title;
	sf::Text pressSpace;
};
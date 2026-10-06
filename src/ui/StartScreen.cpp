
#include "StartScreen.h"

StartScreen::StartScreen(sf::RenderWindow& window, sf::Font& font) : title{ font,"Evac",256 }, pressSpace{ font,"Press space to continue",128 } {
	title.setFillColor(sf::Color::White);
	title.setOrigin({ title.getLocalBounds().size.x / 2,title.getLocalBounds().size.y / 2 });
	title.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });

	pressSpace.setFillColor(sf::Color::White);
	pressSpace.setOrigin({ pressSpace.getLocalBounds().size.x / 2,pressSpace.getLocalBounds().size.y / 2 });
	pressSpace.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) + 256 });
}

void StartScreen::draw(sf::RenderWindow& window) const {
	window.draw(title);;
	window.draw(pressSpace);
}
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class Item;
class Inventory;
class Player;

class InventoryItemOptions {
public:
	InventoryItemOptions(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window) const;
	bool getChoosingStatus() const;
	void setChoosingStatus(bool status);
	void setPosition(sf::Vector2f);
	void mouseClick(sf::Event::MouseButtonPressed const& e, Player& player, Inventory& inventory);
	void mouseMove(sf::Event::MouseMoved const& e);
	void setSelectedItem(Item* item);
private:
	bool isChoosing = false;
	sf::RectangleShape optionsCloseButton;
	std::vector<sf::Text> choicesText;
	std::vector<sf::RectangleShape> choicesButton;
	sf::RectangleShape choicesOverlay;
	Item* selectedItem = nullptr;
};

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class Item;
class Inventory;
class Player;


class InventoryItemOptions {
public:
	InventoryItemOptions(sf::RenderWindow& window, sf::Font& font);

	// Draws the small action menu for the selected inventory item.
	void draw(sf::RenderWindow& window) const;

	bool getChoosingStatus() const;
	void setChoosingStatus(bool status);

	// Positions the options menu relative to the selected inventory slot.
	void setPosition(sf::Vector2f);

	// Handles Equip, Remove and close-button clicks.
	void mouseClick(
		sf::Event::MouseButtonPressed const& e,
		Player& player,
		Inventory& inventory
	);

	// Updates hover colors for the available actions.
	void mouseMove(sf::Event::MouseMoved const& e);

	// Stores a non-owning pointer to the item currently being managed.
	void setSelectedItem(Item* item);

private:
	bool isChoosing = false;

	sf::RectangleShape optionsCloseButton;

	// Text labels representing the available item actions.
	std::vector<sf::Text> choicesText;

	// Reserved container for possible choice buttons.
	std::vector<sf::RectangleShape> choicesButton;

	sf::RectangleShape choicesOverlay;

	// Inventory remains the owner of the item.
	// This pointer only references the currently selected item.
	Item* selectedItem = nullptr;
};
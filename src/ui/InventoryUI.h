#pragma once

#include <SFML/Graphics.hpp>
#include <vector>


class Inventory;
class InventoryItemOptions;


class InventoryUI {
public:
	InventoryUI(
		sf::RenderWindow& window,
		sf::Font& font,
		InventoryItemOptions& options
	);

	// Draws the inventory window, item slots and stored items.
	void draw(sf::RenderWindow& window, Inventory& inventory) const;

	// Handles closing the inventory and selecting an item slot.
	void mouseClick(
		sf::Event::MouseButtonPressed const& e,
		Inventory& inventory
	);

	// Updates the close button hover state.
	void mouseMove(sf::Event::MouseMoved const& e);

	bool getInventoryStatus() const;
	void setInventoryStatus(bool status);

	// Returns the screen position of a selected inventory slot.
	sf::Vector2f getSlotPosition(sf::RectangleShape slot);

private:
	bool isOpen = false;

	sf::Text inventoryTitle;

	sf::RectangleShape inventoryOverlay;
	sf::RectangleShape inventoryCloseButton;

	// Stores all visible inventory slots.
	std::vector<sf::RectangleShape> itemSlots;

	// Temporary slot shape used while creating the slot layout.
	sf::RectangleShape itemSlot;

	// Reference to the separate menu used for actions on a selected item.
	InventoryItemOptions& itemOptions;
};
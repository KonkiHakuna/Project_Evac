#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Inventory;
class InventoryItemOptions;

class InventoryUI {
public:
	InventoryUI(sf::RenderWindow& window, sf::Font& font, InventoryItemOptions& options);
	void draw(sf::RenderWindow& window, Inventory& inventory) const;
	void mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory);
	void mouseMove(sf::Event::MouseMoved const& e);
	bool getInventoryStatus() const;
	void setInventoryStatus(bool status);
	sf::Vector2f getSlotPosition(sf::RectangleShape slot);
private:
	bool isOpen = false;
	sf::Text inventoryTitle;
	sf::RectangleShape inventoryOverlay;
	sf::RectangleShape inventoryCloseButton;
	std::vector<sf::RectangleShape> itemSlots;
	sf::RectangleShape itemSlot;
	InventoryItemOptions& itemOptions;
};
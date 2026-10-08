#pragma once

#include <vector>

#include "gameplay/Inventory.h"
#include "world/lobby.h"


class Shop {
public:
	Shop(sf::RenderWindow& window, sf::Font& font);

	// Creates the items available in each shop type.
	void initializeShopItems();

	// Displays the interaction prompt when the shop is closed.
	void interact(sf::RenderWindow& window, sf::Font& font) const;

	void draw(sf::RenderWindow& window, std::string name);

	// Handles clicks on the close button and item slots.
	void mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory);

	// Updates the visual state of buttons when the mouse moves.
	void mouseMove(sf::Event::MouseMoved const& e);

	bool getShoppingStatus() const;
	void setShoppingStatus(bool status);

	// Selects which shop inventory should currently be displayed.
	void setCurrentShopLocation(LobbyLocation location);

	// Attempts to buy the item stored in the selected frame.
	void buyItem(int itemFrame, Inventory& inventory);

	// Recreates the shop inventory after a successful purchase.
	void resetShopItems();

private:
	bool isOpen = false;

	sf::Text shopTitle;

	sf::RectangleShape shopOverlay;
	sf::RectangleShape shopCloseButton;

	// Two visible slots are currently available in the shop UI.
	sf::RectangleShape shopItemFrame_1;
	sf::RectangleShape shopItemFrame_2;

	// Each lobby shop location has its own list of items.
	// The unique_ptr owns the Item, while the int stores its price.
	std::unordered_map<
		LobbyLocation,
		std::vector<std::pair<std::unique_ptr<Item>, int>>
	> shopItems;

	LobbyLocation currentShopLocation = LobbyLocation::Default;
};
#pragma once

#include <vector>

#include "gameplay/Inventory.h"
#include "world/lobby.h"

class Shop {
public:
	Shop(sf::RenderWindow& window, sf::Font& font);
	void initializeShopItems();
	void interact(sf::RenderWindow& window, sf::Font& font) const;
	void draw(sf::RenderWindow& window, std::string name);
	void mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory);
	void mouseMove(sf::Event::MouseMoved const& e);
	bool getShoppingStatus() const;
	void setShoppingStatus(bool status);
	void setCurrentShopLocation(LobbyLocation location);
	void buyItem(int itemFrame, Inventory& inventory);
	void resetShopItems();
private:
	bool isOpen = false;
	sf::Text shopTitle;
	sf::RectangleShape shopOverlay;
	sf::RectangleShape shopCloseButton;
	sf::RectangleShape shopItemFrame_1;
	sf::RectangleShape shopItemFrame_2;
	std::unordered_map<LobbyLocation, std::vector<std::pair<std::unique_ptr<Item>, int>>> shopItems;
	LobbyLocation currentShopLocation = LobbyLocation::Default;
};
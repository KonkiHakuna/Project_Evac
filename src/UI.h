#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "Map.h"

class StartScreen {
public:
	StartScreen(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window);
private:
	sf::Text title;
	sf::Text pressSpace;
};

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
	int decision=NULL;
};

class Shop {
public:
	Shop(sf::RenderWindow& window, sf::Font& font);
	void initializeShopItems();
	void interact(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window, std::string name);
	void mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory);
	void mouseMove(sf::Event::MouseMoved const& e);
	bool getShoppingStatus();
	void setShoppingStatus(bool status);
	void setCurrentShopLocation(LobbyLocation location);
	void buyItem(int itemFrame, Inventory& inventory);
private:
	bool isOpen = false;
	sf::Text shopTitle;
	sf::RectangleShape shopOverlay;
	sf::RectangleShape shopCloseButton;
	sf::RectangleShape shopItemFrame_1;
	sf::RectangleShape shopItemFrame_2;
	std::unordered_map<LobbyLocation, std::vector<std::pair<std::unique_ptr<Item>,int>>> shopItems;
	LobbyLocation currentShopLocation = LobbyLocation::Default;
};

class MineEntrance {
public:
	void interact(sf::RenderWindow& window, sf::Font& font);
};

class InventoryUI {
public:
	InventoryUI(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window, Inventory& inventory);
	void mouseClick(sf::Event::MouseButtonPressed const& e);
	void mouseMove(sf::Event::MouseMoved const& e);
	bool getInventoryStatus();
	void setInventoryStatus(bool status);
private:
	bool isOpen = false;
	sf::Text inventoryTitle;
	sf::RectangleShape inventoryOverlay;
	sf::RectangleShape inventoryCloseButton;
	sf::RectangleShape itemSlot;
	std::vector<sf::RectangleShape> itemSlots;
};

class HUD {
public:
	HUD(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window, Player& player, Inventory& inventory);
	//void mouseClick(sf::Event::MouseButtonPressed const& e);
	//void mouseMove(sf::Event::MouseMoved const& e);
	void updateHealth(int health);
	void updateMana(int mana);
private:
	sf::RectangleShape currentWeaponSlot;
	sf::Text healthText;
	sf::Text manaText;
	sf::Text gold;
};


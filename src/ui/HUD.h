#pragma once

#include <SFML/Graphics.hpp>

class Player;
class Inventory;

class HUD {
public:
	HUD(sf::RenderWindow& window, sf::Font& font);
	void draw(sf::RenderWindow& window, Player& player, Inventory& inventory);
	void updateHealth(Player& player);
	void updateMana(Player& player);
private:
	sf::RectangleShape currentWeaponSlot;
	sf::RectangleShape currentArmorSlot;
	sf::RectangleShape currentSpellSlot;
	sf::Text healthText;
	sf::Text manaText;
	sf::Text gold;
};


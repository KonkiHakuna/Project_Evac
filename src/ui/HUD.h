#pragma once

#include <SFML/Graphics.hpp>


// Forward declarations are enough because HUD only uses
// Player and Inventory through references.
class Player;
class Inventory;


class HUD {
public:
	HUD(sf::RenderWindow& window, sf::Font& font);

	// Draws player stats and currently equipped items.
	void draw(sf::RenderWindow& window, Player& player, Inventory& inventory);

	// Refreshes individual stat labels when needed.
	void updateHealth(Player& player);
	void updateMana(Player& player);

private:
	// Slots used to display currently equipped items.
	sf::RectangleShape currentWeaponSlot;
	sf::RectangleShape currentArmorSlot;
	sf::RectangleShape currentSpellSlot;

	// Text elements shown on the HUD.
	sf::Text healthText;
	sf::Text manaText;
	sf::Text gold;
};
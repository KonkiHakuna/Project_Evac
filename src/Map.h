#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"

enum class LobbyLocation {
	weaponShop,
	armory,
	doctor,
	wizard,
	mineEntrance,
	Default
};

class Lobby {
public:
	Lobby(const sf::RenderWindow& window);
	void draw(sf::RenderWindow& window) const;
	LobbyLocation playerLocation(const Player& player);
private:
	LobbyLocation location = LobbyLocation::Default;

	sf::RectangleShape weaponShop;
	sf::RectangleShape armory;
	sf::RectangleShape doctor;
	sf::RectangleShape wizard;
	sf::RectangleShape mineEntrance;
};
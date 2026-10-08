#pragma once

#include <SFML/Graphics.hpp>


// Forward declaration is enough because Lobby only uses Player by reference.
class Player;


// Defines the interactive areas available in the lobby.
enum class LobbyLocation {
	weaponShop,
	armory,
	doctor,
	wizard,
	caveEntrance,
	Default
};


class Lobby {
public:
	Lobby(const sf::RenderWindow& window);

	void draw(sf::RenderWindow& window) const;

	// Checks which interactive lobby area currently contains the player.
	LobbyLocation playerLocation(const Player& player);

	// Converts a lobby location into a display name.
	std::string getName(LobbyLocation location);

private:
	// Stores the player's currently detected lobby area.
	LobbyLocation location = LobbyLocation::Default;

	// Simple rectangle shapes represent interactive lobby locations.
	sf::RectangleShape weaponShop;
	sf::RectangleShape armory;
	sf::RectangleShape doctor;
	sf::RectangleShape wizard;
	sf::RectangleShape caveEntrance;
};
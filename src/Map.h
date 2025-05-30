#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"

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
	LobbyLocation playerLocation(const Player& player);
	std::string getName(LobbyLocation location);
private:
	LobbyLocation location = LobbyLocation::Default;

	sf::RectangleShape weaponShop;
	sf::RectangleShape armory;
	sf::RectangleShape doctor;
	sf::RectangleShape wizard;
	sf::RectangleShape caveEntrance;
};

class Cave {
public:
	Cave(sf::RenderWindow& window,sf::Font& font);
	void draw(sf::RenderWindow& window) const;
	void clear();
	std::vector<std::unique_ptr<Enemy>>& getEnemies();
	void levelUpdate();
	int getCurrentWave() const;
private:
	sf::Text waveCounter;
	sf::Font caveatFont;
	sf::Clock waveTextTimer;
	bool showWaveCounter = false;
	int caveWave=1;
	std::vector<std::unique_ptr<Enemy>>enemies;
	sf::Clock waveTimer;
	bool nextWaveAproaching = false;
};
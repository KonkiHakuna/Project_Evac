#pragma once

#include <SFML/Graphics.hpp>

#include "entities/Player.h"

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
	bool showWaveCounter = false;
	int caveWave=1;
	std::vector<std::unique_ptr<Enemy>>enemies;
	sf::Clock waveTimer;
	bool nextWaveAproaching = false;
};
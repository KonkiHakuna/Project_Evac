#pragma once

#include <SFML/Graphics.hpp>

#include "entities/Player.h"


class Cave {
public:
	Cave(sf::RenderWindow& window, sf::Font& font);

	void draw(sf::RenderWindow& window) const;

	// Resets the cave back to its initial wave.
	void clear();

	// Returns the actual enemy container, not a copy.
	std::vector<std::unique_ptr<Enemy>>& getEnemies();

	// Handles transitions between completed waves.
	void levelUpdate();

	int getCurrentWave() const;

private:
	sf::Text waveCounter;
	sf::Font caveatFont;

	bool showWaveCounter = false;

	int caveWave = 1;

	// Cave owns all enemies currently active in the wave.
	std::vector<std::unique_ptr<Enemy>>enemies;

	// Measures the delay before the next wave starts.
	sf::Clock waveTimer;

	bool nextWaveAproaching = false;
};
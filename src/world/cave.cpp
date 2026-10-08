#include "core/Game.h"
#include "cave.h"


Cave::Cave(sf::RenderWindow& window, sf::Font& font)
	: waveCounter{ font,"",256 } {

	waveCounter.setFillColor(sf::Color::White);

	waveCounter.setOrigin({
		waveCounter.getLocalBounds().size.x / 2,
		waveCounter.getLocalBounds().size.y / 2
		});

	waveCounter.setPosition({
		static_cast<float>(window.getSize().x / 2 - 256),
		static_cast<float>(64)
		});


	// Create the enemies for the initial wave.
	enemies = Enemy::createEnemies(caveWave);

	showWaveCounter = true;
}


void Cave::draw(sf::RenderWindow& window) const {

	// Cave uses its own background color.
	window.clear(sf::Color(75, 40, 0));

	window.draw(waveCounter);
}


void Cave::clear() {

	// Reset wave progression.
	caveWave = 1;


	// Destroy all current enemies.
	enemies.clear();


	// Create a fresh first wave.
	enemies = Enemy::createEnemies(caveWave);


	waveCounter.setString(
		"Wave " + std::to_string(caveWave)
	);

	showWaveCounter = true;

	nextWaveAproaching = false;
}


std::vector<std::unique_ptr<Enemy>>& Cave::getEnemies() {

	// Returning by reference allows Game to modify
	// the same enemy vector owned by Cave.
	return enemies;
}


void Cave::levelUpdate() {

	// When every enemy is defeated, start waiting
	// before spawning the next wave.
	if (enemies.empty() && !nextWaveAproaching) {

		nextWaveAproaching = true;

		waveTimer.restart();
	}


	// Spawn the next wave after a three-second delay.
	if (
		nextWaveAproaching
		&& waveTimer.getElapsedTime().asSeconds() >= 3
		) {

		caveWave++;


		// Replacing the vector destroys the old owned enemies
		// and transfers ownership of the newly created ones here.
		enemies = Enemy::createEnemies(caveWave);


		nextWaveAproaching = false;


		waveCounter.setString(
			"Wave " + std::to_string(caveWave)
		);

		showWaveCounter = true;
	}
}


int Cave::getCurrentWave() const {
	return caveWave;
}
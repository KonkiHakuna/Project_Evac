
#include "core/Game.h"
#include "cave.h"

Cave::Cave(sf::RenderWindow& window,sf::Font& font): waveCounter{font,"",256} {
	waveCounter.setFillColor(sf::Color::White);
	waveCounter.setOrigin({waveCounter.getLocalBounds().size.x / 2, waveCounter.getLocalBounds().size.y / 2});
	waveCounter.setPosition({static_cast<float>(window.getSize().x / 2 - 256), static_cast<float>(64)});
	enemies = Enemy::createEnemies(caveWave);
	showWaveCounter = true;
}

void Cave::draw(sf::RenderWindow& window) const {
	window.clear(sf::Color(75,40,0));
	window.draw(waveCounter);
}

void Cave::clear() {
	caveWave = 1;
	enemies.clear();
	enemies=Enemy::createEnemies(caveWave);
	waveCounter.setString("Wave " + std::to_string(caveWave));
	showWaveCounter = true;
	nextWaveAproaching = false;
}

std::vector<std::unique_ptr<Enemy>>& Cave::getEnemies() {
	return enemies;
}

void Cave::levelUpdate() {
	if (enemies.empty() && !nextWaveAproaching) {
		nextWaveAproaching = true;
		waveTimer.restart();
	}
	if (nextWaveAproaching && waveTimer.getElapsedTime().asSeconds() >= 3) {
		caveWave++;
		enemies = Enemy::createEnemies(caveWave);
		nextWaveAproaching = false;
		waveCounter.setString("Wave " + std::to_string(caveWave));
		showWaveCounter = true;
	}
}

int Cave::getCurrentWave() const {
	return caveWave;
}

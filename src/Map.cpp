
#include "Map.h"
#include "Game.h"

/*============================== Lobby =============================*/

Lobby::Lobby(const sf::RenderWindow& window) {
	weaponShop.setSize(sf::Vector2f(256, 256));
	weaponShop.setFillColor(sf::Color(211,211,211));
	weaponShop.setOrigin({ static_cast<float>(weaponShop.getLocalBounds().size.x / 2),static_cast<float>(weaponShop.getLocalBounds().size.y) / 2 });
	weaponShop.setPosition({ static_cast<float>(window.getSize().x * 0.25),static_cast<float>(window.getSize().y)-125});

	armory.setSize(sf::Vector2f(256, 256));
	armory.setFillColor(sf::Color(111,111,111));
	armory.setOrigin({ static_cast<float>(armory.getLocalBounds().size.x / 2),static_cast<float>(armory.getLocalBounds().size.y) / 2 });
	armory.setPosition({ static_cast<float>(window.getSize().x * 0.75),static_cast<float>(window.getSize().y)-125});

	doctor.setSize(sf::Vector2f(256, 256));
	doctor.setFillColor(sf::Color::White);
	doctor.setOrigin({ static_cast<float>(doctor.getLocalBounds().size.x / 2),static_cast<float>(doctor.getLocalBounds().size.y) / 2 });
	doctor.setPosition({static_cast<float>(window.getSize().x)-125,static_cast<float>(window.getSize().y/2)});

	wizard.setSize(sf::Vector2f(256, 256));
	wizard.setFillColor(sf::Color(110,130,255));
	wizard.setOrigin({ static_cast<float>(wizard.getLocalBounds().size.x / 2)-125,static_cast<float>(wizard.getLocalBounds().size.y) / 2 });
	wizard.setPosition({ 0,static_cast<float>(window.getSize().y / 2)});

	caveEntrance.setSize(sf::Vector2f(256, 256));
	caveEntrance.setFillColor(sf::Color::Magenta);
	caveEntrance.setOrigin({ static_cast<float>(caveEntrance.getLocalBounds().size.x / 2),static_cast<float>(caveEntrance.getLocalBounds().size.y) / 2 });
	caveEntrance.setPosition({ static_cast<float>(window.getSize().x / 2),125});
}

void Lobby::draw(sf::RenderWindow& window) const {
	window.clear(sf::Color(160,80,0));
	window.draw(weaponShop);
	window.draw(armory);
	window.draw(doctor);
	window.draw(wizard);
	window.draw(caveEntrance);
}


LobbyLocation Lobby::playerLocation(const Player& player) {
	sf::Vector2f playerPosition = player.getPosition();
	if (weaponShop.getGlobalBounds().contains(playerPosition)) {
	
		location = LobbyLocation::weaponShop;
	}
	else if (armory.getGlobalBounds().contains(playerPosition)) {
		location = LobbyLocation::armory;
	}
	else if (doctor.getGlobalBounds().contains(playerPosition)) {
		location = LobbyLocation::doctor;
	}
	else if (wizard.getGlobalBounds().contains(playerPosition)) {
		location = LobbyLocation::wizard;
	}
	else if (caveEntrance.getGlobalBounds().contains(playerPosition)) {
		location = LobbyLocation::caveEntrance;
	}
	else {
		location = LobbyLocation::Default;
	}
	return location;
}

std::string Lobby::getName(LobbyLocation location) {
	switch (location) {
	case LobbyLocation::weaponShop:
		return "Weapons";
	case LobbyLocation::armory:
		return "Armory";
	case LobbyLocation::doctor:
		return "Doctor";
	case LobbyLocation::wizard:
		return "Wizard";
	case LobbyLocation::caveEntrance:
		return "Cave";
	default:
		return "Default";
	}
}

/*============================== Mine =============================*/

Cave::Cave(sf::RenderWindow& window,sf::Font& font): waveCounter{font,"",256} {
	waveCounter.setFillColor(sf::Color::White);
	waveCounter.setOrigin({waveCounter.getLocalBounds().size.x / 2, waveCounter.getLocalBounds().size.y / 2});
	waveCounter.setPosition({static_cast<float>(window.getSize().x / 2 - 256), static_cast<float>(64)});
	enemies = Enemy::createEnemies(caveWave);
	showWaveCounter = true;
	waveTextTimer.restart();
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
	waveTextTimer.restart();
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
		waveTextTimer.restart();
	}
}

int Cave::getCurrentWave() const {
	return caveWave;
}


#include "Map.h"
#include "Game.h"

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

	mineEntrance.setSize(sf::Vector2f(256, 256));
	mineEntrance.setFillColor(sf::Color::Magenta);
	mineEntrance.setOrigin({ static_cast<float>(mineEntrance.getLocalBounds().size.x / 2),static_cast<float>(mineEntrance.getLocalBounds().size.y) / 2 });
	mineEntrance.setPosition({ static_cast<float>(window.getSize().x / 2),125});
}

void Lobby::draw(sf::RenderWindow& window) const {
	window.clear(sf::Color(160,80,0));

	window.draw(weaponShop);
	window.draw(armory);
	window.draw(doctor);
	window.draw(wizard);
	window.draw(mineEntrance);
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
	else if (mineEntrance.getGlobalBounds().contains(playerPosition)) {
		location = LobbyLocation::mineEntrance;
	}
	else {
		location = LobbyLocation::Default;
	}
	return location;
}
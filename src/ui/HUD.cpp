
#include "HUD.h"
#include "entities/Player.h"
#include "gameplay/Inventory.h"


HUD::HUD(sf::RenderWindow& window, sf::Font& font) : healthText(font), manaText(font), gold(font) {
	healthText.setCharacterSize(128);
	healthText.setFillColor(sf::Color::Red);
	healthText.setOrigin({healthText.getLocalBounds().size.x / 2,healthText.getLocalBounds().size.y / 2});
	healthText.setPosition({0,0});

	manaText.setCharacterSize(128);
	manaText.setFillColor(sf::Color::Blue);
	manaText.setOrigin({manaText.getLocalBounds().size.x / 2,manaText.getLocalBounds().size.y / 2});
	manaText.setPosition({0,128});

	gold.setCharacterSize(128);
	gold.setFillColor(sf::Color::Yellow);
	gold.setOrigin({gold.getLocalBounds().size.x / 2,gold.getLocalBounds().size.y / 2});
	gold.setPosition({ 0,256 });

	currentWeaponSlot.setSize(sf::Vector2f{ 256,256 });
	currentWeaponSlot.setFillColor(sf::Color(74, 74, 74));
	currentWeaponSlot.setOrigin({currentWeaponSlot.getLocalBounds().size.x / 2,currentWeaponSlot.getLocalBounds().size.y / 2});
	currentWeaponSlot.setPosition({static_cast<float>(window.getSize().x) - 192,static_cast<float>(window.getSize().y) - 192});

	currentArmorSlot.setSize(sf::Vector2f{ 256,256 });
	currentArmorSlot.setFillColor(sf::Color(74, 74, 74));
	currentArmorSlot.setOrigin({ currentArmorSlot.getLocalBounds().size.x / 2,currentArmorSlot.getLocalBounds().size.y / 2 });
	currentArmorSlot.setPosition({ static_cast<float>(window.getSize().x) - 192,static_cast<float>(192)});

	currentSpellSlot.setSize(sf::Vector2f{ 256,256 });
	currentSpellSlot.setFillColor(sf::Color(74, 74, 74));
	currentSpellSlot.setOrigin({ currentSpellSlot.getLocalBounds().size.x / 2,currentSpellSlot.getLocalBounds().size.y / 2 });
	currentSpellSlot.setPosition({ static_cast<float>(192),static_cast<float>(window.getSize().y) - 192 });
}

void HUD::draw(sf::RenderWindow& window, Player& player, Inventory& inventory) {
	healthText.setString("Health: " + std::to_string(player.getHealth()));
	manaText.setString("Mana: " + std::to_string(player.getMana()));
	gold.setString("Gold: " + std::to_string(inventory.getPlayerGold()));
	window.draw(healthText);
	window.draw(manaText);
	window.draw(gold);
	window.draw(currentWeaponSlot);
	window.draw(currentArmorSlot);
	window.draw(currentSpellSlot);
	if (auto currentWeapon = inventory.getCurrentWeapon()) {
		currentWeapon->setPosition({ static_cast<float>(currentWeaponSlot.getPosition().x),static_cast<float>(currentWeaponSlot.getPosition().y) });
		currentWeapon->draw(window);
	}
	if (auto currentArmor = inventory.getCurrentArmor()) {
		currentArmor->setPosition({ static_cast<float>(currentArmorSlot.getPosition().x),static_cast<float>(currentArmorSlot.getPosition().y) });
		currentArmor->draw(window);
	}
	if (auto currentSpell = inventory.getCurrentSpell()) {
		currentSpell->setPosition({ static_cast<float>(currentSpellSlot.getPosition().x),static_cast<float>(currentSpellSlot.getPosition().y) });
		currentSpell->draw(window);
	}
}

void HUD::updateHealth(Player& player) {
	healthText.setString("Health: " + std::to_string(player.getHealth()));
}

void HUD::updateMana(Player& player) {
	manaText.setString("Mana: " + std::to_string(player.getMana()));
}
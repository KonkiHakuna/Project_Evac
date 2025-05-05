
#include "Item.h"


Item::Item(std::string name, std::filesystem::path file) : name(name), sprite(texture) {
	if (!texture.loadFromFile(file)) {
		throw std::exception("Failed to load texture");
	}
	sprite = sf::Sprite(texture);
	sprite.setScale(sf::Vector2f(7, 7));
	sprite.setOrigin({ static_cast<float>(sprite.getLocalBounds().size.x) / 2,static_cast<float>(sprite.getLocalBounds().size.y) / 2 });
}

void Item::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

void Item::setPosition(sf::Vector2f position) {
	sprite.setPosition(position);
}

Weapon::Weapon(std::string name, std::filesystem::path file, int damage, float range) : Item(name, file), damage(damage), range(range) {}
Armor::Armor(std::string name, std::filesystem::path file, int defense) : Item(name, file), defense(defense) {}
Potion::Potion(std::string name, std::filesystem::path file, int healingPower) : Item(name, file), healingPower(healingPower) {}
Spell::Spell(std::string name, std::filesystem::path file, int damage, int manaCost) : Item(name, file), damage(damage), manaCost(manaCost) {}
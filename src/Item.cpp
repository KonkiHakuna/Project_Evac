
#include "Item.h"


Item::Item(std::string name, std::filesystem::path file) : name(name), file(file), sprite(texture) {
	if (!texture.loadFromFile(file)) {
		throw std::exception("Failed to load texture");
	}
	sprite = sf::Sprite(texture);
	sprite.setScale(sf::Vector2f(2, 2));
	sprite.setOrigin({ static_cast<float>(sprite.getLocalBounds().size.x) / 2,static_cast<float>(sprite.getLocalBounds().size.y) / 2 });
}

void Item::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

void Item::setPosition(sf::Vector2f position) {
	sprite.setPosition(position);
}

void Item::equip(Player& player, Inventory& inventory) {}
std::string Item::getName() const {
	return name;
}
std::filesystem::path Item::getPath() const {
	return file;
}


Weapon::Weapon(std::string name, std::filesystem::path file, int damage, bool melee) : Item(name, file), damage(damage), melee(melee) {}
void Weapon::equip(Player& player, Inventory& inventory) {
	inventory.setCurrentWeapon(this);
}
bool Weapon::isMelee() const {
	return melee;
}
int Weapon::getDamage() {
	return damage;
}

Armor::Armor(std::string name, std::filesystem::path file, int defense) : Item(name, file), defense(defense) {}
void Armor::equip(Player& player, Inventory& inventory) {
	inventory.setCurrentArmor(this);
	player.setDefense(defense);
}
int Armor::getDefense() {
	return defense;
}
Potion::Potion(std::string name, std::filesystem::path file, int power, TypeOfPotion type) : Item(name, file), power(power), type(type) {}
void Potion::equip(Player& player, Inventory& inventory) {
	if (type == TypeOfPotion::Healing) {
		player.setHealth(power, '+');
	}
	else if (type == TypeOfPotion::Mana) {
		player.setMana(power, '+');
	}
	inventory.removeItem(this);
}
int Potion::getPower() {
	return power;
}
TypeOfPotion Potion::getType() {
	return type;
}

Spell::Spell(std::string name, std::filesystem::path file, int damage, int manaCost) : Item(name, file), damage(damage), manaCost(manaCost) {}
void Spell::equip(Player& player, Inventory& inventory) {
	inventory.setCurrentSpell(this);
}
int Spell::getDamage() {
	return damage;
}
int Spell::getManaCost() {
	return manaCost;
}
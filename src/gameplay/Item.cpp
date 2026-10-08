#include "Item.h"
#include "gameplay/Inventory.h"
#include "entities/Player.h"


Item::Item(std::string name, std::filesystem::path file)
	: name(name), file(file), sprite(texture) {

	// Load the texture belonging to this item.
	if (!texture.loadFromFile(file)) {
		throw std::exception("Failed to load texture");
	}


	// Create the visible sprite from the loaded texture.
	sprite = sf::Sprite(texture);

	sprite.setScale(sf::Vector2f(2, 2));


	// Center the sprite origin so positioning works from its center.
	sprite.setOrigin({
		static_cast<float>(sprite.getLocalBounds().size.x) / 2,
		static_cast<float>(sprite.getLocalBounds().size.y) / 2
		});
}


void Item::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}


void Item::setPosition(sf::Vector2f position) {
	sprite.setPosition(position);
}


// Base Item has no default equip behavior.
// Derived classes provide the actual implementation.
void Item::equip(Player& player, Inventory& inventory) {}


std::string Item::getName() const {
	return name;
}


std::filesystem::path Item::getPath() const {
	return file;
}


// =========================
// Weapon
// =========================

Weapon::Weapon(
	std::string name,
	std::filesystem::path file,
	int damage,
	bool melee
)
	: Item(name, file), damage(damage), melee(melee) {}


void Weapon::equip(Player& player, Inventory& inventory) {

	// Inventory keeps a non-owning pointer to the equipped weapon.
	// The actual object is still owned by the inventory item collection.
	inventory.setCurrentWeapon(this);
}


bool Weapon::isMelee() const {
	return melee;
}


int Weapon::getDamage() {
	return damage;
}


// =========================
// Armor
// =========================

Armor::Armor(
	std::string name,
	std::filesystem::path file,
	int defence
)
	: Item(name, file), defence(defence) {}


void Armor::equip(Player& player, Inventory& inventory) {

	// Store this armor as equipped and immediately
	// apply its defence value to the player.
	inventory.setCurrentArmor(this);

	player.setDefence(defence);
}


int Armor::getDefence() {
	return defence;
}


// =========================
// Potion
// =========================

Potion::Potion(
	std::string name,
	std::filesystem::path file,
	int power,
	TypeOfPotion type
)
	: Item(name, file), power(power), type(type) {}


void Potion::equip(Player& player, Inventory& inventory) {

	// Potion behavior depends on its stored type.
	if (type == TypeOfPotion::Healing) {

		player.heal(power);
	}

	else if (type == TypeOfPotion::Mana) {

		player.restoreMana(power);
	}


	// Potions are consumable, so remove the item after using it.
	// 'this' is a pointer to the current Potion object.
	inventory.removeItem(this);
}


int Potion::getPower() {
	return power;
}


TypeOfPotion Potion::getType() {
	return type;
}


// =========================
// Spell
// =========================

Spell::Spell(
	std::string name,
	std::filesystem::path file,
	int damage,
	int manaCost
)
	: Item(name, file), damage(damage), manaCost(manaCost) {}


void Spell::equip(Player& player, Inventory& inventory) {

	// Selecting a spell only changes the inventory's
	// reference to the currently equipped spell.
	inventory.setCurrentSpell(this);
}


int Spell::getDamage() {
	return damage;
}


int Spell::getManaCost() {
	return manaCost;
}
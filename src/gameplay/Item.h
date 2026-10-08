#pragma once

#include <SFML/Graphics.hpp>


// Defines the two potion effects supported by the game.
enum class TypeOfPotion {
	Healing,
	Mana
};


// Forward declarations are enough because Item only uses
// Player and Inventory through references in function declarations.
class Player;
class Inventory;


// Base class for every item stored in the inventory.
class Item {
public:

	// Virtual destructor is required because derived objects
	// can be owned and deleted through Item pointers.
	virtual ~Item() = default;

	Item(std::string name, std::filesystem::path file);

	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f position);

	// Derived item types override this method
	// to provide their own equip/use behavior.
	virtual void equip(Player& player, Inventory& inventory);

	std::string getName() const;
	std::filesystem::path getPath() const;

protected:
	std::string name;
	std::filesystem::path file;

	// Each item owns its texture and the sprite that uses it.
	sf::Texture texture;
	sf::Sprite sprite;
};


// Weapon extends Item with combat-related properties.
class Weapon : public Item {
public:
	Weapon(std::string name, std::filesystem::path file, int damage, bool melee);

	void equip(Player& player, Inventory& inventory) override;

	bool isMelee() const;
	int getDamage();

private:
	int damage;

	// Distinguishes melee weapons from ranged weapons.
	bool melee;
};


// Armor adds defence to the base Item functionality.
class Armor : public Item {
public:
	Armor(std::string name, std::filesystem::path file, int defence);

	void equip(Player& player, Inventory& inventory) override;

	int getDefence();

private:
	int defence;
};


// Potions are consumable items that restore either health or mana.
class Potion : public Item {
public:
	Potion(std::string name, std::filesystem::path file, int power, TypeOfPotion type);

	void equip(Player& player, Inventory& inventory) override;

	int getPower();
	TypeOfPotion getType();

private:
	int power;
	TypeOfPotion type;
};


// Spell stores its damage and mana cost
// and can be selected as the player's current spell.
class Spell : public Item {
public:
	Spell(std::string name, std::filesystem::path file, int damage, int manaCost);

	void equip(Player& player, Inventory& inventory) override;

	int getDamage();
	int getManaCost();

private:
	int damage;
	int manaCost;
};
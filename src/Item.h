#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"

enum class TypeOfPotion {
	Healing,
	Mana
};

class Player;
class Inventory;

class Item {
public:
	virtual ~Item() = default;
	Item(std::string name, std::filesystem::path file);
	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f position);
	virtual void equip(Player& player, Inventory& inventory);
	std::string getName() const;
	std::filesystem::path getPath() const;

protected:
	std::string name;
	std::filesystem::path file;
	sf::Texture texture;
	sf::Sprite sprite;
};

class Weapon : public Item {
public:
	Weapon(std::string name, std::filesystem::path file, int damage, bool melee);
	void equip(Player& player, Inventory& inventory) override;
	bool isMelee() const;
	int getDamage();
private:
	int damage;
	bool melee;
};

class Armor : public Item {
public:
	Armor(std::string name, std::filesystem::path file, int defense);
	void equip(Player& player, Inventory& inventory) override;
	int getDefense();

private:
	int defense;
};

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

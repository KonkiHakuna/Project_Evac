#pragma once

#include <SFML/Graphics.hpp>

class Item {
public:
	Item(std::string name, std::filesystem::path file);
	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f position);
protected:
	std::string name;
	sf::Texture texture;
	sf::Sprite sprite;
};

class Weapon : public Item {
public:
	Weapon(std::string name, std::filesystem::path file, int damage, float range);
private:
	int damage;
	float range;
};

class Armor : public Item {
public:
	Armor(std::string name, std::filesystem::path file, int defense);
private:
	int defense;
};

class Potion : public Item {
public:
	Potion(std::string name, std::filesystem::path file, int healingPower);
private:
	int healingPower;
};

class Spell : public Item {
public:
	Spell(std::string name, std::filesystem::path file, int damage, int manaCost);
private:
	int damage;
	int manaCost;
};

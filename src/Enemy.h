#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"

enum class EnemyType {
	NORMAL,
	QUICK,
	TANK,
	BOSS
};
class Player;
class Enemy {
public:
	Enemy(EnemyType type, sf::Vector2f position);
	void initialize(EnemyType type);
	void movement(sf::Vector2f playerPosition);
	int getHealth() const;
	void setHealth(int value, char op);
	void draw(sf::RenderWindow& window) const;
	bool isDead() const;
	int getDamage() const;
	void attack(Player& player);
	static std::vector<std::unique_ptr<Enemy>> createEnemies(int level);
	sf::FloatRect getGlobalBounds() const;
	EnemyType getType() const;
private:
	EnemyType type;
	sf::Clock attackCooldown;
	int damage;
	int health;
	int speed;
	sf::CircleShape enemy;
};
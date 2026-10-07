#pragma once

#include <SFML/Graphics.hpp>


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
	void takeDamage(int damage);

	void draw(sf::RenderWindow& window) const;

	bool isDead() const;
	int getDamage() const;

	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;

	void attack(Player& player);

	static std::vector<std::unique_ptr<Enemy>> createEnemies(int level);

	sf::FloatRect getGlobalBounds() const;
	EnemyType getType() const;

private:
	void updateAnimation();

	EnemyType type;

	sf::Clock attackCooldown;
	sf::Clock animationClock;

	int damage;
	int health;
	int speed;

	sf::CircleShape enemy;

	sf::Texture enemyIdleTexture;
	sf::Texture enemyRunTexture;
	sf::Sprite enemySprite;

	int currentFrame = 0;
	bool isMoving = false;
	bool wasMoving = false;
	bool FacingRight = true;

	float size = 2.5f;
};
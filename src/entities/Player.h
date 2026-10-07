#pragma once

#include <SFML/Graphics.hpp>

class Item;
class Inventory;
class Enemy;

class Player {
public:
	Player(sf::RenderWindow& window);
	void movement();
	void movementBounds();
	void dash();
	void draw(sf::RenderWindow& window);
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;

	int getHealth() const;
	void takeDamage(int damage);
	void heal(int amount);
	void setHealth(int newHealth);

	int getMana() const;
	void useMana(int amount);
	void restoreMana(int amount);
	void setMana(int mana);

	void setDefence(int value);
	int getDefence() const;

	void attack(sf::Event::MouseButtonPressed const& e, Inventory& inventory, std::vector<std::unique_ptr<Enemy>>& enemies);
	sf::FloatRect getGlobalBounds() const;



private:
	int health;
	int mana;
	int defence;
	sf::Vector2f velocity;
	sf::CircleShape player;

	sf::Texture playerIdleTexture;
	sf::Texture playerRunTexture;
	sf::Sprite playerSprite;

	sf::Clock animationClock;

	int currentFrame = 0;
	bool isMoving = false;
	bool wasMoving = false;
	void updateAnimation();

	sf::CircleShape attackPlayerHitbox;
	sf::Clock attackTimer;
	bool drawAttackPlayerHitbox = false;
};
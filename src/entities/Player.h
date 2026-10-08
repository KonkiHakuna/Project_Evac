#pragma once

#include <SFML/Graphics.hpp>


// Forward declarations avoid including the full class definitions
// when only pointers or references are needed in this header.
class Item;
class Inventory;
class Enemy;
class Projectile;


class Player {
public:
	Player(sf::RenderWindow& window);


	// Movement and position.
	void movement();
	void movementBounds();
	void dash();
	void draw(sf::RenderWindow& window);
	void resetPosition(sf::RenderWindow& window);
	sf::Vector2f getPosition() const;


	// Health management.
	int getHealth() const;
	void takeDamage(int damage);
	void heal(int amount);
	void setHealth(int newHealth);


	// Mana management.
	int getMana() const;
	void useMana(int amount);
	void restoreMana(int amount);
	void setMana(int mana);


	// Defence value provided by the currently equipped armor.
	void setDefence(int value);
	int getDefence() const;


	// Handles both melee and ranged attacks depending
	// on the weapon currently equipped in the inventory.
	void attack(
		sf::Event::MouseButtonPressed const& e,
		Inventory& inventory,
		std::vector<std::unique_ptr<Enemy>>& enemies,
		std::vector<Projectile>& projectiles,
		sf::Vector2f targetPosition
	);


	sf::FloatRect getGlobalBounds() const;


private:

	int health;
	int mana;
	int defence;

	sf::Vector2f velocity;


	// Circle used as the player's physical position and collision shape.
	sf::CircleShape player;


	// Textures used by the player's animations.
	sf::Texture playerIdleTexture;
	sf::Texture playerRunTexture;
	sf::Sprite playerSprite;


	// Controls the timing between animation frames.
	sf::Clock animationClock;

	int currentFrame = 0;

	bool isMoving = false;
	bool wasMoving = false;

	void updateAnimation();

	bool facingRight = true;


	// Temporary hitbox used for melee attacks.
	sf::CircleShape attackPlayerHitbox;

	// Controls the attack cooldown and melee hitbox display time.
	sf::Clock attackTimer;

	bool drawAttackPlayerHitbox = false;


	sf::Texture playerAttackTexture;

	// Prevents movement/idle animation from replacing
	// the attack animation before it finishes.
	bool isAttacking = false;
};
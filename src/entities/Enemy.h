#pragma once

#include <SFML/Graphics.hpp>


// Defines the available enemy variants.
// The selected type determines stats, size and textures.
enum class EnemyType {
	NORMAL,
	QUICK,
	TANK,
	BOSS
};


// Forward declaration is enough here because Player
// is only used as a reference in the class interface.
class Player;


class Enemy {
public:
	Enemy(EnemyType type, sf::Vector2f position);

	// Configures stats, textures and size for the selected enemy type.
	void initialize(EnemyType type);

	// Moves the enemy toward the player's current position.
	void movement(sf::Vector2f playerPosition);

	int getHealth() const;

	// Reduces health by the given amount.
	void takeDamage(int damage);

	void draw(sf::RenderWindow& window) const;

	bool isDead() const;
	int getDamage() const;

	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition() const;

	// Attempts to damage the player when they are in contact.
	void attack(Player& player);

	// Creates all enemies required for the given level.
	// unique_ptr gives the vector ownership of each created Enemy.
	static std::vector<std::unique_ptr<Enemy>> createEnemies(int level);

	sf::FloatRect getGlobalBounds() const;
	EnemyType getType() const;

private:
	// Updates either the idle or running sprite-sheet animation.
	void updateAnimation();

	EnemyType type;

	// Separate clocks are used for attack cooldown and animation timing.
	sf::Clock attackCooldown;
	sf::Clock animationClock;

	int damage;
	int health;
	int speed;

	// Used as the logical position and collision shape.
	sf::CircleShape enemy;

	// Textures and sprite used for the visible enemy animation.
	sf::Texture enemyIdleTexture;
	sf::Texture enemyRunTexture;
	sf::Sprite enemySprite;

	int currentFrame = 0;

	// Tracks transitions between idle and movement animations.
	bool isMoving = false;
	bool wasMoving = false;

	// Controls horizontal sprite orientation.
	bool FacingRight = true;

	// Different enemy types use different visual scales.
	float size = 2.5f;
};
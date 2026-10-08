#include  "Enemy.h"
#include "Player.h"


Enemy::Enemy(EnemyType type, sf::Vector2f position)
	: type(type), enemySprite(enemyIdleTexture) {

	// Configure the selected enemy type before placing it in the world.
	initialize(type);

	enemy.setPosition(position);
	enemySprite.setPosition(position);
}


void Enemy::initialize(EnemyType type) {

	// Each enemy type loads its textures from a separate folder.
	std::string textureFolderPath;


	// Configure gameplay stats and visual size for the selected type.
	switch (type) {

	case EnemyType::NORMAL:
		enemy.setRadius(64);
		enemy.setFillColor(sf::Color::Yellow);
		damage = 15;
		health = 200;
		speed = 2;
		textureFolderPath = "assets/entities/enemy/normal/";
		size = 2.5f;
		break;

	case EnemyType::QUICK:
		enemy.setRadius(48);
		enemy.setFillColor(sf::Color::Blue);
		damage = 10;
		health = 100;
		speed = 5;
		textureFolderPath = "assets/entities/enemy/quick/";
		size = 2.0f;
		break;

	case EnemyType::TANK:
		enemy.setRadius(96);
		enemy.setFillColor(sf::Color(153, 0, 0));
		damage = 50;
		health = 500;
		speed = 1;
		textureFolderPath = "assets/entities/enemy/tank/";
		size = 3.0f;
		break;

	case EnemyType::BOSS:
		enemy.setRadius(128);
		enemy.setFillColor(sf::Color(153, 0, 76));
		damage = 75;
		health = 2000;
		speed = 1;
		textureFolderPath = "assets/entities/enemy/boss/";
		size = 4.0f;
		break;

	default:
		throw std::exception("Invalid type");
	}


	// Center the logical collision shape around its position.
	enemy.setOrigin({ enemy.getRadius(), enemy.getRadius() });


	// Load the animation textures selected for this enemy type.
	if (!enemyIdleTexture.loadFromFile(textureFolderPath + "Warrior_Idle.png")) {
		throw std::exception("Failed to load enemy idle texture");
	}

	if (!enemyRunTexture.loadFromFile(textureFolderPath + "Warrior_Run.png")) {
		throw std::exception("Failed to load enemy run texture");
	}


	// Start with the first frame of the idle animation.
	enemySprite.setTexture(enemyIdleTexture, true);
	enemySprite.setTextureRect({ {0, 0}, {192, 192} });

	// Each frame is 192x192, so the center is 96x96.
	enemySprite.setOrigin({ 96.f, 96.f });

	enemySprite.setScale({ size, size });
}


void Enemy::movement(sf::Vector2f playerPosition) {

	// Assume the enemy is idle until movement is required.
	isMoving = false;


	// Move independently on each axis toward the player.
	if (playerPosition.x > enemy.getPosition().x) {
		enemy.move({ static_cast<float>(speed), 0 });
		isMoving = true;
		FacingRight = true;
	}

	if (playerPosition.x < enemy.getPosition().x) {
		enemy.move({ static_cast<float>(-speed), 0 });
		isMoving = true;
		FacingRight = false;
	}

	if (playerPosition.y > enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(speed) });
		isMoving = true;
	}

	if (playerPosition.y < enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(-speed) });
		isMoving = true;
	}


	// Keep the visible sprite synchronized with the logical collision shape.
	enemySprite.setPosition(enemy.getPosition());


	// Mirror the sprite horizontally instead of using separate left-facing textures.
	if (FacingRight) {
		enemySprite.setScale({ size, size });
	}

	else {
		enemySprite.setScale({ -size, size });
	}


	updateAnimation();
}


void Enemy::updateAnimation() {

	// Change texture only when the enemy switches
	// between moving and standing still.
	if (isMoving != wasMoving) {

		if (isMoving) {
			enemySprite.setTexture(enemyRunTexture, true);
		}

		else {
			enemySprite.setTexture(enemyIdleTexture, true);
		}


		// Restart the new animation from its first frame.
		currentFrame = 0;

		enemySprite.setTextureRect(
			sf::IntRect({ 0, 0 }, { 192, 192 })
		);

		animationClock.restart();

		// Remember the current state so the texture is not reset every frame.
		wasMoving = isMoving;
	}


	// Running and idle animations use different frame counts and speeds.
	int frameCount = isMoving ? 6 : 8;
	float frameTime = isMoving ? 0.10f : 0.12f;


	// Advance the sprite sheet only after enough time has passed.
	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {

		// Modulo wraps the animation back to frame 0
		// after the final frame.
		currentFrame =
			(currentFrame + 1) % frameCount;


		// Each frame occupies a 192x192 section of the texture.
		enemySprite.setTextureRect(
			sf::IntRect(
				{ currentFrame * 192, 0 },
				{ 192, 192 }
			)
		);


		animationClock.restart();
	}
}


int Enemy::getHealth() const {
	return health;
}


void Enemy::takeDamage(int damage) {

	// Negative damage would increase health,
	// so it is treated as an invalid value.
	if (damage < 0) {
		throw std::exception("Damage cannot be negative");
	}


	health -= damage;


	// Keep health from becoming negative.
	if (health < 0) {
		health = 0;
	}
}


void Enemy::draw(sf::RenderWindow& window) const {
	window.draw(enemySprite);
}


bool Enemy::isDead() const {

	if (health == 0) {
		return true;
	}

	else {
		return false;
	}
}


int Enemy::getDamage() const {
	return damage;
}


void Enemy::setPosition(sf::Vector2f position) {

	// Both representations must stay at the same position.
	// Game.cpp can restore this position when enemy movement causes a collision.
	enemy.setPosition(position);
	enemySprite.setPosition(position);
}


sf::Vector2f Enemy::getPosition() const {
	return enemy.getPosition();
}


std::vector<std::unique_ptr<Enemy>> Enemy::createEnemies(int level) {

	// The vector owns all Enemy objects created for this level.
	std::vector<std::unique_ptr<Enemy>>enemies;


	// Index mapping:
	// 0 = normal, 1 = quick, 2 = tank, 3 = boss.
	std::vector<int>numberOfEnemies(4, 0);


	// Determine the number of normal enemies.
	if (level >= 2) {
		numberOfEnemies[0] = 4;
	}

	else {
		numberOfEnemies[0] = 3;
	}


	// Quick enemies start appearing from level 4.
	if (level >= 4) {

		if (numberOfEnemies[1] < 3) {
			numberOfEnemies[1] = level - 3;
		}
	}


	// Tank enemies start appearing from level 7.
	if (level >= 7) {

		if (numberOfEnemies[2] < 3) {
			numberOfEnemies[2] = level - 6;
		}
	}


	// Every tenth level contains a boss instead of regular enemies.
	if (level % 10 == 0) {

		numberOfEnemies[0] = 0;
		numberOfEnemies[1] = 0;
		numberOfEnemies[2] = 0;
		numberOfEnemies[3] = 1;
	}


	// Create the required number of enemies for each type.
	for (int i = 0; i < numberOfEnemies.size(); i++) {

		for (int j = 0; j < numberOfEnemies[i]; j++) {

			sf::Vector2f position;


			switch (i) {

			case 0:

				position = {
					static_cast<float>(rand() % (2880 - 30) + 15),
					static_cast<float>(rand() % 480 + 30)
				};

				// make_unique creates the Enemy and transfers ownership
				// to the unique_ptr stored inside the vector.
				enemies.push_back(
					std::make_unique<Enemy>(
						EnemyType::NORMAL,
						position
					)
				);

				break;


			case 1:

				position = {
					static_cast<float>(rand() % 2880 - 20) + 10,
					static_cast<float>(rand() % 480 + 20)
				};

				enemies.push_back(
					std::make_unique<Enemy>(
						EnemyType::QUICK,
						position
					)
				);

				break;


			case 2:

				position = {
					static_cast<float>(rand() % 2880 - 50) + 25,
					static_cast<float>(rand() % 480 + 50)
				};

				enemies.push_back(
					std::make_unique<Enemy>(
						EnemyType::TANK,
						position
					)
				);

				break;


			case 3:

				// Boss uses a fixed spawn position.
				position = {
					static_cast<float>(1440),
					static_cast<float>(80)
				};

				enemies.push_back(
					std::make_unique<Enemy>(
						EnemyType::BOSS,
						position
					)
				);

				break;


			default:;
			}
		}
	}


	// Returning the vector transfers the collection of owned enemies
	// to the caller without manually managing their memory.
	return enemies;
}


sf::FloatRect Enemy::getGlobalBounds() const {

	// Collision calculations use the logical circle bounds,
	// not the animated sprite bounds.
	return enemy.getGlobalBounds();
}


EnemyType Enemy::getType() const {
	return type;
}


void Enemy::attack(Player& player) {

	// Damage is applied only while the enemy overlaps the player
	// and at least one second has passed since the previous attack.
	if (
		enemy.getGlobalBounds().findIntersection(player.getGlobalBounds())
		&& attackCooldown.getElapsedTime().asSeconds() >= 1
		) {

		// Defence reduces incoming enemy damage.
		if (player.getDefence() == 25) {

			player.takeDamage(
				getDamage() - getDamage() / 4
			);
		}

		else if (player.getDefence() == 50) {

			player.takeDamage(
				getDamage() / 2
			);
		}

		else {

			player.takeDamage(
				getDamage()
			);
		}


		// Start the one-second cooldown after a successful attack.
		attackCooldown.restart();
	}
}
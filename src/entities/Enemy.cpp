
#include  "Enemy.h"
#include "Player.h"

Enemy::Enemy(EnemyType type, sf::Vector2f position) : type(type), enemySprite(enemyIdleTexture){
	initialize(type);
	enemy.setPosition(position);
	enemySprite.setPosition(position);
}

void Enemy::initialize(EnemyType type) {
	std::string textureFolderPath;
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
		enemy.setFillColor(sf::Color(153,0,0));
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
	enemy.setOrigin({ enemy.getRadius(), enemy.getRadius()});
	if (!enemyIdleTexture.loadFromFile(textureFolderPath + "Warrior_Idle.png")) {
		throw std::exception("Failed to load enemy idle texture");
	}
	if (!enemyRunTexture.loadFromFile(textureFolderPath + "Warrior_Run.png")) {
		throw std::exception("Failed to load enemy run texture");
	}
	enemySprite.setTexture(enemyIdleTexture, true);
	enemySprite.setTextureRect({ {0, 0}, {192, 192} });
	enemySprite.setOrigin({ 96.f, 96.f });
	enemySprite.setScale({size, size});
}

void Enemy::movement(sf::Vector2f playerPosition) {
	isMoving = false;
	if (playerPosition.x > enemy.getPosition().x) {
		enemy.move({ static_cast<float>(speed), 0});
		isMoving = true;
		FacingRight = true;
	}
	if (playerPosition.x < enemy.getPosition().x) {
		enemy.move({ static_cast<float>(-speed), 0});
		isMoving = true;
		FacingRight = false;
	}
	if (playerPosition.y > enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(speed)});
		isMoving = true;
	}
	if (playerPosition.y < enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(-speed)});
		isMoving = true;
	}
	enemySprite.setPosition(enemy.getPosition());

	if (FacingRight) {
		enemySprite.setScale({ size, size });
	}
	else {
		enemySprite.setScale({ -size, size });
	}
	updateAnimation();
}

void Enemy::updateAnimation() {
	if (isMoving != wasMoving) {
		if (isMoving) {
			enemySprite.setTexture(enemyRunTexture, true);
		}
		else {
			enemySprite.setTexture(enemyIdleTexture, true);
		}

		currentFrame = 0;

		enemySprite.setTextureRect(
			sf::IntRect({ 0, 0 }, { 192, 192 })
		);

		animationClock.restart();
		wasMoving = isMoving;
	}

	int frameCount = isMoving ? 6 : 8;
	float frameTime = isMoving ? 0.10f : 0.12f;

	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
		currentFrame =
			(currentFrame + 1) % frameCount;

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
	if (damage < 0) {
		throw std::exception("Damage cannot be negative");
	}

	health -= damage;

	if (health < 0) {
		health = 0;
	}
}

void Enemy::draw(sf::RenderWindow& window) const {
	window.draw(enemySprite);
}

bool Enemy::isDead() const {
	if (health==0) {
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
	enemy.setPosition(position);
	enemySprite.setPosition(position);
}


sf::Vector2f Enemy::getPosition() const {
	return enemy.getPosition();
}

std::vector<std::unique_ptr<Enemy>> Enemy::createEnemies(int level) {
	std::vector<std::unique_ptr<Enemy>>enemies;
	std::vector<int>numberOfEnemies(4,0); // i=0 - normal | i=1 - quick | i=2 - tank | i=3 - boss
	if (level >= 2) {
		numberOfEnemies[0] = 4;
	}
	else {
		numberOfEnemies[0] = 3;
	}
	if (level>=4) {
		if (numberOfEnemies[1]<3) {
			numberOfEnemies[1] = level - 3;
		}
	}
	if (level>=7) {
		if (numberOfEnemies[2]<3) {
			numberOfEnemies[2] = level - 6;
		}
	}
	if (level%10==0) {
		numberOfEnemies[0] = 0;
		numberOfEnemies[1] = 0;
		numberOfEnemies[2] = 0;
		numberOfEnemies[3] = 1;
	}
	for (int i=0;i<numberOfEnemies.size();i++) {
		for (int j = 0; j < numberOfEnemies[i]; j++) {
			sf::Vector2f position;
			switch (i) {
			case 0:
				position = { static_cast<float>(rand() % (2880 - 30)+15),static_cast<float>(rand()%480+30)};
				enemies.push_back(std::make_unique<Enemy>(EnemyType::NORMAL,position));
				break;
			case 1:
				position = { static_cast<float>(rand() % 2880 - 20)+10,static_cast<float>(rand() % 480 + 20)};
				enemies.push_back(std::make_unique<Enemy>(EnemyType::QUICK,position));
				break;
			case 2:
				position = { static_cast<float>(rand() % 2880 - 50)+25,static_cast<float>(rand() % 480 + 50)};
				enemies.push_back(std::make_unique<Enemy>(EnemyType::TANK,position));
				break;
			case 3:
				position = {static_cast<float>(1440),static_cast<float>(80)};
				enemies.push_back(std::make_unique<Enemy>(EnemyType::BOSS,position));
				break;
			default: ;
			}
		}
	}
	return enemies;
}

sf::FloatRect Enemy::getGlobalBounds() const {
	return enemy.getGlobalBounds();
}

EnemyType Enemy::getType() const {
	return type;
}

void Enemy::attack(Player& player) {
	if (enemy.getGlobalBounds().findIntersection(player.getGlobalBounds()) && attackCooldown.getElapsedTime().asSeconds() >= 1) {
		if (player.getDefence()==25) {
			player.takeDamage(getDamage()-getDamage() / 4);
		}
		else if (player.getDefence() == 50) {
			player.takeDamage(getDamage() / 2);
		}
		else {
			player.takeDamage(getDamage());
		}
		attackCooldown.restart();
	}
}


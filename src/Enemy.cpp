
#include  "Enemy.h"

#include <iostream>

Enemy::Enemy(EnemyType type, sf::Vector2f position) : type(type){
	initialize(type);
	enemy.setPosition(position);
}

void Enemy::initialize(EnemyType type) {
	switch (type) {
	case EnemyType::NORMAL:
		enemy.setRadius(64);
		enemy.setFillColor(sf::Color::Yellow);
		damage = 15;
		health = 100;
		speed = 2;
		break;
	case EnemyType::QUICK:
		enemy.setRadius(48);
		enemy.setFillColor(sf::Color::Blue);
		damage = 10;
		health = 50;
		speed = 5;
		break;
	case EnemyType::TANK:
		enemy.setRadius(96);
		enemy.setFillColor(sf::Color(153,0,0));
		damage = 25;
		health = 250;
		speed = 1;
		break;
	case EnemyType::BOSS:
		enemy.setRadius(128);
		enemy.setFillColor(sf::Color(153, 0, 76));
		damage = 25;
		health = 500;
		speed = 1;
		break;
	default:
		throw std::exception("Invalid type");
	}
	enemy.setOrigin({ enemy.getRadius(), enemy.getRadius()});
}

void Enemy::movement(sf::Vector2f playerPosition) {
	if (playerPosition.x > enemy.getPosition().x) {
		enemy.move({ static_cast<float>(speed), 0});
	}
	if (playerPosition.x < enemy.getPosition().x) {
		enemy.move({ static_cast<float>(-speed), 0});
	}
	if (playerPosition.y > enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(speed)});
	}
	if (playerPosition.y < enemy.getPosition().y) {
		enemy.move({ 0, static_cast<float>(-speed)});
	}
}

int Enemy::getHealth() const {
	return health;
}

void Enemy::setHealth(int value, char op) {
	switch (op) {
	case '+':
		health += value;
		break;
	case '-':
		health -= value;
		break;
	case '*':
		health *= value;
		break;
	case '/':
		health /= value;
		break;
	default:
		throw std::exception("Invalid operator");
	}
	if (health > 100) {
		health = 100;
	}
	else if (health < 0) {
		health = 0;
	}
}

void Enemy::draw(sf::RenderWindow& window) const {
	window.draw(enemy);
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

std::vector<std::unique_ptr<Enemy>> Enemy::createEnemies(int level) {
	std::vector<std::unique_ptr<Enemy>>enemies;
	std::vector<int>numberOfEnemies(4,0); // i=0 - normal | i=1 - quick | i=2 - tank | i=3 - boss
	if (level == 2) {
		numberOfEnemies[0] = 4;
	}
	else {
		numberOfEnemies[0] = 3;
	}
	if (level>=5) {
		if (numberOfEnemies[1]<3) {
			numberOfEnemies[1] = level - 4;
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
	if (enemy.getGlobalBounds().findIntersection(player.getGlobalBounds()) && attackCooldown.getElapsedTime().asSeconds() > 1) {
		player.setHealth(getDamage(), '-');
		attackCooldown.restart();
	}
}

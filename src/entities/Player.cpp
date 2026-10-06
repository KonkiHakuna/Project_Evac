
#include "Player.h"
#include "Enemy.h"
#include "gameplay/Inventory.h"

Player::Player(sf::RenderWindow& window) : health(100), mana(100), defence(0) {
	player.setRadius(64);
	player.setFillColor(sf::Color::Green);
	player.setOrigin({player.getRadius(), player.getRadius()});
	player.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
	attackPlayerHitbox.setRadius(256);
	attackPlayerHitbox.setFillColor(sf::Color(255, 0, 0,100));
}

void Player::movement() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		player.move({0,-3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		player.move({0,3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		player.move({-3,0});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		player.move({3,0});
	}
}

void Player::movementBounds() {
	if (player.getPosition().x < 0) {
		player.setPosition({ 0,player.getPosition().y });
	}
	if (player.getPosition().x > 2880) {
		player.setPosition({ 2880,player.getPosition().y });
	}
	if (player.getPosition().y < 0) {
		player.setPosition({ player.getPosition().x,0 });
	}
	if (player.getPosition().y > 1920) {
		player.setPosition({ player.getPosition().x,1920 });
	}
}

void Player::dash() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {
		if (mana >= 20) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
				player.move({ 0,-256 });
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
				player.move({ 0,256 });
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
				player.move({ -256,0 });
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
				player.move({ 256,0 });
			}
			setMana(20);
		}
	}
}


void Player::draw(sf::RenderWindow& window) {
	window.draw(player);
	if (drawAttackPlayerHitbox) {
		window.draw(attackPlayerHitbox);
	}
	if (attackTimer.getElapsedTime().asMilliseconds() >= 350) {
		drawAttackPlayerHitbox = false;
	}
}

sf::Vector2f Player::getPosition() const{
	return player.getPosition();
}

void Player::resetPosition(sf::RenderWindow& window) {
	player.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });
}

int Player::getHealth() const {
	return health;
}

void Player::takeDamage(int damage) {
	if (damage < 0) {
		throw std::exception("Damage cannot be negative");
	}

	health -= damage;

	if (health < 0) {
		health = 0;
	}
}

void Player::heal(int amount) {
	if (amount < 0) {
		throw std::exception("Heal amount cannot be negative");
	}

	health += amount;

	if (health > 100) {
		health = 100;
	}
}

void Player::setHealth(int newHealth) {
	health = newHealth;
}

int Player::getMana() const {
	return mana;
}

void Player::useMana(int amount) {
	if (amount < 0) {
		throw std::exception("Mana amount cannot be negative");
	}

	mana -= amount;

	if (mana < 0) {
		mana = 0;
	}
}

void Player::restoreMana(int amount) {
	if (amount < 0) {
		throw std::exception("Mana amount cannot be negative");
	}

	mana += amount;

	if (mana > 100) {
		mana = 100;
	}
}

void Player::setMana(int newMana) {
	mana = newMana;
}

void Player::setDefence(int value) {
	defence = value;
}

int Player::getDefence() const {
	return defence;
}

void Player::attack(sf::Event::MouseButtonPressed const& e, Inventory& inventory, std::vector<std::unique_ptr<Enemy>>& enemies) {
	if (e.button == sf::Mouse::Button::Left) {
		if (attackTimer.getElapsedTime().asMilliseconds() >= 500) {
			if (dynamic_cast<Weapon*>(inventory.getCurrentWeapon())->isMelee()) {
				attackPlayerHitbox.setOrigin({attackPlayerHitbox.getRadius(),attackPlayerHitbox.getRadius()});
				attackPlayerHitbox.setPosition(player.getPosition());
				drawAttackPlayerHitbox = true;
				for (auto& enemy : enemies) {
					if (attackPlayerHitbox.getGlobalBounds().findIntersection(enemy->getGlobalBounds())) {
						enemy->takeDamage(dynamic_cast<Weapon*>(inventory.getCurrentWeapon())->getDamage());
					}
				}
				for (int i=0;i<enemies.size();) {
					if (enemies[i]->isDead()) {
						switch (enemies[i]->getType()) {
						case EnemyType::NORMAL:
							inventory.setPlayerGold(4, '+');
							break;
						case EnemyType::QUICK:
							inventory.setPlayerGold(2, '+');
							break;
						case EnemyType::TANK:
							inventory.setPlayerGold(8, '+');
							break;
						case EnemyType::BOSS:
							inventory.setPlayerGold(25, '+');
							break;
						default:;
						}
						setMana(5);
						enemies.erase(enemies.begin() + i);
					}
					else {
						i++;
					}
				}
			}
			attackTimer.restart();
		//}
		/*else if (e.button == sf::Mouse::Button::Right) {

		}*/
		}
	}
}
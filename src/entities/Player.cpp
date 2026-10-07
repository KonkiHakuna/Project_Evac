
#include "Player.h"
#include "Enemy.h"
#include "gameplay/Inventory.h"

Player::Player(sf::RenderWindow& window) : health(100), mana(100), defence(0), playerSprite(playerIdleTexture) {
	if (!playerIdleTexture.loadFromFile("assets/entities/player/Warrior_idle.png")) {
		throw std::exception("Failed to load player idle texture");
	}
	if (!playerRunTexture.loadFromFile("assets/entities/player/Warrior_run.png")) {
		throw std::exception("Failed to load player run texture");
	}
	if (!playerAttackTexture.loadFromFile("assets/entities/player/Warrior_Attack1.png")) {
		throw std::exception("Failed to load player attack texture");
	}
	playerSprite.setTexture(playerIdleTexture, true);
	playerSprite.setTextureRect({{0, 0}, {192, 192}});
	playerSprite.setOrigin({ 96.f, 96.f });
	playerSprite.setScale({ 2.5f, 2.5f });

	player.setRadius(64);
	player.setFillColor(sf::Color::Green);
	player.setOrigin({player.getRadius(), player.getRadius()});
	player.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
	attackPlayerHitbox.setRadius(256);
	attackPlayerHitbox.setFillColor(sf::Color(255, 0, 0,100));
}

void Player::movement() {
	isMoving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		player.move({0,-3});
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		player.move({0,3});
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		player.move({-3,0});
		isMoving = true;
		facingRight = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		player.move({3,0});
		isMoving = true;
		facingRight = true;
	}
	if (facingRight) {
		playerSprite.setScale({ 2.5f, 2.5f });
	}
	else {
		playerSprite.setScale({ -2.5f, 2.5f });
	}
	updateAnimation();
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
			useMana(20);
		}
	}
}

void Player::updateAnimation() {
	if (isAttacking) {
		int attackFrameCount = 4;
		float attackFrameTime = 0.08f;

		if (animationClock.getElapsedTime().asSeconds() >= attackFrameTime) {
			currentFrame++;

			if (currentFrame >= attackFrameCount) {
				isAttacking = false;
				currentFrame = 0;

				if (isMoving) {
					playerSprite.setTexture(playerRunTexture, true);
				}
				else {
					playerSprite.setTexture(playerIdleTexture, true);
				}

				playerSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 192, 192 }));
			}
			else {
				playerSprite.setTextureRect(sf::IntRect({ currentFrame * 192, 0 },{ 192, 192 }));
			}

			animationClock.restart();
		}

		return;
	}
	if (isMoving != wasMoving) {
		if (isMoving) {
			playerSprite.setTexture(playerRunTexture, true);
		}
		else {
			playerSprite.setTexture(playerIdleTexture, true);
		}

		currentFrame = 0;

		playerSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 192, 192 }));

		animationClock.restart();
		wasMoving = isMoving;
	}

	int frameCount = isMoving ? 6 : 8;
	float frameTime = isMoving ? 0.10f : 0.12f;

	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {
		currentFrame = (currentFrame + 1) % frameCount;

		playerSprite.setTextureRect(sf::IntRect({ currentFrame * 192, 0 }, { 192, 192 }));

		animationClock.restart();
	}
}

void Player::draw(sf::RenderWindow& window) {
	playerSprite.setPosition(player.getPosition());
	window.draw(playerSprite);
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
				isAttacking = true;
				currentFrame = 0;
				playerSprite.setTexture(playerAttackTexture, true);
				playerSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 192, 192 }));

				animationClock.restart();

				attackPlayerHitbox.setOrigin({
					attackPlayerHitbox.getRadius(),
					attackPlayerHitbox.getRadius()
					});

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
						restoreMana(5);
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
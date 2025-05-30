
#include "Player.h"

#include <iostream>

#include "Enemy.h"
#include "Item.h"

/*============================= Player =============================*/

Player::Player(sf::RenderWindow& window) : health(100), mana(100), defense(0) {
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

int Player::getHealth() {
	return health;
}

int Player::getMana() {
	return mana;
}

void Player::setHealth(int value, char op) {
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
		case '=':
			health = value;
			break;
		default:
			throw std::invalid_argument("Invalid operator");
	}
	if (health > 100 + defense) {
		health = 100 + defense;
	}
	else if (health < 0) {
		health = 0;
	}
}

void Player::setMana(int value, char op) {
	switch (op) {
	case '+':
		mana += value;
		break;
	case '-':
		mana -= value;
		break;
	case '*':
		mana *= value;
		break;
	case '/':
		mana /= value;
		break;
	case '=':
		mana = value;
		break;
	default:
		throw std::exception("Invalid operator");
	}
	if (mana > 100) {
		mana = 100;
	}
	else if (mana < 0) {
		mana = 0;
	}
}

void Player::setDefense(int value) {
	defense = value;
}

int Player::getDefence() {
	return defense;
}

void Player::attack(sf::Event::MouseButtonPressed const& e, Inventory& inventory, std::vector<std::unique_ptr<Enemy>>& enemies) {
	if (e.button == sf::Mouse::Button::Left) {
		if (attackTimer.getElapsedTime().asMilliseconds() >= 350) {
			if (dynamic_cast<Weapon*>(inventory.getCurrentWeapon())->isMelee()) {
				attackPlayerHitbox.setOrigin({attackPlayerHitbox.getRadius(),attackPlayerHitbox.getRadius()});
				attackPlayerHitbox.setPosition(player.getPosition());
				drawAttackPlayerHitbox = true;
				attackTimer.restart();
				for (auto& enemy : enemies) {
					if (attackPlayerHitbox.getGlobalBounds().findIntersection(enemy->getGlobalBounds())) {
						enemy->setHealth(dynamic_cast<Weapon*>(inventory.getCurrentWeapon())->getDamage(),'-');
					}
				}
				for (int i=0;i<enemies.size();) {
					if (enemies[i]->isDead()) {
						switch (enemies[i]->getType()) {
						case EnemyType::NORMAL:
							inventory.setPlayerGold(8, '+');
							break;
						case EnemyType::QUICK:
							inventory.setPlayerGold(6, '+');
							break;
						case EnemyType::TANK:
							inventory.setPlayerGold(16, '+');
							break;
						case EnemyType::BOSS:
							inventory.setPlayerGold(32, '+');
							break;
						default:;
						}
						enemies.erase(enemies.begin() + i);
					}
					else {
						i++;
					}
				}
			}
		}
		/*else if (e.button == sf::Mouse::Button::Right) {

		}*/
	}
}

/*============================= Inventory =============================*/

Inventory::Inventory() : gold(50), currentArmor(nullptr), currentSpell(nullptr) {
	addItem(std::make_unique<Weapon>("Stick", "assets/items/weapons/stick.png", 20, true));
	currentWeapon = items[0].get();
}

void Inventory::addItem(std::unique_ptr<Item> item) {
	items.push_back(std::move(item));
}

void Inventory::removeItem(Item* item) {
	if (item == currentWeapon) {
		currentWeapon = nullptr;
	}
	if (item == currentArmor) {
		currentArmor = nullptr;
	}
	if (item == currentSpell) {
		currentSpell = nullptr;
	}
	for (auto it = items.begin(); it != items.end(); ++it) {
		if (it->get() == item) {
			items.erase(it);
			break;
		}
	}
	if (!currentWeapon) {
		for (auto& it : items) {
			if (dynamic_cast<Weapon*>(it.get())) {
				currentWeapon = it.get();
				break;
			}
		}
		if (!currentWeapon) {
			addItem(std::make_unique<Weapon>("Stick", "assets/items/weapons/stick.png", 20, true));
			currentWeapon = items[0].get();
		}
	}
}

Item* Inventory::getCurrentWeapon() {
	return currentWeapon;
}

void Inventory::setCurrentWeapon(Item* weapon) {
	currentWeapon = weapon;
}

Item* Inventory::getCurrentArmor() {
	return currentArmor;
}

void Inventory::setCurrentArmor(Item* armor) {
	currentArmor = armor;
}

Item* Inventory::getCurrentSpell() {
	return currentSpell;
}

void Inventory::setCurrentSpell(Item* spell) {
	currentSpell = spell;
}

int Inventory::getPlayerGold() {
	return gold;
}

void Inventory::setPlayerGold(int value, char op) {
	switch (op) {
	case '+':
		gold += value;
		break;
	case '-':
		gold -= value;
		break;
	case '=':
		gold = value;
		break;
	default:
		throw std::exception("Invalid operator");
	}
}


std::vector<Item*> Inventory::getItems() {
	std::vector<Item*> tempItems;
	for (auto& item : items) {
		tempItems.push_back(item.get());
	}
	return tempItems;
}

sf::FloatRect Player::getGlobalBounds() const {
	return player.getGlobalBounds();
}

void Inventory::clearItems() {
	items.clear();
	currentWeapon = nullptr;
	currentArmor = nullptr;
	currentSpell = nullptr;
}
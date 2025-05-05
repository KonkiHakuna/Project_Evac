
#include "Player.h"

/*============================= Player =============================*/

Player::Player(sf::RenderWindow& window) : health(100), mana(100) {
	shape.setRadius(20);
	shape.setFillColor(sf::Color::Green);
	shape.setOrigin({shape.getRadius(), shape.getRadius()});
	shape.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
}

void Player::movement() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
		shape.move({0,-3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
		shape.move({0,3});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {
		shape.move({-3,0});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {
		shape.move({3,0});
	}
}

void Player::draw(sf::RenderWindow& window) const{
	window.draw(shape);
}

sf::Vector2f Player::getPosition() const{
	return shape.getPosition();
}

void Player::resetPosition(sf::RenderWindow& window) {
	shape.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });
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
		default:
			throw std::invalid_argument("Invalid operator");
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
	default:
		throw std::exception("Invalid operator");
	}
}

/*============================= Inventory =============================*/

Inventory::Inventory() : gold(100) {
	items.push_back(std::make_unique<Weapon>("Stick","assets/items/weapons/stick.png",5,32));
	currentWeapon = items[0].get();
}

void Inventory::addItem(std::unique_ptr<Item> item) {
	items.push_back(std::move(item));
}

Item* Inventory::getCurrentWeapon() {
	return currentWeapon;
}

void Inventory::setCurrentWeapon(Item* weapon) {
	currentWeapon = weapon;
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



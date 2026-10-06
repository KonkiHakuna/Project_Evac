
#include "Shop.h"

Shop::Shop(sf::RenderWindow& window, sf::Font& font) : shopTitle(font) {
	shopOverlay.setSize(sf::Vector2f{ 1024,1024 });
	shopOverlay.setFillColor(sf::Color::Red);
	shopOverlay.setOrigin({ shopOverlay.getLocalBounds().size.x / 2,shopOverlay.getLocalBounds().size.y / 2 });
	shopOverlay.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });

	shopCloseButton.setSize(sf::Vector2f{ 64,64 });
	shopCloseButton.setFillColor(sf::Color::Black);
	shopCloseButton.setOrigin({ shopCloseButton.getLocalBounds().size.x / 2 + 32,shopCloseButton.getLocalBounds().size.y / 2 });
	shopCloseButton.setPosition({ static_cast<float>(shopOverlay.getSize().x / 2 + shopOverlay.getPosition().x),static_cast<float>(shopOverlay.getPosition().y / 2) });

	shopItemFrame_1.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_1.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_1.setOrigin({ shopItemFrame_1.getLocalBounds().size.x / 2,shopItemFrame_1.getLocalBounds().size.y / 2 });
	shopItemFrame_1.setPosition({ static_cast<float>(shopOverlay.getPosition().x - shopOverlay.getSize().x / 2 + shopItemFrame_1.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_1.getSize().y) - 96 });

	shopItemFrame_2.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_2.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_2.setOrigin({ shopItemFrame_2.getLocalBounds().size.x / 2,shopItemFrame_2.getLocalBounds().size.y / 2 });
	shopItemFrame_2.setPosition({ static_cast<float>(shopOverlay.getPosition().x + shopOverlay.getSize().x / 2 - shopItemFrame_2.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_2.getSize().y) - 96 });

	shopTitle.setCharacterSize(256);
	shopTitle.setFillColor(sf::Color::White);

	initializeShopItems();
}

void Shop::initializeShopItems() {
	shopItems[LobbyLocation::weaponShop].emplace_back(std::make_unique<Weapon>("Sword", "assets/items/weapons/sword.png", 100, true), 50);
	shopItems[LobbyLocation::weaponShop].emplace_back(std::make_unique<Weapon>("Bow", "assets/items/weapons/bow.png", 25, false), 50);
	shopItems[LobbyLocation::armory].emplace_back(std::make_unique<Armor>("Helmet", "assets/items/armor/helmet.png", 25), 50);
	shopItems[LobbyLocation::armory].emplace_back(std::make_unique<Armor>("Chestplate", "assets/items/armor/chestplate.png", 50), 100);
	shopItems[LobbyLocation::doctor].emplace_back(std::make_unique<Potion>("Health Potion", "assets/items/potions/health_potion.png", 50, TypeOfPotion::Healing), 50);
	shopItems[LobbyLocation::doctor].emplace_back(std::make_unique<Potion>("Mana Potion", "assets/items/potions/mana_potion.png", 50, TypeOfPotion::Mana), 50);
	shopItems[LobbyLocation::wizard].emplace_back(std::make_unique<Spell>("Fireball", "assets/items/spells/fireball.png", 15, 30), 50);
	//shopItems[LobbyLocation::wizard].emplace_back(std::make_unique<Spell>("Lightning Bolt", "assets/items/spells/lightning_bolt.png", 25, 40), 50);
}

void Shop::interact(sf::RenderWindow& window, sf::Font& font) const {
	if (!isOpen) {
		sf::Text text{ font,"Press E to interact",128 };
		text.setFillColor(sf::Color::White);
		text.setOrigin({ text.getLocalBounds().size.x / 2,text.getLocalBounds().size.y / 2 });
		text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) - 256 });
		window.draw(text);
	}
}

void Shop::draw(sf::RenderWindow& window, std::string name) {
	shopTitle.setString(name);
	shopTitle.setOrigin({ shopTitle.getLocalBounds().size.x / 2,shopTitle.getLocalBounds().size.y / 2 });
	shopTitle.setPosition({ static_cast<float>(shopOverlay.getPosition().x),static_cast<float>(shopOverlay.getPosition().y - shopOverlay.getSize().y / 2 + shopTitle.getLocalBounds().size.y) });
	window.draw(shopOverlay);
	window.draw(shopCloseButton);
	window.draw(shopItemFrame_1);
	window.draw(shopItemFrame_2);
	window.draw(shopTitle);
	int i = 0;
	for (auto& item : shopItems[currentShopLocation]) {
		if (i == 0) {
			item.first->setPosition({ static_cast<float>(shopItemFrame_1.getPosition().x),static_cast<float>(shopItemFrame_1.getPosition().y) });
			item.first->draw(window);
		}
		else if (i == 1) {
			item.first->setPosition({ static_cast<float>(shopItemFrame_2.getPosition().x),static_cast<float>(shopItemFrame_2.getPosition().y) });
			item.first->draw(window);
		}
		i++;
	}
}

void Shop::mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory) {
	if (e.button == sf::Mouse::Button::Left) {
		if (shopCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			isOpen = false;
			shopCloseButton.setFillColor(sf::Color::Black);
		}
		else if (shopItemFrame_1.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			buyItem(0, inventory);
		}
		else if (shopItemFrame_2.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			buyItem(1, inventory);
		}
	}
}

void Shop::mouseMove(sf::Event::MouseMoved const& e) {
	if (shopCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
		shopCloseButton.setFillColor(sf::Color::Blue);
	}
	else {
		shopCloseButton.setFillColor(sf::Color::Black);
	}
}

bool Shop::getShoppingStatus() const {
	return isOpen;
}
void Shop::setShoppingStatus(bool status) {
	isOpen = status;
}

void Shop::setCurrentShopLocation(LobbyLocation location) {
	currentShopLocation = location;
}

void Shop::buyItem(int itemFrame, Inventory& inventory) {
	if (!shopItems[currentShopLocation].empty()) {
		if (inventory.getPlayerGold() >= shopItems[currentShopLocation][itemFrame].second) {
			inventory.setPlayerGold(shopItems[currentShopLocation][itemFrame].second, '-');
			inventory.addItem(std::move(shopItems[currentShopLocation][itemFrame].first));
			resetShopItems();
		}
	}
}

void Shop::resetShopItems() {
	shopItems.clear();
	initializeShopItems();
}
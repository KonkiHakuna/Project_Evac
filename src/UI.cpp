
#include "UI.h"

/*============================== Start Screen =============================*/

StartScreen::StartScreen(sf::RenderWindow& window, sf::Font& font) : title{ font,"Evac",256 }, pressSpace{ font,"Press space to continue",128 } {
	title.setFillColor(sf::Color::White);
	title.setOrigin({ title.getLocalBounds().size.x / 2,title.getLocalBounds().size.y / 2 });
	title.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
	
	pressSpace.setFillColor(sf::Color::White);
	pressSpace.setOrigin({ pressSpace.getLocalBounds().size.x / 2,pressSpace.getLocalBounds().size.y / 2 });
	pressSpace.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) + 256 });
}

void StartScreen::draw(sf::RenderWindow& window) {
	window.draw(title);;
	window.draw(pressSpace);
}

/*============================= Pause Menu =============================*/

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font) : options{"Resume","Quit"}{
	float y = 0;
	for (const auto& option : options) {
		sf::Text text(font,option,128);
		text.setFillColor(sf::Color::White);
		text.setOrigin({text.getLocalBounds().size.x / 2,text.getLocalBounds().size.y / 2 });
		text.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)+y});
		menuOptions.push_back(text);
		y += 150;
	}
}

void PauseMenu::draw(sf::RenderWindow& window) const {
	for (const auto& option : menuOptions) {
		window.draw(option);
	}
}

void PauseMenu::mouseClick(sf::Event::MouseButtonPressed const& e) {  
   if (e.button == sf::Mouse::Button::Left) {  
       for (int i = 0; i < menuOptions.size(); ++i) {  
           if (menuOptions[i].getGlobalBounds().contains({static_cast<float>(e.position.x), static_cast<float>(e.position.y)})) {  
               decision = i;  
               break;  
           }  
       }  
   }  
}

void PauseMenu::mouseMove(sf::Event::MouseMoved const& e) {
	for (auto& option : menuOptions) {
		if (option.getGlobalBounds().contains({static_cast<float>(e.position.x),static_cast<float>(e.position.y)})) {
			option.setFillColor(sf::Color::Red);
		}
		else {
			option.setFillColor(sf::Color::White);
		}
	}
}

/*============================= Shop =============================*/

Shop::Shop(sf::RenderWindow& window, sf::Font& font) : shopTitle(font) {
	shopOverlay.setSize(sf::Vector2f{1024,1024});
	shopOverlay.setFillColor(sf::Color::Red);
	shopOverlay.setOrigin({ shopOverlay.getLocalBounds().size.x / 2,shopOverlay.getLocalBounds().size.y / 2});
	shopOverlay.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});

	shopCloseButton.setSize(sf::Vector2f{ 64,64 });
	shopCloseButton.setFillColor(sf::Color::Black);
	shopCloseButton.setOrigin({ shopCloseButton.getLocalBounds().size.x / 2 + 32,shopCloseButton.getLocalBounds().size.y / 2});
	shopCloseButton.setPosition({ static_cast<float>(shopOverlay.getSize().x / 2 + shopOverlay.getPosition().x),static_cast<float>(shopOverlay.getPosition().y / 2)});

	shopItemFrame_1.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_1.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_1.setOrigin({ shopItemFrame_1.getLocalBounds().size.x / 2,shopItemFrame_1.getLocalBounds().size.y / 2});
	shopItemFrame_1.setPosition({ static_cast<float>(shopOverlay.getPosition().x-shopOverlay.getSize().x / 2 + shopItemFrame_1.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_1.getSize().y) - 96});

	shopItemFrame_2.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_2.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_2.setOrigin({ shopItemFrame_2.getLocalBounds().size.x / 2,shopItemFrame_2.getLocalBounds().size.y / 2});
	shopItemFrame_2.setPosition({ static_cast<float>(shopOverlay.getPosition().x + shopOverlay.getSize().x / 2 - shopItemFrame_2.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_2.getSize().y) - 96});

	shopTitle.setCharacterSize(256);
	shopTitle.setFillColor(sf::Color::White);

	initializeShopItems();
}

void Shop::initializeShopItems() {
	shopItems[LobbyLocation::weaponShop].emplace_back(std::make_unique<Weapon>("Sword", "assets/items/weapons/sword.png", 10, 32), 50);
	shopItems[LobbyLocation::weaponShop].emplace_back(std::make_unique<Weapon>("Bow", "assets/items/weapons/bow.png", 7, 128), 50);
	shopItems[LobbyLocation::armory].emplace_back(std::make_unique<Armor>("Helmet", "assets/items/armor/helmet.png", 15), 50);
	shopItems[LobbyLocation::armory].emplace_back(std::make_unique<Armor>("Chestplate", "assets/items/armor/chestplate.png", 25), 50);
	shopItems[LobbyLocation::doctor].emplace_back(std::make_unique<Potion>("Health Potion", "assets/items/potions/health_potion.png", 50), 50);
	shopItems[LobbyLocation::doctor].emplace_back(std::make_unique<Potion>("Mana Potion", "assets/items/potions/mana_potion.png", 50), 50);
	shopItems[LobbyLocation::wizard].emplace_back(std::make_unique<Spell>("Fireball", "assets/items/spells/fireball.png", 15, 30), 50);
	shopItems[LobbyLocation::wizard].emplace_back(std::make_unique<Spell>("Lightning Bolt", "assets/items/spells/lightning_bolt.png", 25, 40), 50);
}

void Shop::interact(sf::RenderWindow& window, sf::Font& font) {
	if (!isOpen) {
		sf::Text text{ font,"Press E to interact",128 };
		text.setFillColor(sf::Color::White);
		text.setOrigin({ text.getLocalBounds().size.x / 2,text.getLocalBounds().size.y / 2 });
		text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y)-256});
		window.draw(text);
	}
}

void Shop::draw(sf::RenderWindow& window,std::string name) {
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

bool Shop::getShoppingStatus() {
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
		if (inventory.getPlayerGold()>=shopItems[currentShopLocation][itemFrame].second) {
			inventory.setPlayerGold(shopItems[currentShopLocation][itemFrame].second, '-');
			inventory.addItem(std::move(shopItems[currentShopLocation][itemFrame].first));
			shopItems[currentShopLocation].erase(shopItems[currentShopLocation].begin() + itemFrame);
		}
	}
}

/*============================= Mine Entrance =============================*/

void MineEntrance::interact(sf::RenderWindow& window, sf::Font& font) {
	sf::Text text{font,"Press E to enter",128};
	text.setFillColor(sf::Color::White);
	text.setOrigin({ text.getLocalBounds().size.x / 2,text.getLocalBounds().size.y / 2 });
	text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) - 256 });
	window.draw(text);
}

/*============================= Inventory =============================*/

InventoryUI::InventoryUI(sf::RenderWindow& window, sf::Font& font) : inventoryTitle(font,"Inventory",256) {
	inventoryOverlay.setSize(sf::Vector2f{ 1536,1024 });
	inventoryOverlay.setFillColor(sf::Color::Red);
	inventoryOverlay.setOrigin({inventoryOverlay.getLocalBounds().size.x / 2,inventoryOverlay.getLocalBounds().size.y / 2});
	inventoryOverlay.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });

	inventoryCloseButton.setSize(sf::Vector2f{ 64,64 });
	inventoryCloseButton.setFillColor(sf::Color::Black);
	inventoryCloseButton.setOrigin({inventoryCloseButton.getLocalBounds().size.x / 2 + 32,inventoryCloseButton.getLocalBounds().size.y / 2});
	inventoryCloseButton.setPosition({ static_cast<float>(inventoryOverlay.getSize().x / 2 + inventoryOverlay.getPosition().x),static_cast<float>(inventoryOverlay.getPosition().y / 2) });

	inventoryTitle.setFillColor(sf::Color::White);
	inventoryTitle.setOrigin({inventoryTitle.getLocalBounds().size.x / 2,inventoryTitle.getLocalBounds().size.y / 2});
	inventoryTitle.setPosition({ static_cast<float>(inventoryOverlay.getPosition().x),static_cast<float>(inventoryOverlay.getPosition().y - inventoryOverlay.getSize().y / 2 + inventoryTitle.getLocalBounds().size.y) - 128 });

	itemSlot.setSize(sf::Vector2f{256, 256});
	itemSlot.setFillColor(sf::Color(74, 74, 74));
	itemSlot.setOrigin({ itemSlot.getLocalBounds().size.x / 2, itemSlot.getLocalBounds().size.y / 2 });
	float xPos = inventoryOverlay.getPosition().x - inventoryOverlay.getSize().x / 2 + 192;
	float yPos = inventoryOverlay.getPosition().y - 64;
	itemSlot.setPosition({ xPos,yPos });
	itemSlots.push_back(itemSlot);
	for (int i=0;i<7;i++) {
		if (i%2==0) {
			itemSlot.setPosition({ xPos,yPos + 384});
			itemSlots.push_back(itemSlot);
		}
		else {
			itemSlot.setPosition({ xPos += 384,yPos });
			itemSlots.push_back(itemSlot);
		}
	}
}

void InventoryUI::draw(sf::RenderWindow& window, Inventory& inventory) {
	window.draw(inventoryOverlay);
	window.draw(inventoryCloseButton);
	window.draw(inventoryTitle);
	for (auto itemSlot : itemSlots) {
		window.draw(itemSlot);
	}
	for (int i = 0; i < inventory.getItems().size(); i++) {
		auto item = inventory.getItems()[i];
		item->setPosition({ static_cast<float>(itemSlots[i].getPosition().x),static_cast<float>(itemSlots[i].getPosition().y)});
		item->draw(window);
	}
}

void InventoryUI::mouseClick(sf::Event::MouseButtonPressed const& e) {
	if (e.button == sf::Mouse::Button::Left) {
		if (inventoryCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			isOpen = false;
			inventoryCloseButton.setFillColor(sf::Color::Black);
		}
	}
}

void InventoryUI::mouseMove(sf::Event::MouseMoved const& e) {
	if (inventoryCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
		inventoryCloseButton.setFillColor(sf::Color::Blue);
	}
	else {
		inventoryCloseButton.setFillColor(sf::Color::Black);
	}
}

bool InventoryUI::getInventoryStatus() {
	return isOpen;
}

void InventoryUI::setInventoryStatus(bool status) {
	isOpen = status;
}

/*============================= HUD =============================*/

HUD::HUD(sf::RenderWindow& window, sf::Font& font) : healthText(font), manaText(font), gold(font) {
	healthText.setCharacterSize(128);
	healthText.setFillColor(sf::Color::Red);
	healthText.setOrigin({healthText.getLocalBounds().size.x / 2,healthText.getLocalBounds().size.y / 2});
	healthText.setPosition({0,0});

	manaText.setCharacterSize(128);
	manaText.setFillColor(sf::Color::Blue);
	manaText.setOrigin({manaText.getLocalBounds().size.x / 2,manaText.getLocalBounds().size.y / 2});
	manaText.setPosition({0,128});

	gold.setCharacterSize(128);
	gold.setFillColor(sf::Color::Yellow);
	gold.setOrigin({gold.getLocalBounds().size.x / 2,gold.getLocalBounds().size.y / 2});
	gold.setPosition({ 0,256 });

	currentWeaponSlot.setSize(sf::Vector2f{ 256,256 });
	currentWeaponSlot.setFillColor(sf::Color(74, 74, 74));
	currentWeaponSlot.setOrigin({currentWeaponSlot.getLocalBounds().size.x / 2,currentWeaponSlot.getLocalBounds().size.y / 2});
	currentWeaponSlot.setPosition({static_cast<float>(window.getSize().x) - 192,static_cast<float>(window.getSize().y) - 192});
}

void HUD::draw(sf::RenderWindow& window, Player& player, Inventory& inventory) {
	healthText.setString("Health: " + std::to_string(player.getHealth()));
	manaText.setString("Mana: " + std::to_string(player.getMana()));
	gold.setString("Gold: " + std::to_string(inventory.getPlayerGold()));
	auto currentItem = inventory.getCurrentWeapon();
	currentItem->setPosition({ static_cast<float>(currentWeaponSlot.getPosition().x),static_cast<float>(currentWeaponSlot.getPosition().y) });
	window.draw(healthText);
	window.draw(manaText);
	window.draw(gold);
	window.draw(currentWeaponSlot);
	currentItem->draw(window);
}


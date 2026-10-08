#include "Shop.h"


Shop::Shop(sf::RenderWindow& window, sf::Font& font) : shopTitle(font) {

	// Main shop background.
	shopOverlay.setSize(sf::Vector2f{ 1024,1024 });
	shopOverlay.setFillColor(sf::Color::Red);

	shopOverlay.setOrigin({
		shopOverlay.getLocalBounds().size.x / 2,
		shopOverlay.getLocalBounds().size.y / 2
		});

	shopOverlay.setPosition({
		static_cast<float>(window.getSize().x / 2),
		static_cast<float>(window.getSize().y / 2)
		});


	// Close button positioned relative to the shop overlay.
	shopCloseButton.setSize(sf::Vector2f{ 64,64 });
	shopCloseButton.setFillColor(sf::Color::Black);

	shopCloseButton.setOrigin({
		shopCloseButton.getLocalBounds().size.x / 2 + 32,
		shopCloseButton.getLocalBounds().size.y / 2
		});

	shopCloseButton.setPosition({
		static_cast<float>(
			shopOverlay.getSize().x / 2
			+ shopOverlay.getPosition().x
		),
		static_cast<float>(
			shopOverlay.getPosition().y / 2
		)
		});


	// First shop item slot.
	shopItemFrame_1.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_1.setFillColor(sf::Color(74, 74, 74));

	shopItemFrame_1.setOrigin({
		shopItemFrame_1.getLocalBounds().size.x / 2,
		shopItemFrame_1.getLocalBounds().size.y / 2
		});

	shopItemFrame_1.setPosition({
		static_cast<float>(
			shopOverlay.getPosition().x
			- shopOverlay.getSize().x / 2
			+ shopItemFrame_1.getSize().x
		),
		static_cast<float>(
			(shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2
			+ shopItemFrame_1.getSize().y
		) - 96
		});


	// Second shop item slot.
	shopItemFrame_2.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_2.setFillColor(sf::Color(74, 74, 74));

	shopItemFrame_2.setOrigin({
		shopItemFrame_2.getLocalBounds().size.x / 2,
		shopItemFrame_2.getLocalBounds().size.y / 2
		});

	shopItemFrame_2.setPosition({
		static_cast<float>(
			shopOverlay.getPosition().x
			+ shopOverlay.getSize().x / 2
			- shopItemFrame_2.getSize().x
		),
		static_cast<float>(
			(shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2
			+ shopItemFrame_2.getSize().y
		) - 96
		});


	shopTitle.setCharacterSize(256);
	shopTitle.setFillColor(sf::Color::White);


	// Fill every shop category with its initial items.
	initializeShopItems();
}


void Shop::initializeShopItems() {

	// Each pair contains:
	// first  = owned Item
	// second = item price

	shopItems[LobbyLocation::weaponShop].emplace_back(
		std::make_unique<Weapon>(
			"Sword",
			"assets/items/weapons/sword.png",
			100,
			true
		),
		50
	);

	shopItems[LobbyLocation::weaponShop].emplace_back(
		std::make_unique<Weapon>(
			"Bow",
			"assets/items/weapons/bow.png",
			50,
			false
		),
		50
	);


	shopItems[LobbyLocation::armory].emplace_back(
		std::make_unique<Armor>(
			"Helmet",
			"assets/items/armor/helmet.png",
			25
		),
		50
	);

	shopItems[LobbyLocation::armory].emplace_back(
		std::make_unique<Armor>(
			"Chestplate",
			"assets/items/armor/chestplate.png",
			50
		),
		100
	);


	shopItems[LobbyLocation::doctor].emplace_back(
		std::make_unique<Potion>(
			"Health Potion",
			"assets/items/potions/health_potion.png",
			50,
			TypeOfPotion::Healing
		),
		50
	);

	shopItems[LobbyLocation::doctor].emplace_back(
		std::make_unique<Potion>(
			"Mana Potion",
			"assets/items/potions/mana_potion.png",
			50,
			TypeOfPotion::Mana
		),
		50
	);


	shopItems[LobbyLocation::wizard].emplace_back(
		std::make_unique<Spell>(
			"Fireball",
			"assets/items/spells/fireball.png",
			150,
			30
		),
		50
	);

	//shopItems[LobbyLocation::wizard].emplace_back(std::make_unique<Spell>("Lightning Bolt", "assets/items/spells/lightning_bolt.png", 25, 40), 50);
}


void Shop::interact(sf::RenderWindow& window, sf::Font& font) const {

	// Only display the prompt when the full shop window is closed.
	if (!isOpen) {

		sf::Text text{
			font,
			"Press E to interact",
			128
		};

		text.setFillColor(sf::Color::White);

		text.setOrigin({
			text.getLocalBounds().size.x / 2,
			text.getLocalBounds().size.y / 2
			});

		text.setPosition({
			static_cast<float>(window.getSize().x / 2),
			static_cast<float>(window.getSize().y) - 256
			});

		window.draw(text);
	}
}


void Shop::draw(sf::RenderWindow& window, std::string name) {

	// The title is supplied by Lobby depending on the current location.
	shopTitle.setString(name);

	shopTitle.setOrigin({
		shopTitle.getLocalBounds().size.x / 2,
		shopTitle.getLocalBounds().size.y / 2
		});

	shopTitle.setPosition({
		static_cast<float>(shopOverlay.getPosition().x),
		static_cast<float>(
			shopOverlay.getPosition().y
			- shopOverlay.getSize().y / 2
			+ shopTitle.getLocalBounds().size.y
		)
		});


	// Draw the basic shop UI.
	window.draw(shopOverlay);
	window.draw(shopCloseButton);
	window.draw(shopItemFrame_1);
	window.draw(shopItemFrame_2);
	window.draw(shopTitle);


	int i = 0;


	// Display items belonging only to the currently selected shop.
	for (auto& item : shopItems[currentShopLocation]) {

		if (i == 0) {

			item.first->setPosition({
				static_cast<float>(shopItemFrame_1.getPosition().x),
				static_cast<float>(shopItemFrame_1.getPosition().y)
				});

			item.first->draw(window);
		}

		else if (i == 1) {

			item.first->setPosition({
				static_cast<float>(shopItemFrame_2.getPosition().x),
				static_cast<float>(shopItemFrame_2.getPosition().y)
				});

			item.first->draw(window);
		}

		i++;
	}
}


void Shop::mouseClick(
	sf::Event::MouseButtonPressed const& e,
	Inventory& inventory
) {

	if (e.button == sf::Mouse::Button::Left) {

		// Close the shop when the close button is clicked.
		if (
			shopCloseButton.getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			isOpen = false;

			shopCloseButton.setFillColor(
				sf::Color::Black
			);
		}


		// Item frame numbers are converted to vector indexes:
		// first frame = index 0, second frame = index 1.
		else if (
			shopItemFrame_1.getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			buyItem(0, inventory);
		}


		else if (
			shopItemFrame_2.getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			buyItem(1, inventory);
		}
	}
}


void Shop::mouseMove(sf::Event::MouseMoved const& e) {

	// Highlight the close button while the mouse is over it.
	if (
		shopCloseButton.getGlobalBounds().contains({
			static_cast<float>(e.position.x),
			static_cast<float>(e.position.y)
			})
		) {

		shopCloseButton.setFillColor(
			sf::Color::Blue
		);
	}

	else {

		shopCloseButton.setFillColor(
			sf::Color::Black
		);
	}
}


bool Shop::getShoppingStatus() const {
	return isOpen;
}


void Shop::setShoppingStatus(bool status) {
	isOpen = status;
}


void Shop::setCurrentShopLocation(LobbyLocation location) {

	// This value is later used as the key in shopItems.
	currentShopLocation = location;
}


void Shop::buyItem(int itemFrame, Inventory& inventory) {

	// Do not attempt a purchase when the selected shop has no items.
	if (!shopItems[currentShopLocation].empty()) {

		// The item can only be bought when the player has enough gold.
		if (
			inventory.getPlayerGold()
			>= shopItems[currentShopLocation][itemFrame].second
			) {

			// Subtract the item's price.
			inventory.setPlayerGold(
				shopItems[currentShopLocation][itemFrame].second,
				'-'
			);


			// Ownership of the Item moves from the shop
			// into the player's Inventory.
			inventory.addItem(
				std::move(
					shopItems[currentShopLocation][itemFrame].first
				)
			);


			// Recreate the shop stock after the purchase.
			resetShopItems();
		}
	}
}


void Shop::resetShopItems() {

	// Clearing the map destroys all remaining unique_ptr-owned items.
	shopItems.clear();


	// Rebuild the default stock for every shop.
	initializeShopItems();
}
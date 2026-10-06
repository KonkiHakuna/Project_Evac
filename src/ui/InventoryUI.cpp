
#include "InventoryUI.h"
#include "InventoryItemOptions.h"
#include "gameplay/Inventory.h"

InventoryUI::InventoryUI(sf::RenderWindow& window, sf::Font& font, InventoryItemOptions& options) : inventoryTitle(font, "Inventory", 256), itemOptions(options) {
	inventoryOverlay.setSize(sf::Vector2f{ 1536,1024 });
	inventoryOverlay.setFillColor(sf::Color::Red);
	inventoryOverlay.setOrigin({ inventoryOverlay.getLocalBounds().size.x / 2,inventoryOverlay.getLocalBounds().size.y / 2 });
	inventoryOverlay.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) });

	inventoryCloseButton.setSize(sf::Vector2f{ 64,64 });
	inventoryCloseButton.setFillColor(sf::Color::Black);
	inventoryCloseButton.setOrigin({ inventoryCloseButton.getLocalBounds().size.x / 2,inventoryCloseButton.getLocalBounds().size.y / 2 });
	inventoryCloseButton.setPosition({ static_cast<float>(inventoryOverlay.getSize().x / 2 + inventoryOverlay.getPosition().x) - 32,static_cast<float>(inventoryOverlay.getPosition().y / 2) });

	inventoryTitle.setFillColor(sf::Color::White);
	inventoryTitle.setOrigin({ inventoryTitle.getLocalBounds().size.x / 2,inventoryTitle.getLocalBounds().size.y / 2 });
	inventoryTitle.setPosition({ static_cast<float>(inventoryOverlay.getPosition().x),static_cast<float>(inventoryOverlay.getPosition().y - inventoryOverlay.getSize().y / 2 + inventoryTitle.getLocalBounds().size.y) - 128 });

	itemSlot.setSize(sf::Vector2f{ 256, 256 });
	itemSlot.setFillColor(sf::Color(74, 74, 74));
	itemSlot.setOrigin({ itemSlot.getLocalBounds().size.x / 2, itemSlot.getLocalBounds().size.y / 2 });
	float xPos = inventoryOverlay.getPosition().x - inventoryOverlay.getSize().x / 2 + 192;
	float yPos = inventoryOverlay.getPosition().y - 64;
	itemSlot.setPosition({ xPos,yPos });
	itemSlots.push_back(itemSlot);
	for (int i = 0; i < 7; i++) {
		if (i % 2 == 0) {
			itemSlot.setPosition({ xPos,yPos + 384 });
			itemSlots.push_back(itemSlot);
		}
		else {
			itemSlot.setPosition({ xPos += 384,yPos });
			itemSlots.push_back(itemSlot);
		}
	}
}

void InventoryUI::draw(sf::RenderWindow& window, Inventory& inventory) const {
	window.draw(inventoryOverlay);
	window.draw(inventoryCloseButton);
	window.draw(inventoryTitle);
	for (auto itemSlot : itemSlots) {
		window.draw(itemSlot);
	}
	for (int i = 0; i < inventory.getItems().size(); i++) {
		auto item = inventory.getItems()[i];
		item->setPosition({ static_cast<float>(itemSlots[i].getPosition().x),static_cast<float>(itemSlots[i].getPosition().y) });
		item->draw(window);
	}
}

void InventoryUI::mouseClick(sf::Event::MouseButtonPressed const& e, Inventory& inventory) {
	if (e.button == sf::Mouse::Button::Left) {
		if (inventoryCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			setInventoryStatus(false);
			inventoryCloseButton.setFillColor(sf::Color::Black);
			itemOptions.setChoosingStatus(false);
		}
		else {
			if (!itemOptions.getChoosingStatus()) {
				for (int i = 0; i < itemSlots.size(); i++) {
					if (itemSlots[i].getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
						if (i < inventory.getItems().size()) {
							itemOptions.setChoosingStatus(true);
							itemOptions.setPosition(getSlotPosition(itemSlots[i]));
							itemOptions.setSelectedItem(inventory.getItems()[i]);
						}
					}
				}
			}
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

bool InventoryUI::getInventoryStatus() const {
	return isOpen;
}

void InventoryUI::setInventoryStatus(bool status) {
	isOpen = status;
}

sf::Vector2f InventoryUI::getSlotPosition(sf::RectangleShape slot) {
	return slot.getPosition();
}
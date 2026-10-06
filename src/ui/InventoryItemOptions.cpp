
#include "InventoryItemOptions.h"
#include "gameplay/Inventory.h"


InventoryItemOptions::InventoryItemOptions(sf::RenderWindow& window, sf::Font& font) {
	choicesOverlay.setSize(sf::Vector2f{ 384,256 });
	choicesOverlay.setFillColor(sf::Color::Green);
	choicesOverlay.setOrigin({ choicesOverlay.getLocalBounds().size.x / 2, choicesOverlay.getLocalBounds().size.y / 2 });
	sf::Text text = { font,"",128 };
	text.setFillColor(sf::Color::White);
	text.setOrigin({ text.getLocalBounds().size.x / 2, text.getLocalBounds().size.y / 2 });
	for (int i = 0; i < 2; i++) {
		if (i % 2 == 0) {
			text.setString("Equip");
			choicesText.push_back(text);
		}
		else {
			text.setString("Remove");
			choicesText.push_back(text);
		}
	}
	optionsCloseButton.setSize(sf::Vector2f{ 32,32 });
	optionsCloseButton.setFillColor(sf::Color::Black);
	optionsCloseButton.setOrigin({ optionsCloseButton.getLocalBounds().size.x / 2,optionsCloseButton.getLocalBounds().size.y / 2 });
}

void InventoryItemOptions::draw(sf::RenderWindow& window) const {
	window.draw(choicesOverlay);
	window.draw(optionsCloseButton);
	for (int i = 0; i < choicesText.size(); i++) {
		window.draw(choicesText[i]);
	}
}

bool InventoryItemOptions::getChoosingStatus() const {
	return isChoosing;
}

void InventoryItemOptions::setChoosingStatus(bool status) {
	isChoosing = status;
}

void InventoryItemOptions::setPosition(sf::Vector2f pos) {
	choicesOverlay.setPosition({ pos.x,pos.y + 256 });
	optionsCloseButton.setPosition({ static_cast<float>(choicesOverlay.getPosition().x + choicesOverlay.getSize().x / 2) - 16,static_cast<float>(choicesOverlay.getPosition().y - choicesOverlay.getSize().y / 2) + 16 });
	for (int i = 0; i < choicesText.size(); i++) {
		if (i % 2 == 0) {
			choicesText[i].setPosition({ static_cast<float>(choicesOverlay.getPosition().x - choicesOverlay.getSize().x / 2),static_cast<float>(choicesOverlay.getPosition().y - choicesOverlay.getSize().y / 2 - 32) });
		}
		else {
			choicesText[i].setPosition({ static_cast<float>(choicesOverlay.getPosition().x - choicesOverlay.getSize().x / 2),static_cast<float>(choicesOverlay.getPosition().y - 32) });
		}
	}
}

void InventoryItemOptions::mouseClick(sf::Event::MouseButtonPressed const& e, Player& player, Inventory& inventory) {
	if (e.button == sf::Mouse::Button::Left) {
		if (optionsCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			setChoosingStatus(false);
			optionsCloseButton.setFillColor(sf::Color::Black);
		}
		else {
			for (int i = 0; i < choicesText.size(); i++) {
				if (choicesText[i].getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
					if (i == 0) {
						selectedItem->equip(player, inventory);
					}
					else if (i == 1) {
						inventory.removeItem(selectedItem);
					}
					setChoosingStatus(false);
				}
			}
		}
	}
}

void InventoryItemOptions::mouseMove(sf::Event::MouseMoved const& e) {
	for (int i = 0; i < choicesText.size(); i++) {
		if (choicesText[i].getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			choicesText[i].setFillColor(sf::Color::Blue);
		}
		else {
			choicesText[i].setFillColor(sf::Color::White);
		}
	}
	if (optionsCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
		optionsCloseButton.setFillColor(sf::Color::Blue);
	}
	else {
		optionsCloseButton.setFillColor(sf::Color::Black);
	}
}

void InventoryItemOptions::setSelectedItem(Item* item) {
	selectedItem = item;
}
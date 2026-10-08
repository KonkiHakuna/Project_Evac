#include "InventoryItemOptions.h"
#include "gameplay/Inventory.h"


InventoryItemOptions::InventoryItemOptions(
	sf::RenderWindow& window,
	sf::Font& font
) {

	// Configure the background of the item action menu.
	choicesOverlay.setSize(sf::Vector2f{ 384,256 });
	choicesOverlay.setFillColor(sf::Color::Green);

	choicesOverlay.setOrigin({
		choicesOverlay.getLocalBounds().size.x / 2,
		choicesOverlay.getLocalBounds().size.y / 2
		});


	// Create a reusable text object for the available actions.
	sf::Text text = { font,"",128 };

	text.setFillColor(sf::Color::White);

	text.setOrigin({
		text.getLocalBounds().size.x / 2,
		text.getLocalBounds().size.y / 2
		});


	// Create two menu entries: Equip and Remove.
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


	// Configure the small close button.
	optionsCloseButton.setSize(sf::Vector2f{ 32,32 });
	optionsCloseButton.setFillColor(sf::Color::Black);

	optionsCloseButton.setOrigin({
		optionsCloseButton.getLocalBounds().size.x / 2,
		optionsCloseButton.getLocalBounds().size.y / 2
		});
}


void InventoryItemOptions::draw(sf::RenderWindow& window) const {

	window.draw(choicesOverlay);
	window.draw(optionsCloseButton);


	// Draw every available action label.
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

	// Place the options menu below the selected inventory slot.
	choicesOverlay.setPosition({
		pos.x,
		pos.y + 256
		});


	// Position the close button in the top-right corner of the overlay.
	optionsCloseButton.setPosition({
		static_cast<float>(
			choicesOverlay.getPosition().x
			+ choicesOverlay.getSize().x / 2
		) - 16,

		static_cast<float>(
			choicesOverlay.getPosition().y
			- choicesOverlay.getSize().y / 2
		) + 16
		});


	// Position each action inside the overlay.
	for (int i = 0; i < choicesText.size(); i++) {

		if (i % 2 == 0) {

			choicesText[i].setPosition({
				static_cast<float>(
					choicesOverlay.getPosition().x
					- choicesOverlay.getSize().x / 2
				),

				static_cast<float>(
					choicesOverlay.getPosition().y
					- choicesOverlay.getSize().y / 2
					- 32
				)
				});
		}

		else {

			choicesText[i].setPosition({
				static_cast<float>(
					choicesOverlay.getPosition().x
					- choicesOverlay.getSize().x / 2
				),

				static_cast<float>(
					choicesOverlay.getPosition().y
					- 32
				)
				});
		}
	}
}


void InventoryItemOptions::mouseClick(
	sf::Event::MouseButtonPressed const& e,
	Player& player,
	Inventory& inventory
) {

	if (e.button == sf::Mouse::Button::Left) {


		// Close the action menu when the close button is clicked.
		if (
			optionsCloseButton.getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			setChoosingStatus(false);

			optionsCloseButton.setFillColor(
				sf::Color::Black
			);
		}


		else {

			// Check which action label was clicked.
			for (int i = 0; i < choicesText.size(); i++) {

				if (
					choicesText[i].getGlobalBounds().contains({
						static_cast<float>(e.position.x),
						static_cast<float>(e.position.y)
						})
					) {


					// First option equips or uses the selected item.
					if (i == 0) {

						selectedItem->equip(
							player,
							inventory
						);
					}


					// Second option removes the selected item
					// from the owning Inventory.
					else if (i == 1) {

						inventory.removeItem(
							selectedItem
						);
					}


					// Close the menu after completing the selected action.
					setChoosingStatus(false);
				}
			}
		}
	}
}


void InventoryItemOptions::mouseMove(
	sf::Event::MouseMoved const& e
) {

	// Highlight whichever action is currently under the mouse.
	for (int i = 0; i < choicesText.size(); i++) {

		if (
			choicesText[i].getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			choicesText[i].setFillColor(
				sf::Color::Blue
			);
		}

		else {

			choicesText[i].setFillColor(
				sf::Color::White
			);
		}
	}


	// Highlight the close button separately.
	if (
		optionsCloseButton.getGlobalBounds().contains({
			static_cast<float>(e.position.x),
			static_cast<float>(e.position.y)
			})
		) {

		optionsCloseButton.setFillColor(
			sf::Color::Blue
		);
	}

	else {

		optionsCloseButton.setFillColor(
			sf::Color::Black
		);
	}
}


void InventoryItemOptions::setSelectedItem(Item* item) {

	// No ownership is transferred here.
	// The selected item is still owned by Inventory.
	selectedItem = item;
}
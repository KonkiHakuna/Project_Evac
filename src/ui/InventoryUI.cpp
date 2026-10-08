#include "InventoryUI.h"
#include "InventoryItemOptions.h"
#include "gameplay/Inventory.h"


InventoryUI::InventoryUI(
	sf::RenderWindow& window,
	sf::Font& font,
	InventoryItemOptions& options
)
	: inventoryTitle(font, "Inventory", 256),
	itemOptions(options) {


	// Configure the main inventory background.
	inventoryOverlay.setSize(sf::Vector2f{ 1536,1024 });
	inventoryOverlay.setFillColor(sf::Color::Red);

	inventoryOverlay.setOrigin({
		inventoryOverlay.getLocalBounds().size.x / 2,
		inventoryOverlay.getLocalBounds().size.y / 2
		});

	inventoryOverlay.setPosition({
		static_cast<float>(window.getSize().x / 2),
		static_cast<float>(window.getSize().y / 2)
		});


	// Configure the close button.
	inventoryCloseButton.setSize(sf::Vector2f{ 64,64 });
	inventoryCloseButton.setFillColor(sf::Color::Black);

	inventoryCloseButton.setOrigin({
		inventoryCloseButton.getLocalBounds().size.x / 2,
		inventoryCloseButton.getLocalBounds().size.y / 2
		});

	inventoryCloseButton.setPosition({
		static_cast<float>(
			inventoryOverlay.getSize().x / 2
			+ inventoryOverlay.getPosition().x
		) - 32,
		static_cast<float>(
			inventoryOverlay.getPosition().y / 2
		)
		});


	// Configure the inventory title.
	inventoryTitle.setFillColor(sf::Color::White);

	inventoryTitle.setOrigin({
		inventoryTitle.getLocalBounds().size.x / 2,
		inventoryTitle.getLocalBounds().size.y / 2
		});

	inventoryTitle.setPosition({
		static_cast<float>(
			inventoryOverlay.getPosition().x
		),
		static_cast<float>(
			inventoryOverlay.getPosition().y
			- inventoryOverlay.getSize().y / 2
			+ inventoryTitle.getLocalBounds().size.y
		) - 128
		});


	// Configure the reusable slot shape.
	itemSlot.setSize(sf::Vector2f{ 256, 256 });
	itemSlot.setFillColor(sf::Color(74, 74, 74));

	itemSlot.setOrigin({
		itemSlot.getLocalBounds().size.x / 2,
		itemSlot.getLocalBounds().size.y / 2
		});


	// Starting point used to build the slot layout.
	float xPos =
		inventoryOverlay.getPosition().x
		- inventoryOverlay.getSize().x / 2
		+ 192;

	float yPos =
		inventoryOverlay.getPosition().y
		- 64;


	itemSlot.setPosition({
		xPos,
		yPos
		});

	itemSlots.push_back(itemSlot);


	// Build the remaining inventory slots by moving
	// the reusable rectangle between predefined positions.
	for (int i = 0; i < 7; i++) {

		if (i % 2 == 0) {

			itemSlot.setPosition({
				xPos,
				yPos + 384
				});

			itemSlots.push_back(itemSlot);
		}

		else {

			itemSlot.setPosition({
				xPos += 384,
				yPos
				});

			itemSlots.push_back(itemSlot);
		}
	}
}


void InventoryUI::draw(
	sf::RenderWindow& window,
	Inventory& inventory
) const {

	// Draw the inventory UI before drawing the actual item icons.
	window.draw(inventoryOverlay);
	window.draw(inventoryCloseButton);
	window.draw(inventoryTitle);


	// itemSlot is copied here, because the loop only needs
	// each rectangle temporarily for drawing.
	for (auto itemSlot : itemSlots) {
		window.draw(itemSlot);
	}


	// Draw inventory items inside the corresponding UI slots.
	for (int i = 0; i < inventory.getItems().size(); i++) {

		auto item = inventory.getItems()[i];


		item->setPosition({
			static_cast<float>(
				itemSlots[i].getPosition().x
			),
			static_cast<float>(
				itemSlots[i].getPosition().y
			)
			});


		item->draw(window);
	}
}


void InventoryUI::mouseClick(
	sf::Event::MouseButtonPressed const& e,
	Inventory& inventory
) {

	if (e.button == sf::Mouse::Button::Left) {


		// Close the inventory when the close button is clicked.
		if (
			inventoryCloseButton.getGlobalBounds().contains({
				static_cast<float>(e.position.x),
				static_cast<float>(e.position.y)
				})
			) {

			setInventoryStatus(false);

			inventoryCloseButton.setFillColor(
				sf::Color::Black
			);


			// Also close the item options menu,
			// because it belongs to the inventory UI.
			itemOptions.setChoosingStatus(false);
		}


		else {

			// Do not allow another item to be selected
			// while the options menu is already open.
			if (!itemOptions.getChoosingStatus()) {


				// Check which inventory slot was clicked.
				for (int i = 0; i < itemSlots.size(); i++) {


					if (
						itemSlots[i].getGlobalBounds().contains({
							static_cast<float>(e.position.x),
							static_cast<float>(e.position.y)
							})
						) {


						// A slot can only be selected when it actually
						// contains an item from the Inventory.
						if (i < inventory.getItems().size()) {

							itemOptions.setChoosingStatus(true);


							// Position the options menu next to
							// the slot that was clicked.
							itemOptions.setPosition(
								getSlotPosition(itemSlots[i])
							);


							// Inventory owns the Item.
							// InventoryItemOptions only keeps a pointer
							// to the currently selected one.
							itemOptions.setSelectedItem(
								inventory.getItems()[i]
							);
						}
					}
				}
			}
		}
	}
}


void InventoryUI::mouseMove(
	sf::Event::MouseMoved const& e
) {

	// Highlight the close button while the mouse is over it.
	if (
		inventoryCloseButton.getGlobalBounds().contains({
			static_cast<float>(e.position.x),
			static_cast<float>(e.position.y)
			})
		) {

		inventoryCloseButton.setFillColor(
			sf::Color::Blue
		);
	}

	else {

		inventoryCloseButton.setFillColor(
			sf::Color::Black
		);
	}
}


bool InventoryUI::getInventoryStatus() const {
	return isOpen;
}


void InventoryUI::setInventoryStatus(bool status) {
	isOpen = status;
}


sf::Vector2f InventoryUI::getSlotPosition(
	sf::RectangleShape slot
) {

	return slot.getPosition();
}
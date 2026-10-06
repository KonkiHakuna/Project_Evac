
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font) : options{ "Resume","Quit" }, saveInformations(font, "Press K to save | Press L to load", 128) {
	float y = 0;
	for (const auto& option : options) {
		sf::Text text(font, option, 128);
		text.setFillColor(sf::Color::White);
		text.setOrigin({ text.getLocalBounds().size.x / 2,text.getLocalBounds().size.y / 2 });
		text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) + y });
		menuOptions.push_back(text);
		y += 150;
	}
	saveInformations.setFillColor(sf::Color::White);
	saveInformations.setOrigin({ saveInformations.getLocalBounds().size.x / 2,saveInformations.getLocalBounds().size.y / 2 });
	saveInformations.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(128) });
}

void PauseMenu::draw(sf::RenderWindow& window) const {
	for (const auto& option : menuOptions) {
		window.draw(option);
	}
	window.draw(saveInformations);
}

void PauseMenu::mouseClick(sf::Event::MouseButtonPressed const& e) {
	if (e.button == sf::Mouse::Button::Left) {
		for (int i = 0; i < menuOptions.size(); ++i) {
			if (menuOptions[i].getGlobalBounds().contains({ static_cast<float>(e.position.x), static_cast<float>(e.position.y) })) {
				decision = i;
				break;
			}
		}
	}
}

void PauseMenu::mouseMove(sf::Event::MouseMoved const& e) {
	for (auto& option : menuOptions) {
		if (option.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			option.setFillColor(sf::Color::Red);
		}
		else {
			option.setFillColor(sf::Color::White);
		}
	}
}

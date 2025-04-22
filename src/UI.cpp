
#include "UI.h"

/*============================== Start Screen =============================*/

StartScreen::StartScreen(sf::RenderWindow& window, sf::Font& font) : title{ font,"Evac",256 }, pressSpace{ font,"Press space to continue",128 } {
	title.setFillColor(sf::Color::White);
	title.setOrigin({ static_cast<float>(title.getLocalBounds().size.x / 2),static_cast<float>(title.getLocalBounds().size.y) / 2 });
	title.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});
	
	pressSpace.setFillColor(sf::Color::White);
	pressSpace.setOrigin({ static_cast<float>(pressSpace.getLocalBounds().size.x / 2),static_cast<float>(pressSpace.getLocalBounds().size.y) / 2 });
	pressSpace.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2) + 256 });

}

void StartScreen::drawStartScreen(sf::RenderWindow& window) {
	window.draw(title);;
	window.draw(pressSpace);
}

/*============================= Pause Menu =============================*/

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font) {
	std::vector<std::string> options = { "Resume", "Quit" };
	float y = 0;
	for (const auto& option : options) {
		sf::Text text(font,option,128);
		text.setFillColor(sf::Color::White);
		text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
		text.setPosition({static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)+y});
		menuOptions.push_back(text);
		y += 150;
	}
}

void PauseMenu::drawPauseMenu(sf::RenderWindow& window) const {
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

void Shop::interact(sf::RenderWindow& window, const sf::Font& font) {
	if (!isOpen) {
		sf::Text text{ font,"Press E to interact",128 };
		text.setFillColor(sf::Color::White);
		text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
		text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y)-256});
		window.draw(text);
	}
}

void Shop::keyPressed() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::E)) {
		isOpen = true;
	}
}


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
		text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
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

Shop::Shop(sf::RenderWindow& window) {
	shopOverlay.setSize(sf::Vector2f{1024,1024});
	shopOverlay.setFillColor(sf::Color::Red);
	shopOverlay.setOrigin({ static_cast<float>(shopOverlay.getLocalBounds().size.x / 2),static_cast<float>(shopOverlay.getLocalBounds().size.y / 2)});
	shopOverlay.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y / 2)});

	shopCloseButton.setSize(sf::Vector2f{ 64,64 });
	shopCloseButton.setFillColor(sf::Color::Black);
	shopCloseButton.setOrigin({ static_cast<float>(shopCloseButton.getLocalBounds().size.x / 2) + 32,static_cast<float>(shopCloseButton.getLocalBounds().size.y / 2)});
	shopCloseButton.setPosition({ static_cast<float>(shopOverlay.getSize().x / 2 + shopOverlay.getPosition().x),static_cast<float>(shopOverlay.getPosition().y / 2)});

	shopItemFrame_1.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_1.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_1.setOrigin({ static_cast<float>(shopItemFrame_1.getLocalBounds().size.x / 2),static_cast<float>(shopItemFrame_1.getLocalBounds().size.y / 2)});
	shopItemFrame_1.setPosition({ static_cast<float>(shopOverlay.getPosition().x-shopOverlay.getSize().x / 2 + shopItemFrame_1.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_1.getSize().y)});

	shopItemFrame_2.setSize(sf::Vector2f{ 256,256 });
	shopItemFrame_2.setFillColor(sf::Color(74, 74, 74));
	shopItemFrame_2.setOrigin({ static_cast<float>(shopItemFrame_2.getLocalBounds().size.x / 2),static_cast<float>(shopItemFrame_2.getLocalBounds().size.y / 2) });
	shopItemFrame_2.setPosition({ static_cast<float>(shopOverlay.getPosition().x + shopOverlay.getSize().x / 2 - shopItemFrame_2.getSize().x), static_cast<float>((shopOverlay.getPosition().y + shopOverlay.getPosition().y) / 2 + shopItemFrame_2.getSize().y)});
}

void Shop::interact(sf::RenderWindow& window, sf::Font& font) {
	if (!isOpen) {
		sf::Text text{ font,"Press E to interact",128 };
		text.setFillColor(sf::Color::White);
		text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
		text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y)-256});
		window.draw(text);
	}
}

void Shop::draw(sf::RenderWindow& window, sf::Font& font, std::string name) {
	sf::Text text{ font,name,256 };
	text.setFillColor(sf::Color::White);
	text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
	text.setPosition({ static_cast<float>(shopOverlay.getPosition().x),static_cast<float>(shopOverlay.getPosition().y - shopOverlay.getSize().y * 0.375) });
	window.draw(shopOverlay);
	window.draw(shopCloseButton);
	window.draw(shopItemFrame_1);
	window.draw(shopItemFrame_2);
	window.draw(text);

}

void Shop::mouseClick(sf::Event::MouseButtonPressed const& e) {
	if (e.button == sf::Mouse::Button::Left) {
		if (shopCloseButton.getGlobalBounds().contains({ static_cast<float>(e.position.x),static_cast<float>(e.position.y) })) {
			isOpen = false;
		}
	}
}

bool Shop::getShoppingStatus() {
	return isOpen;
}
void Shop::setShoppingStatus(bool status) {
	isOpen = status;
}

/*============================= Mine =============================*/

void MineEntrance::interact(sf::RenderWindow& window, sf::Font& font) {
	sf::Text text{font,"Press E to enter",128};
	text.setFillColor(sf::Color::White);
	text.setOrigin({ static_cast<float>(text.getLocalBounds().size.x / 2),static_cast<float>(text.getLocalBounds().size.y) / 2 });
	text.setPosition({ static_cast<float>(window.getSize().x / 2),static_cast<float>(window.getSize().y) - 256 });
	window.draw(text);
}



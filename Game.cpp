
#include "Game.h"

Game::Game() {
	mode = sf::VideoMode::getDesktopMode();
	window.create(mode, "Evac", sf::Style::Default,sf::State::Fullscreen);
	window.setFramerateLimit(240);
	if (!caveatFont.openFromFile("assets/fonts/Caveat_font.ttf")) {
		throw std::runtime_error("Failed to load font");
	}
	startScreen = StartScreen(window, caveatFont);
	pauseMenu = PauseMenu(window, caveatFont);
}
void Game::run() {
	while (window.isOpen()) {
		while (auto const event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (isStartScreen){
				if (event->is<sf::Event::KeyPressed>()){
					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Space) {
						isStartScreen = false;
					}
				}
			}
			else {
				if (event->is<sf::Event::KeyPressed>()) {
					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) {
						isPaused = !isPaused;
					}
				}
				if (isPaused) {
					if (event->is<sf::Event::MouseMoved>()) {
						pauseMenu.mouseMove(*event->getIf<sf::Event::MouseMoved>());
					}
					if (event->is<sf::Event::MouseButtonPressed>()) {
						pauseMenu.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>());
						if (pauseMenu.getDecision() == 0) {
							isPaused = false;
						}
						else if (pauseMenu.getDecision() == 1) {
							window.close();
						}
						pauseMenu.resetDecision();
					}
				}
			}
		}
		window.clear(sf::Color::Black);
		if (isStartScreen) {
			startScreen.drawStartScreen(window);
		}
		else {
			if (isPaused) {
				pauseMenu.drawPauseMenu(window);
			}
		}
		window.display();
	}
}


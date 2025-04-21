
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
	lobby = Lobby(window);
	player = Player(window);
}
void Game::run() {
	while (window.isOpen()) {
		while (auto const event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			if (currentGameState == GameState::startScreen){
				if (event->is<sf::Event::KeyPressed>()){
					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Space) {
						currentGameState = GameState::lobby;
					}
				}
			}
			else {
				if (event->is<sf::Event::KeyPressed>()) {
					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) {
						if (currentGameState != GameState::paused) {
							previousGameState = currentGameState;
							currentGameState = GameState::paused;
						}
						else {
							currentGameState = previousGameState;
						}
					}
				}
				if (currentGameState == GameState::paused) {
					if (event->is<sf::Event::MouseMoved>()) {
						pauseMenu.mouseMove(*event->getIf<sf::Event::MouseMoved>());
					}
					if (event->is<sf::Event::MouseButtonPressed>()) {
						pauseMenu.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>());
						if (pauseMenu.getDecision() == 0) {
							currentGameState = previousGameState;
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
		switch (currentGameState) {
			case GameState::startScreen:
				startScreen.drawStartScreen(window);
				break;
			case GameState::lobby:
				player.movement();
				lobby.draw(window);
				player.draw(window);
				break;
			case GameState::paused:
				pauseMenu.drawPauseMenu(window);
				break;
			default:
				throw std::runtime_error("Invalid game state");
		}
		window.display();
	}
}


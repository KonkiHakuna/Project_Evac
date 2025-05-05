
#include "Game.h"

Game::Game() {
	mode = sf::VideoMode::getDesktopMode();
	window.create(mode, "Evac", sf::Style::Default,sf::State::Fullscreen);
	window.setFramerateLimit(240);
	if (!caveatFont.openFromFile("assets/fonts/Caveat_font.ttf")) {
		throw std::exception("Failed to load font");
	}
	startScreen = StartScreen(window, caveatFont);
	pauseMenu = PauseMenu(window, caveatFont);
	lobby = Lobby(window);
	player = Player(window);
	shop = Shop(window, caveatFont);
	inventoryUI = InventoryUI(window, caveatFont);
	hud = HUD{ window,caveatFont };
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
				if (currentGameState == GameState::lobby) {
					if (lobby.playerLocation(player) == LobbyLocation::caveEntrance) {
						if (event->is<sf::Event::KeyPressed>()) {
							if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::E) {
								currentGameState = GameState::mine;
								player.resetPosition(window);
							}
						}
					}
					else{
						if (lobby.playerLocation(player)!=LobbyLocation::Default){
							if (!shop.getShoppingStatus()) {
								if (event->is<sf::Event::KeyPressed>()) {
									if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::E) {
										inventoryUI.setInventoryStatus(false);
										shop.setShoppingStatus(true);
										shop.setCurrentShopLocation(lobby.playerLocation(player));
									}
								}
							}
							else {
								if (event->is<sf::Event::MouseMoved>()) {
									shop.mouseMove(*event->getIf<sf::Event::MouseMoved>());
								}
								if (event->is<sf::Event::MouseButtonPressed>()) {
									if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
										shop.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>(),inventory);
									}
								}
							}
						}
						if (!shop.getShoppingStatus()) {
							if (event->is<sf::Event::KeyPressed>()) {
								if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Tab) {
									if (inventoryUI.getInventoryStatus()) {
										inventoryUI.setInventoryStatus(false);
									}
									else {
										inventoryUI.setInventoryStatus(true);
									}
								}
							}
						}
						if (inventoryUI.getInventoryStatus()) {
							if (event->is<sf::Event::MouseMoved>()) {
								inventoryUI.mouseMove(*event->getIf<sf::Event::MouseMoved>());
							}
							if (event->is<sf::Event::MouseButtonPressed>()) {
								if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
									inventoryUI.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>());
								}
							}
						}
					}
				}
			}
		}
		window.clear(sf::Color::Black);
		switch (currentGameState) {
			case GameState::startScreen:
				startScreen.draw(window);
				break;
			case GameState::lobby:
				player.movement();
				lobby.draw(window);
				player.draw(window);
				if (inventoryUI.getInventoryStatus()) {
					inventoryUI.draw(window,inventory);
				}
				if (lobby.playerLocation(player)!=LobbyLocation::Default) {
					if (lobby.playerLocation(player) == LobbyLocation::caveEntrance) {
						mineEntrance.interact(window,caveatFont);
					}
					else{
						if (!shop.getShoppingStatus()) {
							shop.interact(window, caveatFont);
						}
						else {
							shop.draw(window,lobby.getName(lobby.playerLocation(player)));
						}
					}
				}
				else {
					shop.setShoppingStatus(false);
				}
				hud.draw(window,player,inventory);
				break;
			case GameState::mine:
				player.movement();
				player.draw(window);
				hud.draw(window,player,inventory);
				break;
			case GameState::paused:
				pauseMenu.draw(window);
				break;
			default:
				throw std::exception("Invalid game state");
		}
		window.display();
		}
	}


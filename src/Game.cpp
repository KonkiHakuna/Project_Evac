
#include "Game.h"

#include "fmt/base.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(sf::Vector2u{2880,1920}), "Evac", sf::Style::Default, sf::State::Fullscreen),
startScreen(window, caveatFont), pauseMenu(window, caveatFont), lobby(window),
player(window), shop(window, caveatFont),cave(window,caveatFont), hud(window,caveatFont),
inventoryItemOptions(window,caveatFont), inventoryUI(window,caveatFont,inventoryItemOptions) {
	srand(time(nullptr));
	window.setFramerateLimit(240);
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
					if (event->is<sf::Event::KeyPressed>()) {
						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::K) {
							save.SaveGame(player, inventory);
						}
						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::L) {
							save.LoadGame(player, inventory);
						}
					}
				}
				if (currentGameState == GameState::lobby) {
					if (lobby.playerLocation(player) == LobbyLocation::caveEntrance) {
						if (event->is<sf::Event::KeyPressed>()) {
							if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::E) {
								currentGameState = GameState::cave;
								player.resetPosition(window);
								cave.clear();
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
								inventoryUI.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>(), inventory);
							}
						}
					}
					if (inventoryItemOptions.getChoosingStatus()) {
						if (event->is<sf::Event::MouseMoved>()) {
							inventoryItemOptions.mouseMove(*event->getIf<sf::Event::MouseMoved>());
						}
						if (event->is<sf::Event::MouseButtonPressed>()) {
							if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
								inventoryItemOptions.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>(),player,inventory);
							}
						}
					}
				}
				if (currentGameState == GameState::cave) {
					if (event->is<sf::Event::MouseButtonPressed>()) {
						player.attack(*event->getIf<sf::Event::MouseButtonPressed>(), inventory, cave.getEnemies());
					}
					if (cave.getCurrentWave()>10) {
						currentGameState = GameState::lobby;
						cave.clear();
						player.resetPosition(window);
						shop.resetShopItems();
					}
					if (player.getHealth()==0) {
						currentGameState = GameState::lobby;
						cave.clear();
						player.setHealth(50, '=');
						player.setMana(50, '=');
						player.resetPosition(window);
						shop.resetShopItems();
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
				player.movementBounds();
				lobby.draw(window);
				player.draw(window);
				if (inventoryUI.getInventoryStatus()) {
					inventoryUI.draw(window,inventory);
					if (inventoryItemOptions.getChoosingStatus()) {
						inventoryItemOptions.draw(window);
					}
				}
				if (lobby.playerLocation(player)!=LobbyLocation::Default) {
					if (lobby.playerLocation(player) == LobbyLocation::caveEntrance) {
						shop.interact(window,caveatFont);
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
			case GameState::cave:
				player.movement();
				player.movementBounds();
				cave.levelUpdate();
				cave.draw(window);
				for (auto& enemy : cave.getEnemies()) {
					enemy->movement(player.getPosition());
					enemy->attack(player);
					enemy->draw(window);
				}
				player.draw(window);
				hud.draw(window, player, inventory);
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


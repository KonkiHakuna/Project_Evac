
#include "Game.h"

#include "world/Lobby.h"
#include "ui/StartScreen.h"
#include "ui/PauseMenu.h"
#include "ui/hud.h"
#include "ui/InventoryUI.h"

Game::Game() : window(sf::VideoMode(sf::Vector2u{2880,1920}), "Evac", sf::Style::Default, sf::State::Fullscreen){
	srand(time(nullptr));
	window.setFramerateLimit(240);
}
void Game::run() {
	sf::Clock frameClock;
	while (window.isOpen()) {
		float deltaTime = frameClock.restart().asSeconds();
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
								projectiles.clear();
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
						player.attack(*event->getIf<sf::Event::MouseButtonPressed>(), inventory, cave.getEnemies(), 
							projectiles, window.mapPixelToCoords(event->getIf<sf::Event::MouseButtonPressed>()->position));
					}
					if (event->is<sf::Event::KeyPressed>()) {
						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::LShift) {
							player.dash();
						}
					}
					if (cave.getCurrentWave()>10) {
						currentGameState = GameState::lobby;
						cave.clear();
						projectiles.clear();
						player.resetPosition(window);
					}
					if (player.getHealth()==0) {
						currentGameState = GameState::lobby;
						cave.clear();
						projectiles.clear();
						player.setHealth(50);
						if (player.getMana()<50) {
							player.setMana(50);
						}
						player.resetPosition(window);
					}

				}
			}
		}
		window.clear(sf::Color::Black);

		switch (currentGameState) {
			case GameState::startScreen:
				startScreen.draw(window);
				break;
			case GameState::lobby: {
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
			}
			case GameState::cave: {
				player.movement();
				player.movementBounds();
				cave.levelUpdate();
				cave.draw(window);
				auto& enemies = cave.getEnemies();
				int numberOfEnemies = enemies.size();
				if (numberOfEnemies>0) {
					sf::Vector2f playerPos = player.getPosition();
					std::vector<std::vector<int>> collisionList(numberOfEnemies);
					for (int i = 0; i < numberOfEnemies; i++)
					{
						for (int j = i + 1; j < numberOfEnemies; j++)
						{
							if (enemies[i]->getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds()))
							{
								collisionList[i].push_back(j);
								collisionList[j].push_back(i);
							}
						}
					}

					std::vector<int> groupId(numberOfEnemies, -1);
					int groupCount = 0;
					for (int i = 0; i < numberOfEnemies; i++)
					{
						if (groupId[i] == -1) {
							std::vector<int> stack;
							stack.push_back(i);
							groupId[i] = groupCount;

							while (!stack.empty())
							{
								int n = stack.back();
								stack.pop_back();

								for (int a : collisionList[n])
								{
									if (groupId[a] == -1)
									{
										groupId[a] = groupCount;
										stack.push_back(a);
									}
								}
							}

							++groupCount;
						}
					}

					std::vector<std::vector<int>> groups(groupCount);
					for (int i = 0; i < numberOfEnemies; ++i)
					{
						groups[groupId[i]].push_back(i);
					}
					std::vector<int> groupOrder(groupCount);
					for (int g = 0; g < groupCount; ++g)
					{
						groupOrder[g] = g;
					}

					auto distanceToPlayer = [](sf::Vector2f a, sf::Vector2f b) {
						return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
					};

					std::ranges::sort(groupOrder,[&](int g1, int g2){
						float best1 = std::numeric_limits<float>::max();
						float best2 = std::numeric_limits<float>::max();

						for (int g : groups[g1])
						{
							float d = distanceToPlayer(enemies[g]->getPosition(), playerPos);
							if (d < best1) best1 = d;
						}

						for (int g : groups[g2])
						{
							float d = distanceToPlayer(enemies[g]->getPosition(), playerPos);
							if (d < best2) best2 = d;
						}

						return best1 < best2;
						});

					std::vector<int> proccesedEnemies;
					proccesedEnemies.reserve(numberOfEnemies);
					for (int g : groupOrder)
					{
						std::ranges::sort(groups[g], [&](int a, int b) {
							float da = distanceToPlayer(enemies[a]->getPosition(), playerPos);
							float db = distanceToPlayer(enemies[b]->getPosition(), playerPos);

							if (da == db)
							{
								return std::rand() % 2 == 0;
							}

							return da < db;
							});

						for (int i = 0; i < groups[g].size(); i++)
						{
							int j = groups[g][i];
							auto& enemy = enemies[j];

							sf::Vector2f oldPosition = enemy->getPosition();

							enemy->movement(playerPos);

							bool collided = false;
							for (int k = 0; k < i && !collided; ++k)
							{
								int j = groups[g][k];
								if (enemy->getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds()))
								{
									collided = true;
									break;
								}
							}

							for (int j : proccesedEnemies)
							{
								if (enemy->getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds()))
								{
									collided = true;
									break;
								}
							}

							if (collided)
							{
								enemy->setPosition(oldPosition);
							}
							else
							{
								proccesedEnemies.push_back(j);
							}

							enemy->attack(player);
							enemy->draw(window);
						}
					}
				}

				for (int i = 0; i < projectiles.size();) {
					projectiles[i].update(deltaTime);

					bool removeProjectile = false;

					for (std::size_t j = 0; j < enemies.size(); ++j) {
						if (projectiles[i].getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds())) 
						{enemies[j]->takeDamage(projectiles[i].getDamage());

							removeProjectile = true;

							if (enemies[j]->isDead()) {
								switch (enemies[j]->getType()) {
								case EnemyType::NORMAL:
									inventory.setPlayerGold(4, '+');
									break;

								case EnemyType::QUICK:
									inventory.setPlayerGold(2, '+');
									break;

								case EnemyType::TANK:
									inventory.setPlayerGold(8, '+');
									break;

								case EnemyType::BOSS:
									inventory.setPlayerGold(25, '+');
									break;
								}

								player.restoreMana(5);

								enemies.erase(enemies.begin() + j);
							}

							break;
						}
					}

					sf::Vector2f projectilePosition =
						projectiles[i].getPosition();

					if (projectilePosition.x < 0 || projectilePosition.x > 2880 || projectilePosition.y < 0 || projectilePosition.y > 1920) {
						removeProjectile = true;
					}

					if (removeProjectile) {
						projectiles.erase(projectiles.begin() + i);
					}
					else {
						projectiles[i].draw(window);
						++i;
					}
				}
				player.draw(window);
				hud.draw(window, player, inventory);
				break;
			}
			case GameState::paused:
				pauseMenu.draw(window);
				break;
			default:
				throw std::exception("Invalid game state");
		}
		window.display();
		}
	}

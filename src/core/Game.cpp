
#include "Game.h"

#include "world/Lobby.h"
#include "ui/StartScreen.h"
#include "ui/PauseMenu.h"
#include "ui/hud.h"
#include "ui/InventoryUI.h"

Game::Game() : window(sf::VideoMode(sf::Vector2u{2880,1920}), "Evac", sf::Style::Default, sf::State::Fullscreen){
	
	// Seed rand() using the current time so random results
	// are different between game launches.
	srand(time(nullptr));

	// Limit the maximum number of frames per second to 240.
	window.setFramerateLimit(240);
}
void Game::run() {

	// Create a clock to measure the time between frames.
	sf::Clock frameClock;

	while (window.isOpen()) {

		// Get the time elapsed since the last frame and restart the clock.
		// It is used for frame-rate independent projectile movement.
		float deltaTime = frameClock.restart().asSeconds();



		// =========================
		// Event handling
		// =========================

		while (auto const event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			// Start screen input.
			if (currentGameState == GameState::startScreen){

				if (event->is<sf::Event::KeyPressed>()){

					// Start the game and enter the lobby.
					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Space) {
						currentGameState = GameState::lobby;
					}
				}
			}
			else {

				// Toggle pause with Escape.
				if (event->is<sf::Event::KeyPressed>()) {

					if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) {
						
						if (currentGameState != GameState::paused) {
							
							// Remember where the player was before opening the pause menu.
							previousGameState = currentGameState;
							currentGameState = GameState::paused;
						}
						else {

							// Return to the state that was active before pausing.
							currentGameState = previousGameState;
						}
					}
				}



				// =========================
				// Pause menu input
				// =========================

				if (currentGameState == GameState::paused) {

					if (event->is<sf::Event::MouseMoved>()) {
						pauseMenu.mouseMove(*event->getIf<sf::Event::MouseMoved>());
					}


					if (event->is<sf::Event::MouseButtonPressed>()) {
						pauseMenu.mouseClick(*event->getIf<sf::Event::MouseButtonPressed>());
						
						// Decision 0 resumes the previous game state.
						if (pauseMenu.getDecision() == 0) {
							currentGameState = previousGameState;
						}
						
						// Decision 1 closes the game.
						else if (pauseMenu.getDecision() == 1) {
							window.close();
						}

						// Clear the decision after handling the click.
						pauseMenu.resetDecision();
					}

					// Save and load shortcuts available while paused.
					if (event->is<sf::Event::KeyPressed>()) {

						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::K) {
							save.SaveGame(player, inventory);
						}

						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::L) {
							save.LoadGame(player, inventory);
						}
					}
				}



				// =========================
				// Lobby input
				// =========================

				if (currentGameState == GameState::lobby) {

					// Enter the cave when the player is standing
					// inside the cave entrance and presses E.
					if (lobby.playerLocation(player) == LobbyLocation::caveEntrance) {
						
						if (event->is<sf::Event::KeyPressed>()) {
							
							if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::E) {
								currentGameState = GameState::cave;
								player.resetPosition(window);
								
								// Reset the cave back to its first wave.
								cave.clear();

								// Remove projectiles left from the previous cave session.
								projectiles.clear();
							}
						}
					}

					// Handle interaction with the other lobby locations.
					else{

						if (lobby.playerLocation(player)!=LobbyLocation::Default){
							
							// Open the shop with E.
							if (!shop.getShoppingStatus()) {

								if (event->is<sf::Event::KeyPressed>()) {
									
									if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::E) {
										
										// Hide inventory so the shop and inventory
										// are not open at the same time.
										inventoryUI.setInventoryStatus(false);
										shop.setShoppingStatus(true);
										
										// Remember which shop the player is currently using.
										shop.setCurrentShopLocation(lobby.playerLocation(player));
									}
								}
							}

							// Shop mouse input while the shop is open.
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

					// Toggle inventory with Tab when the shop is closed.
					if (!shop.getShoppingStatus()) {
						
						if (event->is<sf::Event::KeyPressed>()) {
							
							if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Tab) {
								
								if (inventoryUI.getInventoryStatus()) {
									inventoryUI.setInventoryStatus(false);

									// Close the item action menu when the inventory is closed.
									inventoryItemOptions.setChoosingStatus(false);
									inventoryItemOptions.setSelectedItem(nullptr);
								}
								
								else {
									inventoryUI.setInventoryStatus(true);
								}
							}
						}
					}

					// Inventory mouse input.
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

					// Handle the additional menu shown after selecting an inventory item.
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
				

				// =========================
				// Cave input
				// =========================

				if (currentGameState == GameState::cave) {
					
					// Pass mouse clicks to the player's attack logic.
					if (event->is<sf::Event::MouseButtonPressed>()) {
						player.attack(*event->getIf<sf::Event::MouseButtonPressed>(), inventory, cave.getEnemies(), 
							projectiles, window.mapPixelToCoords(event->getIf<sf::Event::MouseButtonPressed>()->position));
					}
					
					// Dash with Left Shift.
					if (event->is<sf::Event::KeyPressed>()) {
						
						if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::LShift) {
							player.dash();
						}
					}
				}
			}
		}



		// Clear the previous frame before drawing the next one.
		window.clear(sf::Color::Black);



		// =========================
		// Update and rendering
		// =========================

		switch (currentGameState) {

			case GameState::startScreen:
				startScreen.draw(window);
				break;
			
			case GameState::lobby: {
				player.movement();
				player.movementBounds();
				lobby.draw(window);
				player.draw(window);

				// Draw inventory and the item action menu when they are open.
				if (inventoryUI.getInventoryStatus()) {
					inventoryUI.draw(window,inventory);
					
					if (inventoryItemOptions.getChoosingStatus()) {
						inventoryItemOptions.draw(window);
					}
				}

				// Show interaction UI when the player is standing
				// inside an interactive lobby location.
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

					// Walking away from a shop automatically closes it.
					shop.setShoppingStatus(false);
				}

				hud.draw(window,player,inventory);
				break;
			}
			case GameState::cave: {

				// Player death also returns to the lobby.
				if (player.getHealth() == 0) {
					currentGameState = GameState::lobby;
					cave.clear();
					projectiles.clear();
					player.setHealth(50);

					if (player.getMana() < 50) {
						player.setMana(50);
					}
					player.resetPosition(window);
					break;
				}

				player.movement();
				player.movementBounds();

				// Update wave progression and spawn the next wave.
				cave.levelUpdate();

				// Finishing wave 10 returns the player to the lobby.
				if (cave.getCurrentWave() > 10) {
					currentGameState = GameState::lobby;
					cave.clear();
					projectiles.clear();
					player.resetPosition(window);
					break;
				}

				cave.draw(window);
				
				// Work directly with Cave's enemy vector instead of making a copy.
				auto& enemies = cave.getEnemies();
				int numberOfEnemies = enemies.size();
				
				

				// =========================
				// Enemy movement and collision handling
				// =========================

				if (numberOfEnemies>0) {
					sf::Vector2f playerPos = player.getPosition();
					
					// Build an undirected graph of current enemy collisions.
					// collisionList[i] stores the indexes of enemies
					// whose bounds overlap enemy i.
					std::vector<std::vector<int>> collisionList(numberOfEnemies);
					
					// Check every unique pair once.
					// Starting j at i + 1 avoids comparing an enemy with itself
					// and avoids checking the same pair twice.
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

					// Find connected groups of enemies.
					// Enemies belong to the same group when they are connected
					// directly or indirectly through collisions.
					std::vector<int> groupId(numberOfEnemies, -1);
					int groupCount = 0;
					
					for (int i = 0; i < numberOfEnemies; i++)
					{
						if (groupId[i] == -1) {
							
							// Use an explicit stack to traverse
							// the connected collision graph.
							// It is baically a depth-first search without recursion.
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

					// Convert the group IDs into vectors containing
					// the enemy indexes belonging to each group.
					std::vector<std::vector<int>> groups(groupCount);
					
					for (int i = 0; i < numberOfEnemies; ++i)
					{
						groups[groupId[i]].push_back(i);
					}

					// Store group indexes separately so they can be sorted
					// without changing the groups themselves.
					std::vector<int> groupOrder(groupCount);
					
					for (int g = 0; g < groupCount; ++g)
					{
						groupOrder[g] = g;
					}

					// Calculate straight-line distance between two positions.
					auto distanceToPlayer = [](sf::Vector2f a, sf::Vector2f b) {
						return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
					};

					// Process groups closest to the player first.
					// The distance of a group is determined by its closest enemy.
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

					// Stores enemies whose new positions have already
					// been accepted during this frame.
					std::vector<int> proccesedEnemies;
					proccesedEnemies.reserve(numberOfEnemies);

					for (int g : groupOrder)
					{

						// Inside each collision group, process enemies
						// closest to the player first.
						std::ranges::sort(groups[g], [&](int a, int b) {
							float da = distanceToPlayer(enemies[a]->getPosition(), playerPos);
							float db = distanceToPlayer(enemies[b]->getPosition(), playerPos);

							// Randomize the order when both enemies
							// are exactly the same distance from the player.
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

							// Save the old position before trying the movement.
							// It can be restored if the new position causes a collision.
							sf::Vector2f oldPosition = enemy->getPosition();

							enemy->movement(playerPos);

							bool collided = false;

							// Check the moved enemy against enemies
							// already processed inside the same group.
							for (int k = 0; k < i && !collided; ++k)
							{
								int j = groups[g][k];
								if (enemy->getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds()))
								{
									collided = true;
									break;
								}
							}


							// Also check against enemies from groups
							// that were processed earlier.
							for (int j : proccesedEnemies)
							{
								if (enemy->getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds()))
								{
									collided = true;
									break;
								}
							}

							// Cancel the movement if it created an overlap.
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



				// =========================
				// Projectile handling
				// =========================

				// The index is incremented manually because projectiles
				// can be erased from the vector inside the loop.
				for (int i = 0; i < projectiles.size();) {
					projectiles[i].update(deltaTime);

					bool removeProjectile = false;

					// Check whether the current projectile hit an enemy.
					for (std::size_t j = 0; j < enemies.size(); ++j) {
						
						if (projectiles[i].getGlobalBounds().findIntersection(enemies[j]->getGlobalBounds())) 
						{
							enemies[j]->takeDamage(projectiles[i].getDamage());

							// A projectile disappears after hitting an enemy.
							removeProjectile = true;

							if (enemies[j]->isDead()) {

								// Reward depends on the defeated enemy type.
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

								// Restore mana after killing an enemy.
								player.restoreMana(5);

								// Remove the defeated enemy from the cave.
								enemies.erase(enemies.begin() + j);
							}

							// One projectile can only hit one enemy.
							break;
						}
					}

					sf::Vector2f projectilePosition =
						projectiles[i].getPosition();

					// Remove projectiles that leave the playable area.
					if (projectilePosition.x < 0 || projectilePosition.x > 2880 || projectilePosition.y < 0 || projectilePosition.y > 1920) {
						removeProjectile = true;
					}

					if (removeProjectile) {

						// Do not increment i after erase().
						// The next projectile shifts into the current index.
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

		// Display the completed frame.
		window.display();
		}
	}

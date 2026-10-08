#include "Player.h"
#include "Enemy.h"
#include "gameplay/Inventory.h"
#include "gameplay/Projectile.h"


Player::Player(sf::RenderWindow& window)
	: health(100),
	mana(100),
	defence(0),
	playerSprite(playerIdleTexture) {


	// Load all textures used by the player's animations.
	if (!playerIdleTexture.loadFromFile("assets/entities/player/Warrior_idle.png")) {
		throw std::exception("Failed to load player idle texture");
	}

	if (!playerRunTexture.loadFromFile("assets/entities/player/Warrior_run.png")) {
		throw std::exception("Failed to load player run texture");
	}

	if (!playerAttackTexture.loadFromFile("assets/entities/player/Warrior_Attack1.png")) {
		throw std::exception("Failed to load player attack texture");
	}


	// Each animation frame is 192x192 pixels.
	// The sprite initially displays the first frame of the idle texture.
	playerSprite.setTexture(playerIdleTexture, true);
	playerSprite.setTextureRect({ {0, 0}, {192, 192} });

	// Put the sprite origin in its center so position and horizontal
	// flipping work relative to the middle of the player.
	playerSprite.setOrigin({ 96.f, 96.f });

	playerSprite.setScale({ 2.5f, 2.5f });


	// This circle stores the player's gameplay position
	// and is also used for collision-related calculations.
	player.setRadius(64);
	player.setFillColor(sf::Color::Green);

	player.setOrigin({
		player.getRadius(),
		player.getRadius()
		});


	// Start the player in the center of the current window.
	player.setPosition({
		static_cast<float>(window.getSize().x / 2),
		static_cast<float>(window.getSize().y / 2)
		});


	// Circular area used to detect enemies hit by melee attacks.
	attackPlayerHitbox.setRadius(256);

	// Semi-transparent red is useful for visualizing the melee range.
	attackPlayerHitbox.setFillColor(
		sf::Color(255, 0, 0, 100)
	);
}


void Player::movement() {

	// Assume the player is standing still until
	// one of the movement keys is detected.
	isMoving = false;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {

		player.move({ 0, -3 });
		isMoving = true;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {

		player.move({ 0, 3 });
		isMoving = true;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {

		player.move({ -3, 0 });
		isMoving = true;

		// Remember which direction the sprite should face.
		facingRight = false;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {

		player.move({ 3, 0 });
		isMoving = true;

		facingRight = true;
	}


	// Negative X scale mirrors the sprite horizontally
	// without requiring a separate left-facing texture.
	if (facingRight) {

		playerSprite.setScale({ 2.5f, 2.5f });
	}

	else {

		playerSprite.setScale({ -2.5f, 2.5f });
	}


	// Update idle, running or attack animation.
	updateAnimation();
}


void Player::movementBounds() {

	// Keep the player's center inside the playable 2880x1920 area.
	if (player.getPosition().x < 0) {

		player.setPosition({
			0,
			player.getPosition().y
			});
	}


	if (player.getPosition().x > 2880) {

		player.setPosition({
			2880,
			player.getPosition().y
			});
	}


	if (player.getPosition().y < 0) {

		player.setPosition({
			player.getPosition().x,
			0
			});
	}


	if (player.getPosition().y > 1920) {

		player.setPosition({
			player.getPosition().x,
			1920
			});
	}
}


void Player::dash() {

	// Dash is only allowed when Left Shift is held
	// and the player has enough mana.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)) {

		if (mana >= 20) {


			// Move a fixed distance in every currently held movement direction.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {

				player.move({ 0, -256 });
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {

				player.move({ 0, 256 });
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) {

				player.move({ -256, 0 });
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) {

				player.move({ 256, 0 });
			}


			// Every successful dash attempt costs 20 mana.
			useMana(20);
		}
	}
}


void Player::updateAnimation() {


	// Attack animation has priority over idle and running animations.
	if (isAttacking) {

		int attackFrameCount = 4;
		float attackFrameTime = 0.08f;


		// Advance to the next attack frame after the required time passes.
		if (animationClock.getElapsedTime().asSeconds() >= attackFrameTime) {

			currentFrame++;


			// After the final attack frame, return to either
			// the running or idle animation.
			if (currentFrame >= attackFrameCount) {

				isAttacking = false;
				currentFrame = 0;


				if (isMoving) {

					playerSprite.setTexture(playerRunTexture, true);
				}

				else {

					playerSprite.setTexture(playerIdleTexture, true);
				}


				playerSprite.setTextureRect(
					sf::IntRect(
						{ 0, 0 },
						{ 192, 192 }
					)
				);
			}


			else {

				// Move the texture rectangle by one 192-pixel frame.
				playerSprite.setTextureRect(
					sf::IntRect(
						{ currentFrame * 192, 0 },
						{ 192, 192 }
					)
				);
			}


			animationClock.restart();
		}


		// Do not run idle/run animation logic
		// while an attack animation is active.
		return;
	}


	// Change the texture when the player switches
	// between standing still and moving.
	if (isMoving != wasMoving) {

		if (isMoving) {

			playerSprite.setTexture(playerRunTexture, true);
		}

		else {

			playerSprite.setTexture(playerIdleTexture, true);
		}


		// Start the new animation from its first frame.
		currentFrame = 0;


		playerSprite.setTextureRect(
			sf::IntRect(
				{ 0, 0 },
				{ 192, 192 }
			)
		);


		animationClock.restart();

		// Remember the current movement state so the texture
		// is only changed when the state changes again.
		wasMoving = isMoving;
	}


	// Running animation has 6 frames and is slightly faster.
	// Idle animation has 8 frames and changes more slowly.
	int frameCount = isMoving ? 6 : 8;
	float frameTime = isMoving ? 0.10f : 0.12f;


	if (animationClock.getElapsedTime().asSeconds() >= frameTime) {


		// Move to the next frame and wrap back to frame 0
		// after reaching the end of the animation.
		currentFrame = (currentFrame + 1) % frameCount;


		playerSprite.setTextureRect(
			sf::IntRect(
				{ currentFrame * 192, 0 },
				{ 192, 192 }
			)
		);


		animationClock.restart();
	}
}


void Player::draw(sf::RenderWindow& window) {

	// Keep the visible sprite at the position
	// stored by the player's gameplay shape.
	playerSprite.setPosition(player.getPosition());

	window.draw(playerSprite);


	// Draw the melee range briefly after a melee attack.
	if (drawAttackPlayerHitbox) {

		window.draw(attackPlayerHitbox);
	}


	// Hide the attack hitbox after 350 milliseconds.
	if (attackTimer.getElapsedTime().asMilliseconds() >= 350) {

		drawAttackPlayerHitbox = false;
	}
}


sf::Vector2f Player::getPosition() const {

	return player.getPosition();
}


void Player::resetPosition(sf::RenderWindow& window) {

	// Move the player back to the center of the window.
	player.setPosition({
		static_cast<float>(window.getSize().x / 2),
		static_cast<float>(window.getSize().y / 2)
		});
}


int Player::getHealth() const {

	return health;
}


void Player::takeDamage(int damage) {

	// Negative damage would effectively heal the player,
	// so treat it as an invalid argument.
	if (damage < 0) {

		throw std::exception("Damage cannot be negative");
	}


	health -= damage;


	// Health cannot fall below zero.
	if (health < 0) {

		health = 0;
	}
}


void Player::heal(int amount) {

	if (amount < 0) {

		throw std::exception("Heal amount cannot be negative");
	}


	health += amount;


	// Maximum health is 100.
	if (health > 100) {

		health = 100;
	}
}


void Player::setHealth(int newHealth) {

	health = newHealth;
}


int Player::getMana() const {

	return mana;
}


void Player::useMana(int amount) {

	if (amount < 0) {

		throw std::exception("Mana amount cannot be negative");
	}


	mana -= amount;


	// Mana cannot fall below zero.
	if (mana < 0) {

		mana = 0;
	}
}


void Player::restoreMana(int amount) {

	if (amount < 0) {

		throw std::exception("Mana amount cannot be negative");
	}


	mana += amount;


	// Maximum mana is 100.
	if (mana > 100) {

		mana = 100;
	}
}


void Player::setMana(int newMana) {

	mana = newMana;
}


void Player::setDefence(int value) {

	defence = value;
}


int Player::getDefence() const {

	return defence;
}


void Player::attack(
	sf::Event::MouseButtonPressed const& e,
	Inventory& inventory,
	std::vector<std::unique_ptr<Enemy>>& enemies,
	std::vector<Projectile>& projectiles,
	sf::Vector2f targetPosition
) {


	// Attacks are triggered only by the left mouse button.
	if (e.button == sf::Mouse::Button::Left) {


		// Require at least 500 ms between attacks.
		if (attackTimer.getElapsedTime().asMilliseconds() >= 500) {


			// getCurrentWeapon() returns an Item pointer.
			// dynamic_cast converts it to Weapon so weapon-specific
			// methods such as isMelee() and getDamage() can be used.
			if (dynamic_cast<Weapon*>(inventory.getCurrentWeapon())->isMelee()) {


				// Start the melee attack animation from its first frame.
				isAttacking = true;
				currentFrame = 0;

				playerSprite.setTexture(
					playerAttackTexture,
					true
				);

				playerSprite.setTextureRect(
					sf::IntRect(
						{ 0, 0 },
						{ 192, 192 }
					)
				);


				animationClock.restart();


				// Center the circular melee hitbox around its own position.
				attackPlayerHitbox.setOrigin({
					attackPlayerHitbox.getRadius(),
					attackPlayerHitbox.getRadius()
					});


				// Place the melee hitbox on the player.
				attackPlayerHitbox.setPosition(
					player.getPosition()
				);


				// Make the hitbox visible temporarily.
				drawAttackPlayerHitbox = true;


				// Damage every enemy whose bounds intersect
				// the melee attack area.
				for (auto& enemy : enemies) {

					if (
						attackPlayerHitbox
						.getGlobalBounds()
						.findIntersection(
							enemy->getGlobalBounds()
						)
						) {

						enemy->takeDamage(
							dynamic_cast<Weapon*>(
								inventory.getCurrentWeapon()
								)->getDamage()
						);
					}
				}


				// Remove enemies killed by the melee attack
				// and give the player the appropriate reward.
				for (int i = 0; i < enemies.size();) {

					if (enemies[i]->isDead()) {


						switch (enemies[i]->getType()) {

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


						default:;
						}


						// Killing an enemy restores 5 mana.
						restoreMana(5);


						// Do not increment i after erase().
						// The next enemy shifts into the current index.
						enemies.erase(
							enemies.begin() + i
						);
					}


					else {

						i++;
					}
				}
			}


			else {


				// For ranged weapons, create a projectile instead
				// of directly checking enemy collision here.
				auto* weapon =
					dynamic_cast<Weapon*>(
						inventory.getCurrentWeapon()
						);


				// The projectile starts at the player's position,
				// travels toward the clicked world position
				// and stores the equipped weapon's damage.
				projectiles.emplace_back(
					player.getPosition(),
					targetPosition,
					weapon->getDamage()
				);
			}


			// Restart the timer after either a melee or ranged attack.
			attackTimer.restart();
		}
	}
}
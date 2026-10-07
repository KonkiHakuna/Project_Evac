# Project Evac

**Project Evac** is a 2D action game written in **C++** using **SFML** and **CMake**.

The goal of the game is to survive consecutive waves of enemies using different weapons and abilities. The player can explore the lobby, buy equipment, manage inventory and enter the cave to fight enemies.

The project originally started as a university assignment and was later expanded with new mechanics, improved visuals and code refactoring.

## Project Status

The game is **playable and still under development**.

The core gameplay is implemented, but I am still improving existing systems, fixing issues and adding new features.

## Features

- Melee and ranged combat
- Projectile system
- Multiple enemy types and boss fights
- Enemy wave system
- Animated player and enemies
- Inventory and equipment
- Weapons, armor, potions and spells
- Shops and player currency
- Dash ability
- Save/load system using JSON
- HUD and pause menu

## Technologies

- **C++20**
- **SFML 3**
- **CMake**
- **nlohmann/json**
- **Git / GitHub**

## Controls

| Input | Action |
|---|---|
| `W A S D` | Move |
| `Left Mouse Button` | Attack |
| `Left Shift` | Dash |
| `E` | Interact |
| `Tab` | Inventory |
| `Esc` | Pause |
| `K` | Save |
| `L` | Load |

## Project Structure

```text
src/
├── core/       # Main game logic
├── entities/   # Player and enemies
├── gameplay/   # Items, inventory and projectiles
├── save/       # Save/load system
├── ui/         # HUD, menus and shops
└── world/      # Game areas
```
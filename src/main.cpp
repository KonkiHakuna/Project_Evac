
#include "Game.h"
#include <iostream>
#include <ctime>

int main() {
	try {
		Game game;
		game.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}
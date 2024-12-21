#include "Game.h"

int main() {
	const int GLmajorVersion = 4;
	const int GLminorVersion = 4;

	Game game(
		"Coursework",
		1920, 1080,
		GLmajorVersion, GLminorVersion,
		true
	);

	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}

	return 0;
}

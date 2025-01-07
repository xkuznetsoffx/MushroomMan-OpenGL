#include "Game.h"

int main() {
	const int GLmajorVersion = 4;
	const int GLminorVersion = 4;
	bool Resizable = true;

	Game game(
		"Coursework",
		800, 600,
		GLmajorVersion, GLminorVersion,
		Resizable
	);

	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}

	return 0;
}

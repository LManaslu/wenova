#include "Game.h"
#include "MenuState.h"

int main(int, char **){
	Game game("Wenova - Rise of Conquerors", 1280, 720);

	State * state =  new MenuState();
	game.push(state);

	game.run();

	return 0;
}

#include "Game.h"
#include "MenuState.h"

int main(int, char **){
	Game game("Wenova - Rise of Conquerors", 1024, 768);

	State * state =  new MenuState();
	game.push(state);

	game.run();

	return 0;
}

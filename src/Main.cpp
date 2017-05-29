#include "Game.h"
#include "TitleState.h"

int main(int, char **){
	Game game("Wenova - Rise of Conquerors", 1280, 720);

	State * state =  new TitleState();
	game.push(state);

	game.run();

	return 0;
}

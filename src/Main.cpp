#include "Game.h"

#include "BattleState.h"

int main(int, char **){
  Game game("Wenova: Rise of Conquerors", 1280, 720);
  State * state =  new BattleState("1");
  game.push(state);
  game.run();

  return 0;
}

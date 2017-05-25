#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class BattleState : public State{
private:
  Sprite background;

public:
  BattleState(string stage);

  void update(float delta);
  void render();

  void pause();
  void resume();
};

#endif

#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Music.h"

class BattleState : public State{
private:
	Sprite background;
	Music music;

  void read_level_design(string stage);

public:
	BattleState(string stage, string cmusic);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

#ifndef STAGESELECSTATE_H
#define STAGESELECSTATE_H

#include "State.h"
#include "Sprite.h"
#include "InputManager.h"

class StageSelectState : public State {
private:
	Sprite background[2], planet, stage[2];
	int stage_select = 0;

public:
	StageSelectState();

	void update(float delta);
	void render();

	void pause();
	void resume();

	void update_stage_select(int increment);
};

#endif
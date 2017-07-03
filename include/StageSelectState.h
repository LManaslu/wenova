#ifndef STAGESELECSTATE_H
#define STAGESELECSTATE_H

#include "State.h"
#include "Sprite.h"
#include "InputManager.h"

#define N_BACKGROUNDS 2
#define N_STAGES 3
#define FRAME_TIME 7.5

class StageSelectState : public State {
private:
	Sprite background[N_BACKGROUNDS], planet, stage[N_STAGES];
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

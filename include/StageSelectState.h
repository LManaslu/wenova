#ifndef STAGESELECSTATE_H
#define STAGESELECSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Sound.h"
#include "InputManager.h"

#define N_BACKGROUNDS 2
#define FRAME_TIME 7.5

class StageSelectState : public State {
private:
	Sprite background[N_BACKGROUNDS], planet, stage[3];
	Sound blocked, selected, changed;
	int stage_select = 0;
	bool go_to_edit;
	int n_stages;

	enum Button { LEFT, RIGHT, A, B, START, SELECT };
	bool pressed[5];

	void process_input();
public:
	StageSelectState(bool cgo_to_edit = false);

	void update(float delta);
	void render();

	void pause();
	void resume();

	void update_stage_select(int increment);
};

#endif

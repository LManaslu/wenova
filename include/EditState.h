#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Fighter.h"

class EditState : public State{
private:
	Sprite background[3];
	Fighter * test_fighter;
	string stage;

	void read_level_design();
	void update_level_design();

public:
	EditState(string stage);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

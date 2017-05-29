#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class EditState : public State{
private:
	Sprite background;

public:
	EditState(string stage);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

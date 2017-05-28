#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "State.h"
#include "Sprite.h"

class MenuState : public State {
private:
	Sprite background;

public:
	MenuState();

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

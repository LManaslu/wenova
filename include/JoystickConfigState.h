#ifndef JOYSTICKCONFIGSTATE_H
#define JOYSTICKCONFIGSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class JoystickConfigState : public State{
private:
	string stage;


public:
	JoystickConfigState(int joystick_id);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

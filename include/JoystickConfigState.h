#ifndef JOYSTICKCONFIGSTATE_H
#define JOYSTICKCONFIGSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class JoystickConfigState : public State{
private:
	Sprite background, joystick_help, back_btn, test_btn, analogic;
	Text *back_txt, *test_txt, *press_both, *to_go_back;
	bool on_test;

public:
	JoystickConfigState(int joystick_id);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

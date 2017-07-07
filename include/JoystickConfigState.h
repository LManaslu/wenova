#ifndef JOYSTICKCONFIGSTATE_H
#define JOYSTICKCONFIGSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Sound.h"

class JoystickConfigState : public State{
private:
	Sprite background, joystick_help, back_btn, test_btn;
	Text *back_txt, *test_txt, *press, *select_text, *and_text, *start_text, *to_go_back;
	bool on_test;
	Sound blocked, selected, changed;

public:
	JoystickConfigState(int joystick_id);

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

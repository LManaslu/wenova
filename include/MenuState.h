#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class MenuState : public State {
private:
	Sprite background, green_ship, red_ship, title, planet;
	vector<Text*> options;
	Text* start_option;
	int current_option;
	bool start_pressed, show_text;
	Timer text_timer;

	enum Button { A, B, Y, LEFT, RIGHT, SELECT, START, LB, RT };
	bool pressed[15], is_holding[15];

public:
	MenuState(bool main_menu = false);

	void update(float delta);
	void render();

	void pause();
	void resume();

	void process_input();
};

#endif

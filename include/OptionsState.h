#ifndef OPTIONS_STATE
#define OPTIONS_STATE

#include "State.h"
#include "Text.h"
#include "Sprite.h"

#include <utility>
#include <map>

using std::pair;
using std::map;

class OptionsState : public State {
private:
	Sprite background;
	vector<Text*> options;
	map<string, vector<Text*> > sub_options;
	Text *title;
	int current_option;
	vector<int> current_sub_option;
	bool on_submenu;

	enum Button { BACK, SELECT, UP, DOWN, START, A };
	bool pressed[10];
	bool is_holding[10];
	bool released[10];

	int get_current_sub_option(int option);

public:
	OptionsState();

	void update(float delta);
	void render();

	void build_options();

	void pause();
	void resume();

	void process_input();
};

#endif

#ifndef OPTIONS_STATE
#define OPTIONS_STATE

#include "State.h"
#include "Text.h"
#include "Sprite.h"
#include "Sound.h"

// maybe
// #include "Timer.h"

#include <utility>
#include <map>

using std::pair;
using std::map;

class OptionsState : public State {
private:
	Sprite background;
	Text *title;
	Sound blocked, selected, changed;
	int current_option;
	bool on_submenu;
	map<string, vector<Text*> > sub_options;
	vector<Text*> options;
	vector<int> current_sub_option;

	int get_current_sub_option(int option);

	enum Button { A, B, UP, DOWN, SELECT, START };
	bool pressed[15];

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

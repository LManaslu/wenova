#ifndef OPTIONS_STATE
#define OPTIONS_STATE

#include "State.h"
#include "Text.h"
#include "Sprite.h"

// maybe
// #include "Timer.h"

#include <utility>
#include <map>

using std::pair;
using std::map;

class OptionsState : public State {
  private:
  	Sprite background;
	vector<Text*> options_text;
	map<string, vector<Text*> > options;
  	Text *title, *back_button;
  	int current_option_text, current_option;

  public:
  	OptionsState();

  	void update(float delta);
  	void render();

	void build_options();

  	void pause();
  	void resume();
};

#endif

#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Music.h"
#include "Sound.h"
#include "Fighter.h"
#include "Vector.h"

#include <vector>
#include <utility>

using std::vector;
using std::pair;

class EditState : public State{
private:
	vector<pair<Sprite, Vector> > backgrounds;
	Fighter * test_fighter;
	string stage;
	Music music;
	Sound sound;

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

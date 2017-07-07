#ifndef BATTLESTATE_H
#define BATTLESTATE_H

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

class BattleState : public State{
private:
	vector<pair<Sprite, Vector> > backgrounds;
	vector <Fighter *> fighters;
	Music music;
	Sound sound;
	void read_level_design(string stage);

public:
	BattleState(string stage, vector< pair<string, string> > players_info);
	~BattleState();

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

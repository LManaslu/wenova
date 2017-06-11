#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Music.h"
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
	void read_level_design(string stage);

public:
	BattleState(string stage, string cmusic);
	~BattleState();

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

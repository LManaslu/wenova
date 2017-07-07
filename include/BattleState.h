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
#include "TimeCounter.h"
#include "BattleEnd.h"

#include <vector>
#include <utility>

#define N_PLAYERS 4

using std::vector;
using std::pair;

class BattleState : public State{
private:
	vector<pair<Sprite, Vector> > backgrounds;
	Fighter* players[N_PLAYERS];
	Music music;
	Sound sound;
	void read_level_design(string stage);
	bool game_over;
	int alive[5];
	TimeCounter *time_counter;
	BattleEnd *battleEnd;

public:
	BattleState(string stage, vector< pair<string, string> > players_info);
	~BattleState();

	void update(float delta);
	void render();

	void pause();
	void resume();
};

#endif

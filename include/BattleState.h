#ifndef BATTLESTATE_H
#define BATTLESTATE_H

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "Music.h"
#include "Fighter.h"

#include <vector>

class BattleState : public State{
	private:
		Sprite background;
		vector <Fighter *> fighters;
		Music music;

	public:
		BattleState(string stage);
		~BattleState();
		
		void update(float delta);
		void render();

		void pause();
		void resume();
};

#endif

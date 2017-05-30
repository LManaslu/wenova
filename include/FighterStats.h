#ifndef FIGHTERSTATS_H
#define FIGHTERSTATS_H

#include "GameObject.h"
#include "Sprite.h"
#include "Fighter.h"

using std::string;

class FighterStats : public GameObject{
	public:
		FighterStats(string type, Fighter *p_fighter = nullptr);
		~FighterStats();

		void update(float delta);

		void render();

		bool is_dead();

		void notify_collision(GameObject &object);

		bool is(string type);

	private:
		Sprite sp;
		Sprite life; 
		string type;
		double percent_to_draw; 
		Fighter *fighter;
};

#endif

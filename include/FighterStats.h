#ifndef FIGHTERSTATS_H
#define FIGHTERSTATS_H

#include <string>

#include "GameObject.h"
#include "Sprite.h"
#include "Fighter.h"

using std::string;

class FighterStats : public GameObject{
	public:
		FighterStats(Fighter *p_fighter, int p_index_fighter, int p_side, double p_x, double p_y);
		~FighterStats();

		void update(float delta);

		void render();

		bool is_dead();

		void notify_collision(GameObject &object);

		bool is(string type);

	private:
		Sprite bg;
		Sprite empty_bg;
		Sprite life; 
		Sprite special;
		double percent_to_draw_life; 
		double percent_to_draw_special;
		int index_fighter;
		int side;
		double x, y;
		Fighter *fighter;
};

#endif

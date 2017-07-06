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

	private:
		Sprite bg[3];
		Sprite empty_bg[3];
		Sprite life[3];
		Sprite special[3];
		Sprite player_image;
		double percent_to_draw_life;
		double percent_to_draw_special;
		int index_fighter;
		int side;
		int condition;
		bool is_ultimate_diff;
		double x, y;
		Fighter *fighter;
};

#endif

#ifndef FLESH_H
#define FLESH_H

#include "Fighter.h"

class Flesh : public Fighter{
	public:
		Flesh(string skin, float x, float y, int cid, Fighter *cpartner = nullptr);

	private:
		void jump(bool change = true);
		void fall(bool change = true);
		void left(bool change = true);
		void right(bool change = true);
		void idle(bool change = true);
		void crouch(bool change = true);
		void idle_atk_neutral(bool change = true);
		void idle_atk_neutral_1(bool change = true);
		void idle_atk_neutral_2(bool change = true);
		void idle_atk_neutral_3(bool change = true);
		void idle_atk_front(bool change = true);
		void jump_atk_down_fallloop(bool change = true);
		void jump_atk_down_dmg(bool change = true);
		void idle_atk_down(bool change = true);

		void update_machine_state();
};

#endif

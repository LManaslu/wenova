#ifndef FLESH_H
#define FLESH_H

#include "Fighter.h"

class Flesh : public Fighter{
	public:
		Flesh(string skin, float x, float y, int cid, Fighter *cpartner = nullptr);

	private:
		void check_jump(bool change = true);
		void check_fall(bool change = true);
		void check_left(bool change = true);
		void check_right(bool change = true);
		void check_idle(bool change = true);
		void check_crouch(bool change = true);
		void check_idle_atk_neutral(bool change = true);
		void check_idle_atk_neutral_1(bool change = true);
		void check_idle_atk_neutral_2(bool change = true);
		void check_idle_atk_neutral_3(bool change = true);
		void check_idle_atk_front(bool change = true);
		void check_jump_atk_down_fallloop(bool change = true);
		void check_jump_atk_down_dmg(bool change = true);
		void check_idle_atk_down(bool change = true);

		void update_machine_state(float delta);
};

#endif

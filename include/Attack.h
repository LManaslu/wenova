#ifndef ATTACK_H
#define ATTACK_H

#include "Skill.h"

class Attack : public Skill{
	public:
		Attack(int p_id_player, float x, float y, float p_width, float p_height, int p_damage);
		~Attack();

		void update(float delta);

		void render();
		void notify_collision(GameObject &object);

	private:
		float width, height;
};

#endif

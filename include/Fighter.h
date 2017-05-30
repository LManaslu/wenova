#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

class Fighter : public GameObject{
	private:
		enum FighterState {IDLE, LEFT, RIGHT, JUMPING, FALLING, SLIDING};
		Sprite sprite[10];
		FighterState state;
		Vector speed;
		Vector acceleration;
		float vertical_speed;
		bool on_floor, pass_through;
		int last_collided_floor;
		float max_speed;
		int remaining_life;
		void test_limits();

	public:
		Fighter(string name, float x, float y);
		~Fighter();

		void update(float delta);
		void post_collision_update(float delta);
		void render();
		bool is_dead();

		int get_remaining_life();

		void notify_collision(GameObject & object);
		bool is(string type);

		void change_state(FighterState cstate);
		void reset_position(float x, float y);

		static const int MAX_LIFE = 100;
};

#endif

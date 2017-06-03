#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

class Fighter : public GameObject{
	private:
		enum FighterState {IDLE, RUNNING, JUMPING, FALLING, CROUCH};
		enum Orientation {LEFT, RIGHT};
		Sprite sprite[10];
		FighterState state;
		Orientation orientation;
		Vector speed;
		Vector acceleration;
		float vertical_speed;
		bool on_floor, pass_through;
		int last_collided_floor;
		float max_speed;
		int remaining_life;
		void test_limits();
		int special;
		Timer crouch_timer;

	public:
		Fighter(string name, float x, float y);
		~Fighter();

		void update(float delta);
		void post_collision_update(float delta);
		void render();
		bool is_dead();

		int get_remaining_life();
		int get_special();

		void notify_collision(GameObject & object);
		bool is(string type);

		void change_state(FighterState cstate);
		void reset_position(float x, float y);

		static const int MAX_LIFE = 500;

		static const int MAX_SPECIAL = 400;
};

#endif

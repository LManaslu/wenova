#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

class Fighter : public GameObject{
	private:
		enum FighterState {IDLE, RUNNING, JUMPING, FALLING, CROUCH, PUNCH_IDLE, PUNCH_RUN, PUNCH_JUMP, PUNCH_FALL, PUNCH_CROUCH};
		enum Button {JUMP_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, ATTACK_BUTTON, SKILL1_BUTTON, SKILL2_BUTTON, BLOCK_BUTTON};
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
		int joystick_id;
		Timer punch_duration;
		bool is_punching;

		void test_limits();

		int special;
		Timer crouch_timer;

		bool pressed[10];
		bool is_holding[10];
		bool released[10];

		void process_input();

	public:
		Fighter(string name, float x, float y, int cjoystick_id);
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

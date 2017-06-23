#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

class Fighter : public GameObject{
private:
	enum FighterState {IDLE, RUNNING, JUMPING, FALLING, CROUCH, IDLE_ATK_NEUTRAL_1, IDLE_ATK_NEUTRAL_2, IDLE_ATK_NEUTRAL_3, IDLE_ATK_FRONT, IDLE_ATK_UP};
	enum Button {JUMP_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, ATTACK_BUTTON, SKILL1_BUTTON, SKILL2_BUTTON, BLOCK_BUTTON};
	enum Orientation {LEFT, RIGHT};
	Sprite sprite[40];
	FighterState state, temporary_state;
	Orientation orientation;
	Vector speed;
	Vector acceleration;
	float vertical_speed;
	bool on_floor, pass_through;
	int last_collided_floor;
	float max_speed;
	int remaining_life;
	int joystick_id;
	int combo;

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
	void render();
	bool is_dead();

	int get_remaining_life();
	int get_special();

	void notify_collision(GameObject & object);

	void change_state(FighterState cstate);
	void reset_position(float x, float y);

	//TODO ver se é pra tá aqui msm
	void jump(bool change = true);
	void fall(bool change = true);
	void left(bool change = true);
	void right(bool change = true);
	void idle(bool change = true);
	void crouch(bool change = true);
	void idle_atk_neutral_1(bool change = true);
	void idle_atk_neutral_2(bool change = true);
	void idle_atk_neutral_3(bool change = true);
	void idle_atk_front(bool change = true);
	void idle_atk_up(bool change = true);
	void pass_through_platform(bool change = true);

	static const int MAX_LIFE = 500;

	static const int MAX_SPECIAL = 400;
};

#endif

#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

#include <vector>

using std::vector;

class Fighter : public GameObject{
protected:
	enum FighterState {IDLE, RUNNING, JUMPING, FALLING, CROUCH, IDLE_ATK_NEUTRAL_1, IDLE_ATK_NEUTRAL_2, IDLE_ATK_NEUTRAL_3, IDLE_ATK_FRONT, IDLE_ATK_UP, IDLE_ATK_DOWN, CROUCH_ATK, JUMP_ATK_UP, JUMP_ATK_DOWN, DEFENDING, STUNT, SPECIAL_1_1, SPECIAL_1_2, LAST};
	enum Button {JUMP_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, ATTACK_BUTTON, SPECIAL1_BUTTON, SPECIAL2_BUTTON, BLOCK_BUTTON};
	enum Orientation {LEFT, RIGHT};
	enum AttackDirection{ATK_DOWN = 1, ATK_LEFT = 2, ATK_UP = 4, ATK_RIGHT = 8};
	vector<Sprite> sprite;
	FighterState state, temporary_state;
	Orientation orientation;
	Vector speed;
	Vector acceleration;
	float vertical_speed;
	bool on_floor, pass_through, grab;
	int last_collided_floor;
	float max_speed;
	float remaining_life;
	int joystick_id;
	int combo;
	int n_sprite_start;
	float attack_damage;
	int attack_mask;

	void test_limits();

	float special;
	Timer crouch_timer;

	bool pressed[10];
	bool is_holding[10];
	bool released[10];

	void process_input();

	virtual void check_jump(bool change = true) = 0;
	virtual void check_fall(bool change = true) = 0;
	virtual void check_left(bool change = true) = 0;
	virtual void check_right(bool change = true) = 0;
	virtual void check_idle(bool change = true) = 0;
	virtual void check_crouch(bool change = true) = 0;
	virtual void check_idle_atk_neutral_1(bool change = true) = 0;
	virtual void check_idle_atk_neutral_2(bool change = true) = 0;
	virtual void check_idle_atk_neutral_3(bool change = true) = 0;
	virtual void check_idle_atk_front(bool change = true) = 0;
	virtual void check_idle_atk_up(bool change = true) = 0;
	virtual void check_idle_atk_down(bool change = true, bool condition = false) = 0;
	virtual void check_crouch_atk(bool change = true) = 0;
	virtual void check_jump_atk_up(bool change = true) = 0;
	virtual void check_jump_atk_down(bool change = true) = 0;
	virtual void check_pass_through_platform(bool change = true) = 0;
	virtual void check_defense(bool change = true) = 0;
	virtual void check_stunt(bool change = true) = 0;
	virtual void check_special_1_1(bool change = true) = 0;
	virtual void check_special_1_2(bool change = true) = 0;

	AttackDirection get_attack_orientation();

	virtual void update_machine_state() = 0;

public:
	Fighter(int cjoystick_id);
	~Fighter();

	void update(float delta);
	void render();
	bool is_dead();

	float get_remaining_life();
	float get_special();

	void notify_collision(GameObject & object);

	void change_state(FighterState cstate);
	void reset_position(float x, float y);

	bool is_attacking();
	float get_attack_damage();
	int get_attack_mask();

	static const int MAX_LIFE = 500;
	static const int MAX_SPECIAL = 400;
};

#endif

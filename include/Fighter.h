#ifndef FIGHTER_H
#define FIGHTER_H

#include "InputManager.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Sound.h"
#include "Vector.h"
#include "Timer.h"

#include <vector>

using std::vector;

class Fighter : public GameObject{
protected:
	enum FighterState {IDLE, RUNNING, JUMPING, FALLING, CROUCH, IDLE_ATK_NEUTRAL_1, IDLE_ATK_NEUTRAL_2, IDLE_ATK_NEUTRAL_3, IDLE_ATK_FRONT, IDLE_ATK_UP, IDLE_ATK_DOWN, CROUCH_ATK, JUMP_ATK_UP, JUMP_ATK_NEUTRAL, JUMP_ATK_DOWN, JUMP_ATK_DOWN_FALLLOOP, JUMP_ATK_DOWN_DMG, DEFENDING, STUNNED, SPECIAL_1, SPECIAL_1_1, SPECIAL_1_2, SPECIAL_2, DYING, LAST};
	enum Button {JUMP_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, ATTACK_BUTTON, SPECIAL1_BUTTON, SPECIAL2_BUTTON, BLOCK_BUTTON, ULTIMATE_BUTTON};
	enum Orientation {LEFT, RIGHT};
	enum AttackDirection{ATK_DOWN = 1, ATK_LEFT = 2, ATK_UP = 4, ATK_RIGHT = 8};
	vector<Sprite> sprite;
	vector<Sound> sound;
	Sound hit_sounds[4];
	FighterState state, temporary_state;
	Orientation orientation;
	Vector speed;
	Vector acceleration;
	Sound land_sound, ultimate_sound;
	Vector crouching_size, not_crouching_size;
	float vertical_speed;
	bool on_floor, grab;
	int last_collided_floor;
	float max_speed;
	float remaining_life;
	int id;
	int combo;
	int n_sprite_start;
	float attack_damage;
	int attack_mask;
	Fighter * partner;

	void test_limits();
	void play_sound();

	float special;
	Timer crouch_timer;
	Timer pass_through_timer;

	bool pressed[20];
	bool is_holding[20];
	bool released[20];

	bool ultimate_ready, played;

	string path, sound_path;

	void process_input();

	virtual void check_jump(bool change = true) = 0;
	virtual void check_fall(bool change = true) = 0;
	virtual void check_left(bool change = true) = 0;
	virtual void check_right(bool change = true) = 0;
	virtual void check_crouch(bool change = true) = 0;
	virtual void check_idle_atk_neutral_1(bool change = true) = 0;
	virtual void check_idle_atk_neutral_2(bool change = true) = 0;
	virtual void check_idle_atk_neutral_3(bool change = true) = 0;
	virtual void check_stunned(bool change = true) = 0;

	AttackDirection get_attack_orientation();

	virtual void update_machine_state(float delta) = 0;

public:
	Fighter(int cid, float x, Fighter * cpartner = nullptr);
	~Fighter();

	void update(float delta);
	void render();
	bool is_dead();

	float get_remaining_life();
	float get_special();


	void notify_collision(GameObject & object);
	void kill();

	void change_state(FighterState cstate);
	void reset_position(float x, float y);
	void play_hit();

	bool is_attacking();
	float get_attack_damage();
	int get_attack_mask();
	int get_id();
	int get_max_life();
	void increment_life(float increment);
	void increment_special(float increment);
	void set_partner(Fighter * cpartner);
	Fighter * get_partner();
	string get_path();

	double MAX_LIFE = 2000;
	static const int MAX_SPECIAL = 250;
};

#endif

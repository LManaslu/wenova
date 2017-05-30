#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"
#include "Timer.h"

class Fighter : public GameObject{
private:
	enum FighterState {IDLE, LEFT, RIGHT, JUMPING, FALLING, CROUCH};
	Sprite sprite[10];
	FighterState state;
	Vector speed;
	Vector acceleration;
	float vertical_speed;
	int last_collided_floor;
	bool on_floor, pass_through;
	float max_speed;
	void test_limits();
	Timer crouch_timer;

public:

	Fighter(string name, float x, float y);
	~Fighter();

	void update(float delta);
	void post_collision_update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);
	bool is(string type);

	void change_state(FighterState cstate);
	void reset_position(float x, float y);
};

#endif

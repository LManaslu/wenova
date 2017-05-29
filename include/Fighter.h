#ifndef FIGHTER_H
#define FIGHTER_H

#include "GameObject.h"
#include "Sprite.h"
#include "Vector.h"

class Fighter : public GameObject{
private:
	enum FighterState {IDLE, LEFT, RIGHT, JUMPING, FALLING, SLIDING};
	Sprite sprite[10];
	FighterState state;
	Vector speed;
	Vector acceleration;
	float vertical_speed;
	bool on_floor;
	float max_speed;
	void test_limits();

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

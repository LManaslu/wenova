#ifndef ANIMATION_H
#define ANIMATION_H

#include "GameObject.h"
#include "Timer.h"
#include "Sprite.h"

class Animation : public GameObject{
private:
	Timer end_timer;
	float time_limit;
	bool one_time_only;
	Sprite sprite;

public:
	Animation(float x, float y, float crotation, string csprite, int frame_count, float frame_time, bool ends);
	void update(float delta);
	void render();

	bool is_dead();

	void notify_collision(GameObject & object);
};

#endif

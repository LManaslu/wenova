#ifndef FLOOR_H
#define FLOOR_H

#include "GameObject.h"
#include "Sprite.h"

class Floor : public GameObject{
private:
	int id;

protected:
	bool is_platform;

public:
	Floor(float x, float y, float width, float crotation, bool cplatform);
	~Floor();

	void update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);

	int get_id();

	static int floor_id;
};

#endif

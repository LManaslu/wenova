#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Rectangle.h"
#include <iostream>

using std::string;

class GameObject{
public:
	virtual ~GameObject(){};
	virtual void update(float delta) = 0;
	virtual void post_collision_update(float){};

	virtual void render() = 0;
	virtual bool is_dead() = 0;
	virtual void notify_collision(GameObject & object) = 0;
	virtual bool is(string type) = 0;

	Rectangle box;
	float rotation = 0;
};

#endif

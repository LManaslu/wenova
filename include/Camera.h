#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "Vector.h"

#define LAYERS 4

class Camera{
private:
	static GameObject * focus;

public:
	static Vector pos[LAYERS];
	static float layer_speed[LAYERS];
	static Vector speed;

	static void follow(GameObject * new_focus);
	static void unfollow();
	static void update(float delta);
};

#endif

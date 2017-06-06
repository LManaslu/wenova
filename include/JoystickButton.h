#ifndef JOYSTICKBUTTON_H
#define JOYSTICKBUTTON_H

#include "GameObject.h"
#include "Sprite.h"

class JoystickButton : public GameObject{
private:
	bool pressed;
	Sprite sprite;

public:
	JoystickButton(int cbutton, int cjoystick_id);

	~JoystickButton();

	void update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);

};

#endif

#ifndef JOYSTICKBUTTON_H
#define JOYSTICKBUTTON_H

#include "GameObject.h"
#include "Sprite.h"

class JoystickButton : public GameObject{
private:
	bool pressed;
	Sprite sprite;
	Sprite pressed_sprite;

	int button_id;
	int joystick_id;
	string label;
	//TODO text com label escrito

public:
	JoystickButton(int x, int y, string clabel, int cbutton, int cjoystick_id, string csprite);

	~JoystickButton();

	void update(float delta);
	void render();
	bool is_dead();

	void notify_collision(GameObject & object);

};

#endif

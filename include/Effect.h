#ifndef EFFECT_H
#define EFFECT_H

#include "GameObject.h"
#include "Fighter.h"
#include "Sprite.h"

class Effect : public GameObject{
private:
	Sprite sprite;
	Fighter * parent;

public:
	Effect(Fighter * cparent, string csprite, string ctags, int frames = 1);
	Effect(string ctags);

	void update(float delta);
	void render();
	bool is_dead();
	void notify_collision(GameObject & object);
};

#endif

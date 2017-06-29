#ifndef EFFECT_H
#define EFFECT_H

#include "GameObject.h"
#include "Fighter.h"
#include "Sprite.h"

class Effect : public GameObject{
protected:
	Sprite sprite;
	Fighter * parent;

public:
	Effect(Fighter * cparent, string csprite, string ctags, int frames = 1);
	Effect(string ctags);

	virtual void update(float delta) = 0;
	virtual bool is_dead() = 0;
	virtual void render();
	void notify_collision(GameObject & object);

};

#endif

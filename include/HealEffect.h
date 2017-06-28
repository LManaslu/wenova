#ifndef HEALEFFECT_H
#define HEALEFFECT_H

#include "Effect.h"

class HealEffect : public Effect{
private:
	float healing_factor;
	
public:
	HealEffect(Fighter * cparent, string csprite, string ctags, int frames = 1, float chealing_factor = 0.0);

	void update(float delta);
    bool is_dead();
};

#endif
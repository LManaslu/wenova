#ifndef ULTIMATEEFFECT_H
#define ULTIMATEEFFECT_H

#include "Effect.h"

class UltimateEffect : public Effect{	
public:
	UltimateEffect(Fighter * cparent, string csprite, string ctags, int frames = 1);

	void update(float delta);
    bool is_dead();
};

#endif
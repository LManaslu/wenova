#ifndef FLESHULTIMATEEFFECT_H
#define FLESHULTIMATEEFFECT_H

#include "Effect.h"

class FleshUltimateEffect : public Effect{
	public:
		FleshUltimateEffect(Fighter * cparent, string csprite, string ctags, int frames = 1);

		void update(float delta);
		void render();
		bool is_dead();
};

#endif

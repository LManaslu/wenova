#ifndef FLESHULTIMATEEFFECT_H
#define FLESHULTIMATEEFFECT_H

#include "Effect.h"

class FleshUltimateEffect : public Effect{	
	private:
		Sprite aura;
		Rectangle sprite_box;

	public:
		FleshUltimateEffect(Fighter * cparent, string csprite, string caura, string ctags, int frames = 1);

		void update(float delta);
		void render();
		bool is_dead();
		void notify_collision(GameObject & object);
};

#endif

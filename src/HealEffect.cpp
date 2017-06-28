#include "HealEffect.h"

HealEffect::HealEffect(Fighter * cparent, string csprite, string ctags, int frames, float chealing_factor) : Effect(cparent, csprite, ctags, frames){
	healing_factor = chealing_factor;
}

void HealEffect::update(float delta){
	if(parent){
		box.x = parent->box.x;
		box.y = parent->box.y;
	}
	sprite.update(delta);
	if(parent){
		parent->increment_life(healing_factor);
	}
}

bool HealEffect::is_dead(){
	return sprite.is_finished();
}
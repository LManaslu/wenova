#include "UltimateEffect.h"

UltimateEffect::UltimateEffect(Fighter * cparent, string csprite, string ctags, int frames) : Effect(cparent, csprite, ctags, frames){

}

void UltimateEffect::update(float delta){
	if(parent){
		box.x = parent->box.x;
		box.y = parent->box.get_draw_y() - box.get_height()/2 - 10;
	}
	sprite.update(delta);
}

bool UltimateEffect::is_dead(){
	return parent->get_special() < Fighter::MAX_SPECIAL;
}
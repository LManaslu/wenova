#include "UltimateEffect.h"

UltimateEffect::UltimateEffect(Fighter * cparent, string csprite, string ctags, int frames) : Effect(cparent, csprite, ctags, frames){
	if(parent){
		parent->add_tags("in_ultimate");
	}
}

void UltimateEffect::update(float delta){
	if(parent){
		box.x = parent->box.x;
		box.y = parent->box.get_draw_y() - box.get_height()/2 - 10;
		parent->increment_special(-1 * delta);
	}
	sprite.update(delta);
}

bool UltimateEffect::is_dead(){
	bool dead = parent->get_special() <= 0;
	//FIXME colocar prioridade pra tirar bug de atualização
	if(dead) parent->remove_tags("in_ultimate");
	return dead;
}

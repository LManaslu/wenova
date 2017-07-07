#include "FleshUltimateEffect.h"

FleshUltimateEffect::FleshUltimateEffect(Fighter * cparent, string csprite, string ctags, int frames) : Effect(cparent, csprite, ctags, frames){
	if(parent){
		parent->add_tags("in_ultimate");
	}
	parent->MAX_LIFE *= 1.5;
}

void FleshUltimateEffect::update(float delta){
	if(parent){
		box.x = parent->box.x;
		box.y = parent->box.get_draw_y() - box.get_height()/2 - 10;
		parent->increment_special(-0.4 * delta);
	}
	sprite.update(delta);
}
void FleshUltimateEffect::render() {
	sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

bool FleshUltimateEffect::is_dead(){
	bool dead = parent->get_special() <= 0 or parent->is("dying");
	//FIXME colocar prioridade pra tirar bug de atualização
	if(dead){
		parent->remove_tags("in_ultimate");
		parent->MAX_LIFE /= 1.5;
		parent->increment_life(0);
	}
	return dead;
}

#include "UltimateEffect.h"

UltimateEffect::UltimateEffect(Fighter * cparent, string csprite, string caura, string ctags, int frames) : Effect(cparent, csprite, ctags, frames){
	aura = Sprite(caura, 14, 10, 4);
	printf("Aura: %d, %d\n", aura.get_height(), aura.get_width());
	sprite_box = box;
	box = Rectangle(0, 0, aura.get_width(), aura.get_height());
	if(parent){
		parent->add_tags("in_ultimate");
	}
}

void UltimateEffect::update(float delta){
	healing_factor = 1 * delta;
	if(parent){
		sprite_box.x = parent->box.x;
		sprite_box.y = parent->box.get_draw_y() - sprite_box.get_height()/2 - 10;
		box.x = parent->box.x;
		box.y = parent->box.y;
		parent->increment_special(-1 * delta);
		parent->increment_life(healing_factor);
	}
	sprite.update(delta);
	aura.update(delta);
}
void UltimateEffect::render() {
	sprite.render(sprite_box.get_draw_x(), sprite_box.get_draw_y(), rotation);
	aura.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

bool UltimateEffect::is_dead(){
	bool dead = parent->get_special() <= 0 or parent->is_dead();
	//FIXME colocar prioridade pra tirar bug de atualização
	if(dead) parent->remove_tags("in_ultimate");
	return dead;
}

void UltimateEffect::notify_collision(GameObject & object){
	int partner_id = (parent->get_partner() ? parent->get_partner()->get_id() : -100);
	if(!object.is("player")) return;
	Fighter & fighter = (Fighter &) object;
	if(fighter.get_id() == partner_id) {
		parent->get_partner()->increment_life(healing_factor);
	}
}

#include "Effect.h"

Effect::Effect(Fighter * cparent, string csprite, string ctags, int frames){
	parent = cparent;
	sprite = Sprite(csprite, frames, 10);
	box = Rectangle(0, 0, sprite.get_width(), sprite.get_height());
	//aplica tags
}

Effect::Effect(string ctags){
	parent = nullptr;
	//aplica tags
}

void Effect::update(float delta){
	if(parent){
		box.x = parent->box.x;
		box.y = parent->box.y;
	}
	sprite.update(delta);
	//TODO abstrair
	if(parent){
		parent->increment_life(0.2);
	}
}

void Effect::render(){
	sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

bool Effect::is_dead(){
	return sprite.is_finished();
}

void Effect::notify_collision(GameObject & object){

}

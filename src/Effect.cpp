#include "Effect.h"

Effect::Effect(Fighter * cparent, string csprite, string ctags, int frames){
	parent = cparent;
	sprite = Sprite(csprite, frames, 10);
	box = Rectangle(0, 0, sprite.get_width(), sprite.get_height());
	add_tags(ctags);
}

Effect::Effect(string ctags){
	parent = nullptr;
	add_tags(ctags);
}

void Effect::render(){
	if(is("has_sprite"))
		sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

void Effect::notify_collision(GameObject &){

}

#include "FighterStats.h"
#include "InputManager.h"

using std::to_string;

//FIXME: Trocar side pra enum
FighterStats::FighterStats(Fighter *p_fighter, int p_index_fighter, int p_side, double p_x, double p_y){
	fighter = p_fighter;
	side = p_side;
	x = p_x;
	y = p_y;
	percent_to_draw_life = 1.0;
	index_fighter = p_index_fighter;

	bg = Sprite("hud/life" + to_string(index_fighter) + "_frame.png");

	//Left
	if(side == 0){
		empty_bg = Sprite("hud/left_empty_background.png");
		life = Sprite("hud/left_life.png");
		special = Sprite("hud/left_special_bar.png");
	}

	//Right
	if(side == 1){
		empty_bg = Sprite("hud/right_empty_background.png");
		life = Sprite("hud/right_life.png");
		special = Sprite("hud/right_special_bar.png");
	}

	box = Rectangle(x, y, bg.get_width(), bg.get_height());
}

FighterStats::~FighterStats(){

}

void FighterStats::update(float){
	percent_to_draw_life = (fighter->get_remaining_life() * 1.0) / Fighter::MAX_LIFE;
	percent_to_draw_special = (fighter->get_special() * 1.0) / Fighter::MAX_SPECIAL;

	//Left
	if(side == 0){
		special.set_clip(special.get_width() * (1 - percent_to_draw_special) , 0, special.get_width() * percent_to_draw_special, special.get_height());
		life.set_clip(0, 0, life.get_width() * percent_to_draw_life, life.get_height());
	}

	//Right
	if(side == 1){
		special.set_clip(0, 0, special.get_width() * percent_to_draw_special, special.get_height());
		life.set_clip(life.get_width() * (1 - percent_to_draw_life), 0, life.get_width() * percent_to_draw_life, life.get_height());
	}
}

void FighterStats::render(){
	int offset = -3 * ((index_fighter + 1) % 2);

	//Left
	if(side == 0){
		special.render(82, box.get_draw_y());

		empty_bg.render(82, box.get_draw_y() + 22 + offset);
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 82, box.get_draw_y() + 22 + offset);
	}

	//Right
	if(side == 1){
		special.render(box.get_draw_x() - 12 + special.get_width() * (1 - percent_to_draw_special), box.get_draw_y());

		empty_bg.render(box.get_draw_x() + 8, box.get_draw_y() + 22 + offset);
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 8 + life.get_width() * (1 - percent_to_draw_life), box.get_draw_y() + 22 + offset);
	}
}

bool FighterStats::is_dead(){
	return fighter->is_dead();
}

void FighterStats::notify_collision(GameObject &){

}

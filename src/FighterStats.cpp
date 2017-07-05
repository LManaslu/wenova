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
		special[0] = Sprite("hud/left_special_bar.png");
		special[1] = Sprite("hud/full_left_special_bar.png");
		special[2] = Sprite("hud/in_ultimate_left_special_bar.png");
	}

	//Right
	if(side == 1){
		empty_bg = Sprite("hud/right_empty_background.png");
		life = Sprite("hud/right_life.png");
		special[0] = Sprite("hud/right_special_bar.png");
		special[1] = Sprite("hud/full_right_special_bar.png");
		special[2] = Sprite("hud/in_ultimate_right_special_bar.png");
	}

	player_image = Sprite(fighter->get_path() + "miniature.png");

	box = Rectangle(x, y, bg.get_width(), bg.get_height());
}

FighterStats::~FighterStats(){

}

void FighterStats::update(float){
	percent_to_draw_life = (fighter->get_remaining_life() * 1.0) / fighter->get_max_life();
	percent_to_draw_special = (fighter->get_special() * 1.0) / Fighter::MAX_SPECIAL;

	condition = (percent_to_draw_special == 1.0) ? 1 : 0;
	if(fighter->is("in_ultimate")) condition = 2;
	//Left
	if(side == 0){
		special[condition].set_clip(special[condition].get_width() * (1 - percent_to_draw_special) , 0, special[condition].get_width() * percent_to_draw_special, special[condition].get_height());
		life.set_clip(0, 0, life.get_width() * percent_to_draw_life, life.get_height());
	}

	//Right
	if(side == 1){
		special[condition].set_clip(0, 0, special[condition].get_width() * percent_to_draw_special, special[condition].get_height());
		life.set_clip(life.get_width() * (1 - percent_to_draw_life), 0, life.get_width() * percent_to_draw_life, life.get_height());
	}
}

void FighterStats::render(){
	int offset = -3 * ((index_fighter + 1) % 2);

	//Left
	if(side == 0){
		special[condition].render(82, box.get_draw_y());

		empty_bg.render(82, box.get_draw_y() + 22 + offset);
		player_image.render(box.get_draw_x() + 7, box.get_draw_y() + 9);
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 82, box.get_draw_y() + 22 + offset);
	}

	//Right
	if(side == 1){
		SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
		special[condition].render(box.get_draw_x() - 12 + special[condition].get_width() * (1 - percent_to_draw_special), box.get_draw_y());

		empty_bg.render(box.get_draw_x() + 8, box.get_draw_y() + 22 + offset);
		player_image.render(box.get_draw_x() + 197, box.get_draw_y() + 9, 0, flip);
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 8 + life.get_width() * (1 - percent_to_draw_life), box.get_draw_y() + 22 + offset);
	}
}

bool FighterStats::is_dead(){
	//FIXME ficar cinza, nao destruir
	return fighter->is_dead();
}

void FighterStats::notify_collision(GameObject &){

}

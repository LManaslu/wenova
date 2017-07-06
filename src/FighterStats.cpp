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

	is_ultimate_diff = fighter->is("flesh");

	bg[0] = Sprite("hud/life" + to_string(index_fighter) + "_frame.png");
	bg[1] = bg[2] = Sprite("hud/life" + to_string(index_fighter) + "_frame.png");
	if(is_ultimate_diff) bg[2] = Sprite("hud/life" + to_string(index_fighter) + "_frame_ultimate.png");

	//Left
	if(side == 0){
		empty_bg[0] = Sprite("hud/empty_background.png");
		empty_bg[1] = empty_bg[2] = Sprite("hud/empty_background.png");
		if(is_ultimate_diff) empty_bg[2] = Sprite("hud/empty_background_ultimate.png");
		life[0] = life[2] = Sprite("hud/left_life.png");
		life[1] = Sprite("hud/full_left_life.png");
		if(is_ultimate_diff) life[2] = Sprite("hud/in_ultimate_left_life.png");
		special[0] = Sprite("hud/left_special_bar.png");
		special[1] = Sprite("hud/full_left_special_bar.png");
		special[2] = Sprite("hud/in_ultimate_left_special_bar.png");
	}

	//Right
	if(side == 1){
		empty_bg[0] = Sprite("hud/empty_background.png");
		empty_bg[1] = empty_bg[2] = Sprite("hud/empty_background.png");
		if(is_ultimate_diff) empty_bg[2] = Sprite("hud/empty_background_ultimate.png");
		life[0] = life[2] = Sprite("hud/right_life.png");
		life[1] = Sprite("hud/full_right_life.png");
		if(is_ultimate_diff) life[2] = Sprite("hud/in_ultimate_right_life.png");
		special[0] = Sprite("hud/right_special_bar.png");
		special[1] = Sprite("hud/full_right_special_bar.png");
		special[2] = Sprite("hud/in_ultimate_right_special_bar.png");
	}

	player_image = Sprite(fighter->get_path() + "miniature.png");

	box = Rectangle(x, y, bg[0].get_width(), bg[0].get_height());
}

FighterStats::~FighterStats(){

}

void FighterStats::update(float){
	if(fighter) {
		percent_to_draw_life = (fighter->get_remaining_life() * 1.0) / Fighter::MAX_LIFE;
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
	if(fighter and fighter->is("dying")) {
		player_image = Sprite(fighter->get_path() + "dead_miniature.png");
		fighter = nullptr;
	}

}

void FighterStats::render(){
	int offset = -3 * ((index_fighter + 1) % 2);

	int y_offset = is_ultimate_diff and condition == 2 ? -15 : 0;
	int y_even_offset = is_ultimate_diff and condition == 2 and index_fighter % 2 == 0 ? 3 : 0;
	int x_right_offset = is_ultimate_diff and condition == 2 ? -88 : 0;
	int x_right_bg_offset = is_ultimate_diff and condition == 2 ? -30 : 0;
	int x_right_ebg_offset = is_ultimate_diff and condition == 2 ? -22 : 0;
	//Left
	if(side == 0){
		special[condition].render(82, box.get_draw_y());

		empty_bg[condition].render(82, box.get_draw_y() + 22 + offset + y_even_offset);
		player_image.render(box.get_draw_x() + 7, box.get_draw_y() + 9);
		life[condition].render(box.get_draw_x() + 82, box.get_draw_y() + 22 + offset + y_even_offset);
		bg[condition].render(box.get_draw_x(), box.get_draw_y() + y_offset);
	}

	//Right
	if(side == 1){
		SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
		special[condition].render(box.get_draw_x() - 12 + special[condition].get_width() * (1 - percent_to_draw_special), box.get_draw_y());

		empty_bg[condition].render(box.get_draw_x() + 8 + x_right_ebg_offset, box.get_draw_y() + 22 + offset + y_even_offset);
		player_image.render(box.get_draw_x() + 197, box.get_draw_y() + 9, 0, flip);
		life[condition].render(box.get_draw_x() + 8 + life[condition].get_width() * (1 - percent_to_draw_life) + x_right_offset, box.get_draw_y() + 22 + offset + y_even_offset);
		bg[condition].render(box.get_draw_x() + x_right_bg_offset, box.get_draw_y() + y_offset);
	}
}

bool FighterStats::is_dead(){
	//FIXME ficar cinza, nao destruir
	return false;
}

void FighterStats::notify_collision(GameObject &){

}

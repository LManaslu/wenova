#include "FighterStats.h"
#include "InputManager.h"

//Trocar side pra enum
FighterStats::FighterStats(Fighter *p_fighter, int index_fighter, int p_side, double p_x, double p_y){
	fighter = p_fighter;
	side = p_side;
	x = p_x;
	y = p_y;
	percent_to_draw_life = 1.0;

	bg = Sprite("hud/life" + std::to_string(index_fighter) + (string)"_frame.png");

	//Left
	if(side == 0){
		life = Sprite("hud/left_life.png");
		special = Sprite("hud/left_special_bar.png"); 
	}
	
	//Right
	if(side == 1){
		life = Sprite("hud/right_life.png");
		special = Sprite("hud/right_special_bar.png");
	}

	box = Rectangle(x, y, bg.get_width(), bg.get_height());
}

FighterStats::~FighterStats(){

}

void FighterStats::update(float delta){
	percent_to_draw_life = (fighter->get_remaining_life() * 1.0) / Fighter::MAX_LIFE;
	percent_to_draw_special = (fighter->get_special() * 1.0) / Fighter::MAX_SPECIAL;

	//Left
	if(side == 0){
		special.set_clip(0, 0, special.get_width() * percent_to_draw_special, special.get_height());
		life.set_clip(0, 0, life.get_width() * percent_to_draw_life, life.get_height());
	}

	//Right
	if(side == 1){
		special.set_clip(special.get_width() * (1 - percent_to_draw_special), 0, special.get_width() * percent_to_draw_special, special.get_height());
		life.set_clip(life.get_width() * (1 - percent_to_draw_life), 0, life.get_width() * percent_to_draw_life, life.get_height());
	}

	/*
	if(type == "Life1" || type == "Life2")
		life.set_clip(0, 0, life.get_width() * percent_to_draw, life.get_height());

	if(type == "Life3" || type == "Life4")
		life.set_clip(life.get_width() * (1 - percent_to_draw), 0, life.get_width() * percent_to_draw, life.get_height());

	if(type == "Special1")
		sp.set_clip(0, 0, sp.get_width() * (1-percent_to_draw), sp.get_height());
	*/
}

void FighterStats::render(){
	//Left
	if(side == 0){
		special.render(82, box.get_draw_y());
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 82, box.get_draw_y() + 23);
	}

	//Right
	if(side == 1){
		special.render(box.get_draw_x() - 12 + special.get_width() * (1 - percent_to_draw_special), box.get_draw_y());
		bg.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 8 + life.get_width() * (1 - percent_to_draw_life), box.get_draw_y() + 23);
	}

	/*
	if(type == "Life1" || type == "Life2"){
		sp.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 82, box.get_draw_y() + 23);
	}

	if(type == "Life3" || type == "Life4"){
		sp.render(box.get_draw_x(), box.get_draw_y());
		life.render(box.get_draw_x() + 8 + life.get_width() * (1 - percent_to_draw), box.get_draw_y() + 23);
	}

	if(type == "Timer")
		text->render(640, 664);

	*/
}

bool FighterStats::is_dead(){
	return false;
}

void FighterStats::notify_collision(GameObject &object){

}

bool FighterStats::is(string type){
	return false;
}

#include "FighterStats.h"
#include "InputManager.h"

FighterStats::FighterStats(string p_type, Fighter *p_fighter){
	type = p_type;
	fighter = p_fighter;

	if(type == "Timer"){
  		text = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "100", 
						{0, 0, 0, 255}, 640, 664);
		remaining_seconds = 100;
		sp = Sprite("hud/time_board.png");
		box = Rectangle(640, 664, sp.get_width(), sp.get_height());
	}else if(type == "Life1"){
		sp = Sprite("hud/life1_frame.png");
		life = Sprite("hud/left_life.png");
		box = Rectangle(133, 599.5, sp.get_width(), sp.get_height());
	}else if(type == "Life2"){
		sp = Sprite("hud/life2_frame.png");
		life = Sprite("hud/left_life.png");
		box = Rectangle(133, 679.5, sp.get_width(), sp.get_height());
	}else if(type == "Life3"){
		sp = Sprite("hud/life3_frame.png");
		life = Sprite("hud/right_life.png");
		box = Rectangle(1147, 599.5, sp.get_width(), sp.get_height());
	}else if(type == "Life4"){
		sp = Sprite("hud/life4_frame.png");
		life = Sprite("hud/right_life.png");
		box = Rectangle(1147, 679.5, sp.get_width(), sp.get_height());
	}
}

FighterStats::~FighterStats(){

}

void FighterStats::update(float delta){
	if(type == "Timer"){
		timer.update(delta);
		remaining_seconds = 100 - (timer.get() / 100); 
		printf("%f\n", timer.get());
		text->set_text(std::to_string(remaining_seconds));
	}

	if(fighter)
		percent_to_draw = (fighter->get_remaining_life() * 1.0) / Fighter::MAX_LIFE;

	if(type == "Life1" || type == "Life2")
		life.set_clip(0, 0, life.get_width() * percent_to_draw, life.get_height());

	if(type == "Life3" || type == "Life4")
		life.set_clip(life.get_width() * (1 - percent_to_draw), 0, life.get_width() * percent_to_draw, life.get_height());
}

void FighterStats::render(){
	sp.render(box.get_draw_x(), box.get_draw_y());

	if(type == "Life1" || type == "Life2")
		life.render(box.get_draw_x() + 82, box.get_draw_y() + 23);

	if(type == "Life3" || type == "Life4")
		life.render(box.get_draw_x() + 8 + life.get_width() * (1 - percent_to_draw), box.get_draw_y() + 23);

	if(type == "Timer")
		text->render(640, 664);
}

bool FighterStats::is_dead(){
	return false;
}

void FighterStats::notify_collision(GameObject &object){

}

bool FighterStats::is(string type){
	return false;
}

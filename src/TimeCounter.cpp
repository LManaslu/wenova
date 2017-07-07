#include "TimeCounter.h"

using std::to_string;

TimeCounter::TimeCounter(){
	remaining_seconds = total_time;

	text = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "99",
					{255, 255, 255, 255});

	bg = Sprite("hud/time_board.png");
	box = Rectangle(640, 664, bg.get_width(), bg.get_height());

	text->set_pos(640, 664, true, true);
}

TimeCounter::~TimeCounter(){

}

void TimeCounter::update(float delta){
	text->set_pos(640, 664, true, true);
	timer.update(delta);
	remaining_seconds -= delta * 0.01 / 3; 
	//FIXME
	if(remaining_seconds < 0)
		remaining_seconds = 0;
	text->set_text(get_time_string());
}

void TimeCounter::render(){
	bg.render(box.get_draw_x(), box.get_draw_y());
	text->render();
}

string TimeCounter::get_time_string(){
	return to_string((int)remaining_seconds);
}

bool TimeCounter::is_dead(){
	return false;
}

bool TimeCounter::is_over(){
	return remaining_seconds <= 0;
}

void TimeCounter::notify_collision(GameObject &){

}

#include "TimeCounter.h"

using std::to_string;

TimeCounter::TimeCounter(){
	remaining_seconds = total_time;

	text = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "99", 
					{255, 255, 255, 255});

	text->set_pos(640, 664, true, true);

	bg = Sprite("hud/time_board.png");
	box = Rectangle(640, 664, bg.get_width(), bg.get_height());
}

TimeCounter::~TimeCounter(){

}

void TimeCounter::update(float delta){
	timer.update(delta);
	remaining_seconds = total_time - (timer.get() / total_time); 
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
	return (remaining_seconds < 10 ? "0" : "") + to_string(remaining_seconds);
}

bool TimeCounter::is_dead(){
	return false;
}

void TimeCounter::notify_collision(GameObject &){

}

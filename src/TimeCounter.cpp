#include "TimeCounter.h"

TimeCounter::TimeCounter(){
	text = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "100", 
					{0, 0, 0, 255}, 640, 664);

	bg = Sprite("hud/time_board.png");
	box = Rectangle(640, 664, bg.get_width(), bg.get_height());

	remaining_seconds = total_time;

}

TimeCounter::~TimeCounter(){

}

void TimeCounter::update(float delta){
	timer.update(delta);
	remaining_seconds = total_time - (timer.get() / total_time); 
	printf("%f\n", timer.get());
	text->set_text(std::to_string(remaining_seconds));
}

void TimeCounter::render(){
	bg.render(box.get_draw_x(), box.get_draw_y());	
	text->render(640, 664);
}

bool TimeCounter::is_dead(){
	return false;
}

void TimeCounter::notify_collision(GameObject &object){

}

bool TimeCounter::is(string type){
	return false;
}

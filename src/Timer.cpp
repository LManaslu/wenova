#include "Timer.h"

Timer::Timer(){
	time = 0;
}

void Timer::update(float delta){
	time += delta;
}

void Timer::restart(){
	time = 0;
}

float Timer::get(){
	return time;
}

void Timer::set(float ctime){
	time = ctime;
}

#ifndef TIMER_H
#define TIMER_H

class Timer{
private:
	float time;

public:
	Timer();
	void update(float delta);
	void restart();
	float get();
	void set(float ctime);
};

#endif

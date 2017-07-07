#ifndef TIMECOUNTER_H
#define TIMECOUNTER_H

#include <string>

#include "GameObject.h"
#include "Sprite.h"
#include "Timer.h"
#include "Text.h"

using std::string;

class TimeCounter : public GameObject{
	public:
		TimeCounter();
		~TimeCounter();

		void update(float delta);

		void render();

		bool is_dead();

		bool is_over();

		void notify_collision(GameObject &object);

		static const int total_time = 100;

	private:
		Sprite bg;
		Timer timer;
		Text *text;
		float remaining_seconds;

		string get_time_string();
};

#endif

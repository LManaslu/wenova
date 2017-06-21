#ifndef SKILL_H
#define SKILL_H

#include "GameObject.h"

class Skill : public GameObject{
	public:
		virtual void update(float delta) = 0;

		virtual void render() = 0;

		bool is_dead(){
			return (elapsed_time.get() > duration);
		}

		virtual void notify_collision(GameObject &object) = 0;
	
	private:
		int id_player, damage;
		double duration;
		
		Timer elapsed_time;
};

#endif

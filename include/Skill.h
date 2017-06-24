#ifndef SKILL_H
#define SKILL_H

#include "GameObject.h"
#include "Timer.h"

using std::to_string;

class Skill : public GameObject{
	public:
		virtual ~Skill(){};

		virtual void update(float delta) = 0;

		virtual void render() = 0;

		bool is_dead(){
			return (elapsed_time.get() > duration) || hit;
		}

		virtual void notify_collision(GameObject &object) = 0;

		bool is(string type){
			return type == "skill" +  to_string(team);
		}

		int id_player, damage, team;
		bool hit;

	private:
		double duration;

		bool collidable, gravity_affected, follow_parent, has_sprite;

		Timer elapsed_time;
};

#endif

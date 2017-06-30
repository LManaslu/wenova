#include "Flesh.h"

Flesh::Flesh(string skin, float x, float y, int cid, Fighter *cpartner) : Fighter(cid, x, cpartner){

	string name = "flesh";

	sprite[IDLE] = Sprite(name + "/idle.png", 8, 10);
	sprite[RUNNING] = Sprite(name + "/running.png", 8, 10);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 10);
	sprite[FALLING] = Sprite(name + "/falling.png", 7, 10);
	sprite[CROUCH] = Sprite(name + "/crouch.png", 6, 20);
	sprite[IDLE_ATK_NEUTRAL_1] = Sprite(name + "/idle_atk_neutral.png", 12, 10);
	sprite[IDLE_ATK_NEUTRAL_2] = Sprite(name + "/idle_atk_neutral.png", 12, 10);
	sprite[IDLE_ATK_NEUTRAL_3] = Sprite(name + "/idle_atk_neutral.png", 12, 10);
	sprite[IDLE_ATK_FRONT] = Sprite(name + "/idle_atk_front.png", 4, 10);
	sprite[JUMP_ATK_DOWN_FALLLOOP] = Sprite(name + "/jump_atk_down_fallloop.png", 3, 10);
	sprite[JUMP_ATK_DOWN_DMG] = Sprite(name + "/jump_atk_down_dmg.png", 3, 10);
	sprite[IDLE_ATK_DOWN] = Sprite(name + "/idle_atk_down.png", 4, 10);

	box = Rectangle(x, y, 84, 84);
}

void Flesh::update_machine_state(){
	switch(state){
		case FighterState::IDLE_ATK_NEUTRAL_1:
			if(sprite[state].is_finished()){
				idle();
				crouch();
				idle_atk_neutral_2();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_2:
			if(sprite[state].is_finished()){
				idle();
				crouch();
				idle_atk_neutral_3();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_3:
			if(sprite[state].is_finished()){
				idle();
				crouch();
			}
		break;

		case FighterState::IDLE_ATK_FRONT:
			if(sprite[state].is_finished()){
				idle();
				crouch();
			}
		break;

		case FighterState::JUMP_ATK_DOWN_FALLLOOP:
			speed.x = 3 * (orientation == LEFT ? -1 : 1);
			speed.y = 3;
			if(sprite[state].is_finished()){
				idle();
				crouch();
				jump_atk_down_dmg();
			}
		break;

		case FighterState::JUMP_ATK_DOWN_DMG:
			if(sprite[state].is_finished()){
				idle();
				crouch();
			}
		break;

		case FighterState::IDLE_ATK_DOWN:
			if(sprite[state].is_finished()){
				idle();
				crouch();
			}
		break;

		case FighterState::IDLE:
			combo = 0;
			jump();
			left();
			right();
			idle_atk_down();
			crouch();
			fall();
			idle_atk_neutral_1();
			idle_atk_front();
		break;

		case FighterState::JUMPING:
			left(false);
			right(false);
			fall();
			jump_atk_down_fallloop();
			idle();
		break;

		case FighterState::FALLING:
			idle();
			left(false);
			right(false);
			fall();
			crouch();
			jump_atk_down_fallloop();
		break;

		case FighterState::RUNNING:
			jump();
			left(false);
			right(false);
			idle();
			crouch();
			idle_atk_neutral_1();
			idle_atk_front();
			fall();
		break;

		case FighterState::CROUCH:
			idle();
			fall();
		break;
	}
}

void Flesh::jump(bool change){
	if(pressed[JUMP_BUTTON]){
		if(change) temporary_state = FighterState::JUMPING;
		speed.y = -5;
		on_floor = false;
	}
}

void Flesh::fall(bool change){
	if(speed.y > 0){
		if(change) temporary_state = FighterState::FALLING;
	}
}

void Flesh::left(bool change){
	if(is_holding[LEFT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = -2;
		orientation = Orientation::LEFT;
	}
}

void Flesh::right(bool change){
	if(is_holding[RIGHT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = 2;
		orientation = Orientation::RIGHT;
	}
}

void Flesh::idle(bool change){
	if(speed.x == 0 and on_floor and not is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE;
	}
}

void Flesh::crouch(bool change){
	if(is_holding[DOWN_BUTTON] and not is_holding[ATTACK_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::CROUCH;
    }
}

void Flesh::idle_atk_neutral_1(bool change){
	if(pressed[ATTACK_BUTTON] and not is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_1;
	}
}

void Flesh::idle_atk_neutral_2(bool change){
	printf("Pressing: %d\n", is_holding[ATTACK_BUTTON]);
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_2;
	}
}

void Flesh::idle_atk_neutral_3(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_3;
	}
}

void Flesh::idle_atk_front(bool change){
	if(pressed[ATTACK_BUTTON] and (is_holding[LEFT_BUTTON] or is_holding[RIGHT_BUTTON])){
		if(change) temporary_state = FighterState::IDLE_ATK_FRONT;
		orientation = is_holding[LEFT_BUTTON] ? Orientation::LEFT : Orientation::RIGHT;
	}
}

void Flesh::jump_atk_down_fallloop(bool change){
	if(is_holding[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN_FALLLOOP;
	}
}

void Flesh::jump_atk_down_dmg(bool change){
	if(grab){
//	if(is_holding[ATTACK_BUTTON] and is_holding[DOWN_BUTTON] && collided_after_atk_down_fallloop){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN_DMG;
	}
}

void Flesh::idle_atk_down(bool change){
	if(is_holding[DOWN_BUTTON]) printf("to descendo\n");
	if(is_holding[ATTACK_BUTTON]) printf("to atacando\n");
	if(is_holding[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		printf("troquei, parsa\n");
		if(change) temporary_state = FighterState::IDLE_ATK_DOWN;
	}
}

#include "Fighter.h"

#include "InputManager.h"
#include "Camera.h"
#include "Floor.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#define LAYER 0

#define ii pair<int, int>

#define PI 3.14159265358979

#define CROUCH_COOLDOWN 100.0

#define PUNCH_DURATION 100.0

using std::pair;
using std::vector;

//TODO reavaliar se precisa ou não de Camera
Fighter::Fighter(string name, float x, float y, int cjoystick_id){
	printf("In construtor\n");

	sprite[IDLE] = Sprite(name + "/idle.png", 12, 10);
	sprite[RUNNING] = Sprite(name + "/running.png", 8, 10);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 10);
	sprite[FALLING] = Sprite(name + "/falling.png", 2, 10);
	sprite[CROUCH] = Sprite(name + "/crouch.png", 3, 20);
	sprite[IDLE_ATK_NEUTRAL_1] = Sprite(name + "/idle_atk_neutral_1.png", 4, 10);
	sprite[IDLE_ATK_NEUTRAL_2] = Sprite(name + "/idle_atk_neutral_2.png", 4, 10);
	sprite[IDLE_ATK_NEUTRAL_3] = Sprite(name + "/idle_atk_neutral_3.png", 3, 10);
	sprite[IDLE_ATK_FRONT] = Sprite(name + "/idle_atk_front.png", 5, 10);
	sprite[IDLE_ATK_UP] = Sprite(name + "/idle_atk_up.png", 5, 10);
	//FIXME Trocar sprites
	/*
	sprite[PUNCH_IDLE] = Sprite(name + "/punch_idle.png", 6, 40);
	sprite[PUNCH_RUN] = Sprite(name + "/punch_run.png", 6, 40);
	sprite[PUNCH_JUMP] = Sprite(name + "/punch_jump.png", 6, 40);
	sprite[PUNCH_FALL] = Sprite(name + "/punch_fall.png", 6, 40);
	sprite[PUNCH_CROUCH] = Sprite(name + "/punch_crouch.png", 6, 40);
	*/

	state = FighterState::IDLE;
	joystick_id = cjoystick_id;

	remaining_life = MAX_LIFE;
	special = 0;

	vertical_speed = rotation = 0;
	speed = Vector(0, 0);
	//FIXME recebe no construtor
	acceleration = Vector(0, 0.1);
	max_speed = 9;

	orientation = RIGHT;

	on_floor = false;
	last_collided_floor = 0;
	pass_through = false;

	box = Rectangle(x, y, sprite[RUNNING].get_width(), sprite[RUNNING].get_height());

	printf("Out construtor\n");
}

Fighter::~Fighter(){
}

void Fighter::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	vector< pair<int, int> > buttons = {
		ii(JUMP_BUTTON, SDLK_SPACE),
		ii(UP_BUTTON, SDLK_w),
		ii(DOWN_BUTTON, SDLK_s),
		ii(LEFT_BUTTON, SDLK_a),
		ii(RIGHT_BUTTON, SDLK_d),
		ii(ATTACK_BUTTON, SDLK_x),
		ii(SKILL1_BUTTON, SDLK_o),
		ii(SKILL2_BUTTON, SDLK_p),
		ii(BLOCK_BUTTON, SDLK_l)
	};

	vector< pair<int, int> > joystick_buttons = {
		ii(JUMP_BUTTON, InputManager::A),
		ii(UP_BUTTON, InputManager::UP),
		ii(DOWN_BUTTON, InputManager::DOWN),
		ii(LEFT_BUTTON, InputManager::LEFT),
		ii(RIGHT_BUTTON, InputManager::RIGHT),
		ii(ATTACK_BUTTON, InputManager::X),
		ii(SKILL1_BUTTON, InputManager::LT),
		ii(SKILL2_BUTTON, InputManager::RT),
		ii(BLOCK_BUTTON, InputManager::RB)
	};

	if(joystick_id != -1){
		for(ii button : joystick_buttons){
			pressed[button.first] = input_manager->joystick_button_press(button.second, joystick_id);
			is_holding[button.first] = input_manager->is_joystick_button_down(button.second, joystick_id);
			released[button.first] = input_manager->joystick_button_release(button.second, joystick_id);
		}
	}else{
		for(ii button : buttons){
			pressed[button.first] = input_manager->key_press(button.second, true);
			is_holding[button.first] = input_manager->is_key_down(button.second, true);
			released[button.first] = input_manager->key_release(button.second, true);
		}
	}
}

void Fighter::update(float delta){
	process_input();

	temporary_state = state;

	//FIXME
	if(pressed[JUMP_BUTTON]){
		remaining_life--;

		special++;

		if(special > MAX_SPECIAL)
			special = MAX_SPECIAL;
	}

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
		case FighterState::IDLE_ATK_UP:
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
			crouch();
			idle_atk_neutral_1();
			idle_atk_front();
			idle_atk_up();
			fall();
		break;

		case FighterState::JUMPING:
			left(false);
			right(false);
			fall();
		break;

		case FighterState::FALLING:
			idle();
			left(false);
			right(false);
			fall();
			crouch();
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

	// check pass through when double crouching
	if(pressed[DOWN_BUTTON]){
		//FIXME só checa se tiver no chão
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			pass_through = true;
		}

		crouch_timer.restart();
	}

	speed.y = std::min(speed.y + !on_floor * acceleration.y * delta, max_speed);
	box.x += speed.x * delta;
	if(not on_floor) box.y += speed.y * delta;

	test_limits();

	crouch_timer.update(delta);
	sprite[state].update(delta);
	//punch_duration.update(delta);

	change_state(temporary_state);

	speed.x = 0;
	on_floor = false;
}

void Fighter::notify_collision(GameObject & object){
	//FIXME tá feio
	float floor_y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - object.box.height * 0.5;
	if(object.is("floor") && speed.y >= 0 && not on_floor && abs(floor_y - (box.y + box.height * 0.5)) < 10){
		if(pass_through){
			if(object.is("platform")){
				if(((Floor&)object).get_id() == last_collided_floor)
					return;
				else
					pass_through = false;
			}
		}


		speed.y = 0;
		box.y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - (box.height + object.box.height ) * 0.5;

		on_floor = true;
		last_collided_floor = ((Floor&)object).get_id();
		pass_through = false;
	}
}

void Fighter::render(){
	int x = box.get_draw_x()  + 0 * Camera::pos[LAYER].x;
	int y = box.get_draw_y() + 0 * Camera::pos[LAYER].x;

	SDL_RendererFlip flip = (orientation == Orientation::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	sprite[state].render(x, y, rotation, flip);
}

bool Fighter::is_dead(){
	return false;
}

int Fighter::get_remaining_life(){
	return remaining_life;
}

int Fighter::get_special(){
	return special;
}

//only use in the end of update
void Fighter::change_state(FighterState cstate){
	if(state == cstate) return;

	printf("%d pra %d\n", state, cstate);

	sprite[state].restart_count();
	float old_width = sprite[state].get_width();
	float old_height = sprite[state].get_height();
	state = cstate;
	float new_width = sprite[state].get_width();
	float new_height = sprite[state].get_height();

	float x = box.x - (new_width - old_width) * 0.5;
	float y = box.y - (new_height - old_height) * 0.5;

	box.x = x;
	box.y = y;
	box = Rectangle(x, y, sprite[state].get_width(), sprite[state].get_height());
}

void Fighter::test_limits(){
	//TODO Matar personagem ao cair do cenario
	if(box.x < box.width / 2) box.x = box.width / 2;
	if(box.x > 1280 - box.width / 2) box.x = 1280 - box.width / 2;
	if(box.y < 0 or box.y > 720){
		box.y = 0;
		pass_through = false;
	}
}

void Fighter::reset_position(float x, float y){
	box.x = x;
	box.y = y;
	speed.y = 0;
}

void Fighter::jump(bool change){
	if(pressed[JUMP_BUTTON]){
		if(change) temporary_state = FighterState::JUMPING;
		speed.y = -5;
		on_floor = false;
	}
}

void Fighter::fall(bool change){
	if(speed.y > 0){
		if(change) temporary_state = FighterState::FALLING;
	}
}

void Fighter::left(bool change){
	if(is_holding[LEFT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = -2;
		orientation = Orientation::LEFT;
	}
}

void Fighter::right(bool change){
	if(is_holding[RIGHT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = 2;
		orientation = Orientation::RIGHT;
	}
}

void Fighter::idle(bool change){
	if(speed.x == 0 and on_floor and not is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::IDLE;
	}
}

void Fighter::crouch(bool change){
	if(is_holding[DOWN_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::CROUCH;
    }
}

void Fighter::idle_atk_neutral_1(bool change){
	if(pressed[ATTACK_BUTTON]){
		speed.y = 0;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_1;
	}
}

void Fighter::idle_atk_neutral_2(bool change){
	printf("Pressing: %d\n", is_holding[ATTACK_BUTTON]);
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_2;
	}
}

void Fighter::idle_atk_neutral_3(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_3;
	}
}

void Fighter::idle_atk_front(bool change){
	if(pressed[ATTACK_BUTTON] and (is_holding[LEFT_BUTTON] or is_holding[RIGHT_BUTTON])){
		if(change) temporary_state = FighterState::IDLE_ATK_FRONT;
		orientation = is_holding[LEFT_BUTTON] ? Orientation::LEFT : Orientation::RIGHT;
	}
}

void Fighter::idle_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(change) temporary_state = FighterState::IDLE_ATK_UP;
	}
}

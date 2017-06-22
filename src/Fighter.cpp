#include "Fighter.h"

#include "InputManager.h"
#include "Camera.h"
#include "Floor.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#define LAYER 0

#define IDLE Fighter::FighterState::IDLE
#define RUNNING Fighter::FighterState::RUNNING
#define JUMPING Fighter::FighterState::JUMPING
#define FALLING Fighter::FighterState::FALLING
#define CROUCH Fighter::FighterState::CROUCH

#define PUNCH_IDLE Fighter::FighterState::PUNCH_IDLE
#define PUNCH_RUN Fighter::FighterState::PUNCH_RUN
#define PUNCH_JUMP Fighter::FighterState::PUNCH_JUMP
#define PUNCH_FALL Fighter::FighterState::PUNCH_FALL
#define PUNCH_CROUCH Fighter::FighterState::PUNCH_CROUCH

#define FIGHTER_LEFT Fighter::Orientation::LEFT
#define FIGHTER_RIGHT Fighter::Orientation::RIGHT

#define ii pair<int, int>

#define PI 3.14159265358979

#define CROUCH_COOLDOWN 100.0

#define PUNCH_DURATION 100.0

using std::pair;
using std::vector;

//TODO reavaliar se precisa ou não de Camera
Fighter::Fighter(string name, float x, float y, int cjoystick_id){
	sprite[IDLE] = Sprite(name + "/idle.png", 8, 10);
	sprite[RUNNING] = Sprite(name + "/running.png", 8, 10);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 10);
	sprite[FALLING] = Sprite(name + "/falling.png", 7, 10);
	sprite[CROUCH] = Sprite(name + "/crouch.png", 6, 20);
	//FIXME Trocar sprites
	sprite[PUNCH_IDLE] = Sprite(name + "/punch_idle.png", 6, 40);
	sprite[PUNCH_RUN] = Sprite(name + "/punch_run.png", 6, 40);
	sprite[PUNCH_JUMP] = Sprite(name + "/punch_jump.png", 6, 40);
	sprite[PUNCH_FALL] = Sprite(name + "/punch_fall.png", 6, 40);
	sprite[PUNCH_CROUCH] = Sprite(name + "/punch_crouch.png", 6, 40);

	is_punching = false;

	state = IDLE;
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
		ii(ATTACK_BUTTON, InputManager::B),
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

	FighterState temporary_state = state;

	//FIXME
	if(pressed[JUMP_BUTTON]){
		remaining_life--;

		special++;

		if(special > MAX_SPECIAL)
			special = MAX_SPECIAL;
	}

	if(pressed[ATTACK_BUTTON] && not is_punching){
		is_punching = true;
		punch_duration.restart();
	}

	if(is_punching && punch_duration.get() < PUNCH_DURATION){
		if(state == IDLE)
			change_state(PUNCH_IDLE);

		if(state == RUNNING)
			change_state(PUNCH_RUN);

		if(state == JUMPING)
			change_state(PUNCH_JUMP);

		if(state == FALLING)
			change_state(PUNCH_FALL);

		if(state == CROUCH)
			change_state(PUNCH_CROUCH);
	}else{
		punch_duration.restart();
		is_punching = false;

		if(state != CROUCH){
			if(is_holding[LEFT_BUTTON]){
				temporary_state = RUNNING;
				speed.x = -2;
				orientation = FIGHTER_LEFT;
			}
			if(is_holding[RIGHT_BUTTON]){
				temporary_state = RUNNING;
				speed.x = 2;
				orientation = FIGHTER_RIGHT;
			}
		}

		if(is_holding[DOWN_BUTTON]){
			temporary_state = CROUCH;
		}

		if(pressed[JUMP_BUTTON] && speed.y == 0){
			speed.y = -5;
		}

		if(speed.x == 0 && speed.y == 0 && not is_holding[DOWN_BUTTON]){
			temporary_state = IDLE;
		}
	}

	// check pass through when double crouching
	if(pressed[DOWN_BUTTON]){
		//FIXME só checa se tiver no chão
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			pass_through = true;
		}

		crouch_timer.restart();
		temporary_state = CROUCH;
	}

	speed.y = std::min(speed.y + !on_floor * acceleration.y * delta, max_speed);
	box.x += speed.x * delta;
	if(not on_floor) box.y += speed.y * delta;

	test_limits();

	if(speed.y < 0 && not is_punching){
		temporary_state = JUMPING;
	}else if(speed.y > 0 && not on_floor && not is_punching){
		temporary_state = FALLING;
	}

	crouch_timer.update(delta);
	sprite[state].update(delta);
	punch_duration.update(delta);

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

	SDL_RendererFlip flip = (orientation == FIGHTER_LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
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

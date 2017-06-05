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

#define LEFT Fighter::Orientation::LEFT
#define RIGHT Fighter::Orientation::RIGHT

#define ii pair<int, int>

#define PI 3.14159265358979

#define CROUCH_COOLDOWN 100.0

using std::pair;
using std::vector;

//TODO reavaliar se precisa ou não de Camera
Fighter::Fighter(string name, float x, float y){
	sprite[IDLE] = Sprite(name + "/idle.png", 8, 10);
	sprite[RUNNING] = Sprite(name + "/running.png", 8, 10);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 10);
	sprite[FALLING] = Sprite(name + "/falling.png", 7, 10);
	sprite[CROUCH] = Sprite(name + "/crouch.png", 6, 20);

	state = IDLE;

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
	InputManager * inputManager = InputManager::get_instance();

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

	for(ii button : buttons){
		pressed[button.first] = inputManager->key_press(button.second, true);
		is_holding[button.first] = inputManager->is_key_down(button.second, true);
		released[button.first] = inputManager->key_release(button.second, true);
	}
}

void Fighter::update(float delta){
	process_input();

	//FIXME
	if(pressed[JUMP_BUTTON]){
		remaining_life--;

		special++;

		if(special > MAX_SPECIAL)
			special = MAX_SPECIAL;
	}

	speed.x = 0;
	on_floor = false;

	if(state != CROUCH){
		if(is_holding[LEFT_BUTTON]){
			if(state == IDLE) change_state(RUNNING);
			speed.x = -2;
			orientation = LEFT;
		}
		if(is_holding[RIGHT_BUTTON]){
			if(state == IDLE) change_state(RUNNING);
			speed.x = 2;
			orientation = RIGHT;
		}
	}

	if(is_holding[DOWN_BUTTON]){
		change_state(CROUCH);
	}

	if(pressed[JUMP_BUTTON] && speed.y == 0){
		speed.y = -5;
	}

	if(speed.x == 0 && speed.y == 0 && not is_holding[DOWN_BUTTON]){
		change_state(IDLE);
	}

	sprite[state].update(delta);
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

		if(state != IDLE and state != CROUCH) change_state(RUNNING);
		on_floor = true;
		last_collided_floor = ((Floor&)object).get_id();
		pass_through = false;
	}
}

void Fighter::post_collision_update(float delta){
	// check pass through when double crouching
	if(pressed[DOWN_BUTTON]){
		//FIXME só checa se tiver no chão
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			pass_through = true;
		}

		crouch_timer.restart();
		change_state(CROUCH);
	}

	speed.y = std::min(speed.y + !on_floor * acceleration.y * delta, max_speed);
	box.x += speed.x * delta;
	if(not on_floor) box.y += speed.y * delta;

	test_limits();

	if(speed.y < 0){
		change_state(JUMPING);
	}else if(speed.y > 0 && not on_floor){
		change_state(FALLING);
	}

	crouch_timer.update(delta);
}

void Fighter::render(){
	int x = box.get_draw_x()  + 0 * Camera::pos[LAYER].x;
	int y = box.get_draw_y() + 0 * Camera::pos[LAYER].x;

	SDL_RendererFlip flip = (orientation == LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
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

bool Fighter::is(string type){
	return type == "fighter";
}

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

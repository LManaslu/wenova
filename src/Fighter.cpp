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
	sprite[IDLE_ATK_DOWN] = Sprite(name + "/idle_atk_down.png", 6, 10);
	sprite[CROUCH_ATK] = Sprite(name + "/crouch_atk.png", 3, 10);
	sprite[JUMP_ATK_DOWN] = Sprite(name + "/jump_atk_down.png", 4, 10);
	sprite[JUMP_ATK_UP] = Sprite(name + "/jump_atk_up.png", 4, 10);
	sprite[DEFENDING] = Sprite(name + "/defending.png", 2, 10);
	sprite[STUNT] = Sprite(name + "/stunt.png", 2, 10);
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

	tags["player"] = true;

	remaining_life = MAX_LIFE;
	special = 0;
	attack_damage = 0;

	vertical_speed = rotation = 0;
	speed = Vector(0, 0);
	//FIXME recebe no construtor
	acceleration = Vector(0, 0.1);
	max_speed = 9;

	orientation = RIGHT;

	on_floor = false;
	last_collided_floor = 0;
	pass_through = false;

	n_sprite_start = 0;

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
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_2();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_2:
			attack_damage = 5 * (sprite[state].get_current_frame() == 1);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
				check_idle_atk_neutral_3();
			}else if(pressed[ATTACK_BUTTON]){
				combo++;
			}
		break;

		case FighterState::IDLE_ATK_FRONT: //2
			attack_damage = 10 * (sprite[state].get_current_frame() == 2);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_DOWN: //3
			attack_damage = 10 * (sprite[state].get_current_frame() == 3);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::IDLE_ATK_NEUTRAL_3: //1
		case FighterState::IDLE_ATK_UP: //1
		case FighterState::CROUCH_ATK: //1
			attack_damage = 3 * (sprite[state].get_current_frame() == 1);
			if(sprite[state].is_finished()){
				check_idle();
				check_crouch();
			}
		break;

		case FighterState::JUMP_ATK_DOWN:
			attack_damage = 3;
			if(on_floor){
				n_sprite_start = 2;
				check_idle_atk_down(true, true);
			}
		break;

		case FighterState::JUMP_ATK_UP:
			attack_damage = 7 * (sprite[state].get_current_frame() == 1);
			if(sprite[state].is_finished()){
				speed.y = 0.1;
				check_fall();
				check_idle();
				check_left(false);
				check_right(false);
			}
		break;

		case FighterState::STUNT:
			attack_damage = 0;
			if(sprite[state].is_finished()){
				check_fall();
				check_idle();
			}
		break;

		case FighterState::IDLE:
			attack_damage = 0;
			combo = 0;
			check_jump();
			check_left(on_floor);
			check_right(on_floor);
			check_crouch();
			check_defense();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_idle_atk_up();
			check_idle_atk_down();
			check_pass_through_platform();
			check_fall();
		break;

		case FighterState::JUMPING:
			attack_damage = 0;
			check_left(on_floor);
			check_right(on_floor);
			check_jump_atk_down();
			check_fall();
			check_jump_atk_up();
		break;

		case FighterState::FALLING:
			attack_damage = 0;
			check_idle();
			check_left(on_floor);
			check_right(on_floor);
			check_fall();
			check_crouch();
			check_jump_atk_up();
			check_jump_atk_down();
		break;


		case FighterState::RUNNING:
			attack_damage = 0;
			check_jump();
			check_left(false);
			check_right(false);
			check_idle();
			check_crouch();
			check_defense();
			check_idle_atk_neutral_1();
			check_idle_atk_front();
			check_pass_through_platform();
			check_fall();
		break;

		case FighterState::DEFENDING:
			attack_damage = 0;
			check_idle();
			check_fall();
		break;

		case FighterState::CROUCH:
			attack_damage = 0;
			check_idle();
			check_crouch_atk();
			check_fall();
		break;
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
	if(object.is("player")){
		Fighter & fighter = (Fighter &) object;
		if(fighter.is_attacking()){
			remaining_life -= fighter.get_attack_damage();
			special += fighter.get_attack_damage() / 3;
			check_stunt();
		}else if(is_attacking()){
			special += attack_damage / 2;
			if(special > MAX_SPECIAL) special = MAX_SPECIAL;
		}
	}

	change_state(temporary_state);
}

void Fighter::render(){
	int x = box.get_draw_x()  + 0 * Camera::pos[LAYER].x;
	int y = box.get_draw_y() + 0 * Camera::pos[LAYER].x;

	SDL_RendererFlip flip = (orientation == Orientation::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	sprite[state].render(x, y, rotation, flip);
}

bool Fighter::is_dead(){
	return remaining_life <= 0;
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

	float old_width = sprite[state].get_width();
	float old_height = sprite[state].get_height();
	state = cstate;
	float new_width = sprite[state].get_width();
	float new_height = sprite[state].get_height();
	sprite[state].restart_count(n_sprite_start);
	n_sprite_start = 0;

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
	if(box.y < 0){
		box.y = 0;
		pass_through = false;
	}

	if(box.y > 900){
		remaining_life = 0;
		//Comentar linha acima e descomentar duas abaixo para não morrer ao cair
		//box.y = 0;
		//pass_through = false;
	}
}

void Fighter::reset_position(float x, float y){
	box.x = x;
	box.y = y;
	speed.y = 0;
}

void Fighter::check_jump(bool change){
	if(pressed[JUMP_BUTTON]){
		if(change) temporary_state = FighterState::JUMPING;
		speed.y = -5;
		on_floor = false;
	}
}

void Fighter::check_fall(bool change){
	if(speed.y > 0){
		if(change) temporary_state = FighterState::FALLING;
	}
}

void Fighter::check_left(bool change){
	if(is_holding[LEFT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = -2;
		orientation = Orientation::LEFT;
	}
}

void Fighter::check_right(bool change){
	if(is_holding[RIGHT_BUTTON]){
		if(change) temporary_state = FighterState::RUNNING;
		speed.x = 2;
		orientation = Orientation::RIGHT;
	}
}

void Fighter::check_idle(bool change){
	if(speed.x == 0 and on_floor and not is_holding[DOWN_BUTTON] and not is_holding[BLOCK_BUTTON]){
		if(change) temporary_state = FighterState::IDLE;
	}
}

void Fighter::check_crouch(bool change){
	if(is_holding[DOWN_BUTTON] and on_floor and not is_holding[ATTACK_BUTTON]){
   		if(change) temporary_state = FighterState::CROUCH;
    }
}

void Fighter::check_idle_atk_neutral_1(bool change){
	if(pressed[ATTACK_BUTTON]){
		speed.y = 0;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_1;
	}
}

void Fighter::check_idle_atk_neutral_2(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_2;
	}
}

void Fighter::check_idle_atk_neutral_3(bool change){
	if(combo){
		combo--;
		if(change) temporary_state = FighterState::IDLE_ATK_NEUTRAL_3;
	}
}

void Fighter::check_idle_atk_front(bool change){
	if(pressed[ATTACK_BUTTON] and (is_holding[LEFT_BUTTON] or is_holding[RIGHT_BUTTON])){
		if(change) temporary_state = FighterState::IDLE_ATK_FRONT;
		orientation = is_holding[LEFT_BUTTON] ? Orientation::LEFT : Orientation::RIGHT;
	}
}

void Fighter::check_idle_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(change) temporary_state = FighterState::IDLE_ATK_UP;
	}
}

void Fighter::check_idle_atk_down(bool change, bool condition) {
	if(pressed[ATTACK_BUTTON] and is_holding[DOWN_BUTTON] or condition) {
		if(change) temporary_state = FighterState::IDLE_ATK_DOWN;
	}
}

void Fighter::check_pass_through_platform(bool change) {
	if(pressed[DOWN_BUTTON] and not is_holding[ATTACK_BUTTON]){
		if(crouch_timer.get() < CROUCH_COOLDOWN){
			if (change) temporary_state = FighterState::FALLING;
			pass_through = true;
		}
		crouch_timer.restart();
	}
}

void Fighter::check_crouch_atk(bool change){
	if(pressed[ATTACK_BUTTON]){
		if(change) temporary_state = FighterState::CROUCH_ATK;
	}
}

void Fighter::check_jump_atk_down(bool change){
	if(pressed[ATTACK_BUTTON] and is_holding[DOWN_BUTTON]){
		if(change) temporary_state = FighterState::JUMP_ATK_DOWN;
	}
}

void Fighter::check_jump_atk_up(bool change) {
	if(pressed[ATTACK_BUTTON] and is_holding[UP_BUTTON]) {
		if(combo) return;
		pass_through = false;
		combo++;
		speed.y = -5;
		if(change) temporary_state = FighterState::JUMP_ATK_UP;
	}
}

void Fighter::check_defense(bool change){
	if(is_holding[BLOCK_BUTTON] and on_floor){
   		if(change) temporary_state = FighterState::DEFENDING;
    }
}

void Fighter::check_stunt(bool change){
	speed.x = 0;
	if(change) temporary_state = FighterState::STUNT;
}

bool Fighter::is_attacking(){
	return attack_damage > 0;
}

int Fighter::get_attack_damage(){
	return attack_damage;
}

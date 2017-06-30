#include "Fighter.h"

#include "InputManager.h"
#include "Floor.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#define LAYER 0

#define ii pair<int, int>

#define PI 3.14159265358979
#define INVALID_ID -100
#define CROUCH_COOLDOWN 100.0

using std::min;
using std::pair;
using std::vector;

Fighter::Fighter(int cid, float x, Fighter * cpartner){
	partner = cpartner;
	state = FighterState::IDLE;
	id = cid;
	remaining_life = MAX_LIFE / 2;
	special = 0;
	attack_damage = 0;
	vertical_speed = rotation = 0;
	speed = Vector(0, 0);
	acceleration = Vector(0, 0.1);
	max_speed = 9;
	attack_mask = 0;
	sprite = vector<Sprite>(LAST);
	temporary_state = state;

	orientation = (x > 640 ? LEFT : RIGHT);

	on_floor = false;
	last_collided_floor = 0;
	grab = false;
	pass_through = false;

	ultimate_ready = false;

	n_sprite_start = 0;
	tags["player"] = true;
}

Fighter::~Fighter(){
}

void Fighter::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	vector< pair<int, int> > buttons = {
		ii(JUMP_BUTTON, InputManager::K_A),
		ii(UP_BUTTON, InputManager::K_UP),
		ii(DOWN_BUTTON, InputManager::K_DOWN),
		ii(LEFT_BUTTON, InputManager::K_LEFT),
		ii(RIGHT_BUTTON, InputManager::K_RIGHT),
		ii(ATTACK_BUTTON, InputManager::K_X),
		ii(SPECIAL1_BUTTON, InputManager::K_B),
		ii(SPECIAL2_BUTTON, InputManager::K_RB),
		ii(BLOCK_BUTTON, InputManager::K_LB),
		ii(ULTIMATE_BUTTON, InputManager::K_Y)
	};

	vector< pair<int, int> > joystick_buttons = {
		ii(JUMP_BUTTON, InputManager::A),
		ii(UP_BUTTON, InputManager::UP),
		ii(DOWN_BUTTON, InputManager::DOWN),
		ii(LEFT_BUTTON, InputManager::LEFT),
		ii(RIGHT_BUTTON, InputManager::RIGHT),
		ii(ATTACK_BUTTON, InputManager::X),
		ii(SPECIAL1_BUTTON, InputManager::B),
		ii(SPECIAL2_BUTTON, InputManager::RB),
		ii(BLOCK_BUTTON, InputManager::LB),
		ii(ULTIMATE_BUTTON, InputManager::Y)
	};

	bool alive = !is("dying");

	if(id != -1){
		for(ii button : joystick_buttons){
			pressed[button.first] = alive and input_manager->joystick_button_press(button.second, id);
			is_holding[button.first] = alive and input_manager->is_joystick_button_down(button.second, id);
			released[button.first] = alive and input_manager->joystick_button_release(button.second, id);
		}
	}else{
		for(ii button : buttons){
			pressed[button.first] = alive and input_manager->key_press(button.second, true);
			is_holding[button.first] = alive and input_manager->is_key_down(button.second, true);
			released[button.first] = alive and input_manager->key_release(button.second, true);
		}
	}
}

void Fighter::update(float delta){
	process_input();

	temporary_state = state;

	sprite[state].update(delta);

	update_machine_state(delta);

	speed.y = std::min(speed.y + !on_floor * acceleration.y * delta, max_speed);
	box.x += speed.x * delta;
	if(not on_floor) box.y += speed.y * delta;

	test_limits();

	crouch_timer.update(delta);

	change_state(temporary_state);

	speed.x = 0;
	grab = false;
	attack_damage *= delta;
	on_floor = false;
}

void Fighter::notify_collision(GameObject & object){
	int partner_id = (partner ? partner->get_id() : INVALID_ID);
	int not_in_ultimate = (tags["in_ultimate"] ? 0 : 1);
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
	}else if(object.is("player") and !is("dying") and !(object.is("dying"))){
		Fighter & fighter = (Fighter &) object;

		if(fighter.is_attacking() and fighter.get_id() != partner_id){
			printf("In fighter stunt\n");
			int left = AttackDirection::ATK_LEFT * (fighter.box.x > box.x);
			int right = AttackDirection::ATK_RIGHT * (fighter.box.x <= box.x);
			int up = AttackDirection::ATK_UP * (fighter.box.y > box.y);
			int down = AttackDirection::ATK_DOWN * (fighter.box.y <= box.y);
			int position_mask = left | right | up | down;
			if(position_mask & fighter.get_attack_mask()){
				float damage = fighter.get_attack_damage() * ((state == FighterState::DEFENDING) ? 0.5 : 1);
				increment_life(-damage);
				float increment_special = (fighter.get_attack_damage() / 3) * ((state == FighterState::DEFENDING) ? 0 : 1) * not_in_ultimate;
				this->increment_special(increment_special);
				if(state != FighterState::DEFENDING) check_stunt();
			}
		}else if(is_attacking() and fighter.get_id() != partner_id){
			int left = AttackDirection::ATK_LEFT * (fighter.box.x <= box.x);
			int right = AttackDirection::ATK_RIGHT * (fighter.box.x > box.x);
			int up = AttackDirection::ATK_UP * (fighter.box.y <= box.y);
			int down = AttackDirection::ATK_DOWN * (fighter.box.y > box.y);
			int position_mask = left | right | up | down;
			if(position_mask & get_attack_mask()){
				grab = true;
				this->increment_special((attack_damage / 2) * not_in_ultimate);
			}
		}
	}

	change_state(temporary_state);
}

void Fighter::render(){
	int x = box.get_draw_x() + (orientation == RIGHT ? 0 : -1) * (sprite[state].get_width() - 84);
	int y = box.get_draw_y();

	SDL_RendererFlip flip = (orientation == Orientation::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	sprite[state].render(x, y, rotation, flip);
}

bool Fighter::is_dead(){
	bool dead = remaining_life <= 0;
	if(dead and partner) partner->set_partner(nullptr);
	return remaining_life <= 0;
}

float Fighter::get_remaining_life(){
	return remaining_life;
}

float Fighter::get_special(){
	return special;
}

//only use in the end of update
void Fighter::change_state(FighterState cstate){
	if(state == cstate) return;

	float old_height = box.height;
	state = cstate;
	Vector csize;
	if(cstate == CROUCH or cstate == CROUCH_ATK) csize = crouching_size;
	else csize = not_crouching_size;
	float new_height = csize.y;

	sprite[state].restart_count(n_sprite_start);
	n_sprite_start = 0;

	float y = box.y - (new_height - old_height) * 0.5;

	box = Rectangle(box.x, y, csize.x, csize.y);
}

void Fighter::test_limits(){
	if(box.x < box.width / 2) box.x = box.width / 2;
	if(box.x > 1280 - box.width / 2) box.x = 1280 - box.width / 2;
	if(box.y < -100){
		box.y = -100;
		pass_through = false;
	}

	if(box.y > 900){
		pass_through = false;
		if(is("test")) box.y = -100;
		else remaining_life = 0;
		//Comentar linha acima e descomentar abaixo para não morrer ao cair
		//box.y = 0;
	}
}

Fighter::AttackDirection Fighter::get_attack_orientation(){
	return (orientation == Orientation::LEFT ? AttackDirection::ATK_LEFT : AttackDirection::ATK_RIGHT);
}

void Fighter::reset_position(float x, float y){
	box.x = x;
	box.y = y;
	speed.y = 0;
}

bool Fighter::is_attacking(){
	return attack_damage > 0;
}

float Fighter::get_attack_damage(){
	return attack_damage;
}

int Fighter::get_attack_mask(){
	return attack_mask;
}

int Fighter::get_id(){
	return id;
}

void Fighter::increment_life(float increment){
	remaining_life += increment;
	if(remaining_life < 1) {
		remaining_life = 1;
		if(partner) partner->set_partner(nullptr);
		add_tags("dying");
	}
	if(remaining_life > MAX_LIFE) remaining_life = MAX_LIFE;
}

void Fighter::increment_special(float increment){
	special += increment;
	if(special < 0) special = 0;
	if(special > MAX_SPECIAL) special = MAX_SPECIAL;
}

void Fighter::set_partner(Fighter * cpartner){
	partner = cpartner;
}

Fighter * Fighter::get_partner() {
	return partner;
}

#include "Fighter.h"

#include "InputManager.h"
#include "Camera.h"

#include <algorithm>
#include <cstdlib>

#define LAYER 0

#define IDLE Fighter::FighterState::IDLE
#define LEFT Fighter::FighterState::LEFT
#define RIGHT Fighter::FighterState::RIGHT
#define JUMPING Fighter::FighterState::JUMPING
#define FALLING Fighter::FighterState::FALLING
#define SLIDING Fighter::FighterState::SLIDING

#define PI 3.14159265358979


//TODO reavaliar se precisa ou não de Camera
Fighter::Fighter(string name, float x, float y){
	sprite[IDLE] = Sprite(name + "/idle.png", 6, 1);
	sprite[LEFT] = Sprite(name + "/left.png", 6, 1);
	sprite[RIGHT] = Sprite(name + "/right.png", 8, 1);
	sprite[JUMPING] = Sprite(name + "/jumping.png", 6, 1);
	sprite[FALLING] = Sprite(name + "/falling.png", 6, 1);
	sprite[SLIDING] = Sprite(name + "/sliding.png", 6, 1);

	state = IDLE;

	vertical_speed = rotation = 0;
	speed = Vector(0, 0);
	acceleration = Vector(0, 0.2);
	max_speed = 5;
	//FIXME recebe no construtor

	on_floor = false;

	box = Rectangle(x, y, sprite[state].get_width(), sprite[state].get_height());
}

Fighter::~Fighter(){
}

void Fighter::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	speed.x = 0;
	on_floor = false;
	if(inputManager.is_key_down(SDLK_a)){
		change_state(LEFT);
		speed.x = -1;
	}
	if(inputManager.is_key_down(SDLK_d)){
		change_state(RIGHT);
		speed.x = 1;
	}
	if(inputManager.is_key_down(SDLK_s)){
		change_state(SLIDING);
	}
	if(inputManager.is_key_down(SDLK_SPACE) && speed.y == 0){
		speed.y = -5;
	}

	if(speed.x == 0 && speed.y == 0 && not inputManager.is_key_down(SDLK_s)){
		change_state(IDLE);
	}

	sprite[state].update(delta);
}

void Fighter::notify_collision(GameObject & object){
	//FIXME tá feio

	float floor_y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - object.box.height * 0.5;
	if(object.is("floor") && speed.y >= 0 && not on_floor && abs(floor_y - (box.y + box.height * 0.5)) < 30){
		speed.y = 0;
		box.y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - (box.height + object.box.height ) * 0.5;

		on_floor = true;
	}
}

void Fighter::post_collision_update(float delta){
	speed.y += std::min(!on_floor * acceleration.y * delta, max_speed);
	box.x += speed.x * delta;
	if(not on_floor) box.y += speed.y * delta;

	test_limits();

	if(speed.y < 0){
		change_state(JUMPING);
	}else if(speed.y > 0 && not on_floor){
		change_state(FALLING);
	}
}

void Fighter::render(){
	int x = box.get_draw_x()  + 0 * Camera::pos[LAYER].x;
	int y = box.get_draw_y() + 0 * Camera::pos[LAYER].x;
	sprite[state].render(x, y, rotation);
}

bool Fighter::is_dead(){
	return false;
}

void Fighter::notify_collision(GameObject & object){
	//FIXME tá feio

	float floor_y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - object.box.height * 0.5;
	if(object.is("floor") && speed.y >= 0 && not on_floor && abs(floor_y - (box.y + box.height * 0.5)) < 20){
		speed.y = 0;
		box.y = object.box.y + (box.x - object.box.x) * tan(object.rotation) - (box.height + object.box.height ) * 0.5;

		//printf("%f %f\n", object.rotation, object.rotation * 180 / PI);
		on_floor = true;
	}
}

bool Fighter::is(string type){
	return type == "fighter";
}

void Fighter::change_state(FighterState cstate){
	if(state == cstate) return;

	//TODO Dar restart no count da sprite
	float old_width = sprite[state].get_width();
	float old_height = sprite[state].get_height();
	state = cstate;
	float new_width = sprite[state].get_width();
	float new_height = sprite[state].get_height();

	float x = box.x - (new_width - old_width) * 0.5;
	float y = box.y - (new_height - old_height) * 0.5;

	box = Rectangle(x, y, sprite[state].get_width(), sprite[state].get_height());
}

void Fighter::test_limits(){
	//TODO config
	if(box.x < 0) box.x = 0;
	if(box.x > 1280) box.x = 1280;
	if(box.y < 0) box.y = 0;
	if(box.y > 720) box.y = 720;
}

void Fighter::reset_position(float x, float y){
	box.x = x;
	box.y = y;
	speed.y = 0;
}

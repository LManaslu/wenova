#include "EditableFloor.h"

#include "InputManager.h"
#include "Collision.h"
#include "Rectangle.h"

#include <cstdio>

#define LAYER 0
#define PI 3.14159265358979

EditableFloor::EditableFloor(float x, float y, float crotation, bool cplatform) : Floor(x, y, 100, crotation, cplatform), normal_sprite(Sprite("edit_state/floor/editable_floor.png")), platform_sprite(Sprite("edit_state/floor/editable_platform.png")), selected_sprite(Sprite("edit_state/floor/selected_editable_floor.png")) {
	box = Rectangle(x, y, normal_sprite.get_width(), normal_sprite.get_height());
	deleted = false;
	selected = false;
}

EditableFloor::EditableFloor(float x, float y, float width, float crotation, bool cplatform) : EditableFloor(x, y, crotation, cplatform){
	normal_sprite.set_scale_x(width / normal_sprite.get_width());
	platform_sprite.set_scale_x(width / platform_sprite.get_width());
	selected_sprite.set_scale_x(width / selected_sprite.get_width());
	box.width = normal_sprite.get_width();
}

EditableFloor::~EditableFloor(){

}

void EditableFloor::update(float delta){
	InputManager * inputManager = InputManager::get_instance();

	if(inputManager->mouse_press(InputManager::LEFT_MOUSE_BUTTON)){
		int x = inputManager->get_mouse_x();
      	int y = inputManager->get_mouse_y();
		Rectangle mouse = Rectangle(x, y, 1, 1);
		if(Collision::is_colliding(box, mouse, rotation, 0)){
			selected = true;
		}else{
			selected = false;
		}
	}


	if(selected){
		static float acceleration = 1;
		float value = 0.5 * delta * acceleration;
		bool moved = false;
		if(inputManager->is_key_down(SDLK_RIGHT)){
			box.x += value; moved = true;
		}
		if(inputManager->is_key_down(SDLK_LEFT)){
			box.x -= value; moved = true;
		}
		if(inputManager->is_key_down(SDLK_UP)){
			box.y -= value; moved = true;
		}
		if(inputManager->is_key_down(SDLK_DOWN)){
			box.y += value; moved = true;
		}
		if(box.x < 0) box.x = 0;
		if(box.x > 1280) box.x = 1280;
		if(box.y < 0) box.y = 0;
		if(box.y > 720) box.y = 720;

		if(inputManager->is_key_down(SDLK_z)){
			rotation += 0.01 * value / acceleration;
		}
		if(inputManager->is_key_down(SDLK_x)){
			rotation -= 0.01 * value / acceleration;
		}
		if(inputManager->is_key_down(SDLK_r)){
			rotation = 0;
		}
		if(inputManager->key_press(SDLK_k)){
			is_platform = !is_platform;
		}

		if(inputManager->is_key_down(SDLK_PERIOD)){
			normal_sprite.update_scale_x(0.005 * value);
			platform_sprite.update_scale_x(0.005 * value);
			selected_sprite.update_scale_x(0.005 * value);
			box.width = normal_sprite.get_width();
			moved = true;
		}
		if(inputManager->is_key_down(SDLK_COMMA)){
			normal_sprite.update_scale_x(-0.005 * value);
			platform_sprite.update_scale_x(-0.005 * value);
			selected_sprite.update_scale_x(-0.005 * value);
			box.width = normal_sprite.get_width();
			moved = true;
		}
		if(moved) acceleration += 0.2;
		else	acceleration = 1;
		if(acceleration > 4) acceleration = 4;

		if(inputManager->is_key_down(SDLK_DELETE)){
			deleted = true;
		}
	}

	//printf("%f, %f, %.f, %f, %f %d\n", box.x, box.y, box.width, box.height, rotation * 180.0 / PI, (int) is_platform);
}

void EditableFloor::render(){
	if(selected){
		selected_sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
	}

	if(is_platform)
		platform_sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
	else
		normal_sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
}

bool EditableFloor::is_dead(){
	return deleted;
}

void EditableFloor::notify_collision(GameObject &){
}

string EditableFloor::get_information(){
	char info[500];
	sprintf(info, "%f %f %f %f %d", box.x, box.y, box.width, rotation * 180 / PI, (int) is_platform);
	string s(info);
	for(auto & c : s) c += 15;

	return s;
}

void EditableFloor::set_selected(bool cselected){
	selected = cselected;
}

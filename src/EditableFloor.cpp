#include "EditableFloor.h"

#include "InputManager.h"
#include "Collision.h"
#include "Rectangle.h"

#include <cstdio>

#define LAYER 0
#define PI 3.14159265358979

EditableFloor::EditableFloor(float x, float y, float crotation, bool cplatform) : Floor(x, y, 100, crotation, cplatform), normal_sprite(Sprite("images/editable_floor.png")), selected_sprite(Sprite("images/selected_editable_floor.png")){
	box = Rectangle(x, y, normal_sprite.get_width(), normal_sprite.get_height());
	deleted = false;
	selected = false;
}

EditableFloor::EditableFloor(float x, float y, float width, float crotation, bool cplatform) : EditableFloor(x, y, crotation, cplatform){
	normal_sprite.set_scale_x(width / normal_sprite.get_width());
	selected_sprite.set_scale_x(width / selected_sprite.get_width());
	box.width = normal_sprite.get_width();
}

EditableFloor::~EditableFloor(){

}

void EditableFloor::update(float delta){
	InputManager inputManager = InputManager::get_instance();


	if(inputManager.mouse_press(InputManager::LEFT_MOUSE_BUTTON)){
		int x = inputManager.get_mouse_x();
      	int y = inputManager.get_mouse_y();
		Rectangle mouse = Rectangle(x, y, 1, 1);
		if(Collision::is_colliding(box, mouse, rotation, 0)){
			selected = true;
		}else{
			selected = false;
		}
	}


	if(selected){
		float value = 0.5 * delta;
		if(inputManager.is_key_down(SDLK_RIGHT)){
			box.x += value;
		}
		if(inputManager.is_key_down(SDLK_LEFT)){
			box.x -= value;
		}
		if(inputManager.is_key_down(SDLK_UP)){
			box.y -= value;
		}
		if(inputManager.is_key_down(SDLK_DOWN)){
			box.y += value;
		}

		if(inputManager.is_key_down(SDLK_z)){
			rotation += 0.01 * value;
		}
		if(inputManager.is_key_down(SDLK_x)){
			rotation -= 0.01 * value;
		}
		if(inputManager.is_key_down(SDLK_r)){
			rotation = 0;
		}

		if(inputManager.is_key_down(SDLK_PERIOD)){
			normal_sprite.update_scale_x(0.005 * value);
			selected_sprite.update_scale_x(0.005 * value);
			box.width = normal_sprite.get_width();
		}
		if(inputManager.is_key_down(SDLK_COMMA)){
			normal_sprite.update_scale_x(-0.005 * value);
			selected_sprite.update_scale_x(-0.005 * value);
			box.width = normal_sprite.get_width();
		}
		if(inputManager.is_key_down(SDLK_DELETE)){
			deleted = true;
		}
	}

	printf("%f, %f, %.f, %f, %f %d\n", box.x, box.y, box.width, box.height, rotation * 180.0 / PI, (int) is_platform);
}

void EditableFloor::render(){
	if(selected){
		selected_sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
	}else{
		normal_sprite.render(box.get_draw_x(), box.get_draw_y(), rotation);
	}
}

bool EditableFloor::is_dead(){
	return deleted;
}

void EditableFloor::notify_collision(GameObject &){
}

bool EditableFloor::is(string type){
	return type == "floor";
}

string EditableFloor::get_information(){
	char info[500];
	sprintf(info, "%f %f %f %f %d", box.x, box.y, box.width, rotation * 180 / PI, (int) is_platform);
	string s(info);
	for(auto & c : s) c += 10;

	return s;
}

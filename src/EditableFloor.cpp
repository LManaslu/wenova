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
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->mouse_press(InputManager::LEFT_MOUSE_BUTTON)){
		int x = input_manager->get_mouse_x();
		int y = input_manager->get_mouse_y();
		Rectangle mouse = Rectangle(x, y, 1, 1);
		selected = Collision::is_colliding(box, mouse, rotation, 0);
	}

	if(selected){
		static float acceleration = 1;
		float value = 0.5 * delta * acceleration;
		bool moved = false;

		// move floor
		if(input_manager->is_key_down(InputManager::K_ARROW_RIGHT)){
			box.x += value; moved = true;
		}

		if(input_manager->is_key_down(InputManager::K_ARROW_LEFT)){
			box.x -= value; moved = true;
		}

		if(input_manager->is_key_down(InputManager::K_ARROW_UP)){
			box.y -= value; moved = true;
		}

		if(input_manager->is_key_down(InputManager::K_ARROW_DOWN)){
			box.y += value; moved = true;
		}

		if(box.x < 0) box.x = 0;
		if(box.x > 1280) box.x = 1280;
		if(box.y < 0) box.y = 0;
		if(box.y > 720) box.y = 720;

		// rotate floor to left
		if(input_manager->is_key_down(InputManager::K_ROT_LEFT)){
			rotation += 0.01 * value / acceleration;
		}

		// rotate floor to right
		if(input_manager->is_key_down(InputManager::K_ROT_RIGHT)){
			rotation -= 0.01 * value / acceleration;
		}

		// reset rotation
		if(input_manager->is_key_down(InputManager::K_ROT_RESET)){
			rotation = 0;
		}

		// toggle floor
		if(input_manager->key_press(InputManager::K_C)){
			is_platform = !is_platform;
			tags["platform"] = !tags["platform"];
		}

		// increase floor width
		if(input_manager->is_key_down(InputManager::K_INC_W)){
			normal_sprite.update_scale_x(0.005 * value);
			platform_sprite.update_scale_x(0.005 * value);
			selected_sprite.update_scale_x(0.005 * value);
			box.width = normal_sprite.get_width();
			moved = true;
		}

		// decrease floor width
		if(input_manager->is_key_down(InputManager::K_DEC_W)){
			normal_sprite.update_scale_x(-0.005 * value);
			platform_sprite.update_scale_x(-0.005 * value);
			selected_sprite.update_scale_x(-0.005 * value);
			box.width = normal_sprite.get_width();
			moved = true;
		}

		if(moved)
			acceleration = fmin(acceleration + 0.2, 4);
		else
			acceleration = 1;

		// delete floor
		if(input_manager->is_key_down(InputManager::K_DEL)){
			deleted = true;
		}
	}
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

#include "EditState.h"

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "EditableFloor.h"

#define WIDTH 1280
#define HEIGHT 720

EditState::EditState(string stage){
	background = Sprite("stage_" + stage + "/background.png", 6, 1);

	test_fighter = new Fighter("fighter", WIDTH/2, HEIGHT/2 - 200);
	add_object(test_fighter);
	add_object(new EditableFloor(WIDTH/2, HEIGHT/2, 0));

	//TODO ler os tiles que já tem e colocar
}

void EditState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(inputManager.mouse_press(InputManager::RIGHT_MOUSE_BUTTON)){
		int x = inputManager.get_mouse_x();
		int y = inputManager.get_mouse_y();
		test_fighter->reset_position(x, y);
	}

	if(inputManager.key_press(SDLK_n)){
		int x = inputManager.get_mouse_x();
      	int y = inputManager.get_mouse_y();
		add_object(new EditableFloor(x, y, 0));
	}

	update_array(delta);
}

void EditState::render(){
	background.render(0, 0);

	render_array();
}

void EditState::pause(){

}

void EditState::resume(){

}

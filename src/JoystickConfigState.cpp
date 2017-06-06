#include "JoystickConfigState.h"

#include "InputManager.h"
#include "MenuState.h"
#include "JoystickButton.h"
#include "Game.h"

#define A_X 500
#define A_Y 500

JoystickConfigState::JoystickConfigState(int joystick_id){
	add_object(new JoystickButton(A_X, A_Y, "A", 5, joystick_id, "button"));
	add_object(new JoystickButton(A_X + 100, A_Y - 100, "B", 6, joystick_id, "button"));
	add_object(new JoystickButton(A_X, A_Y - 200, "Y", 7, joystick_id, "button"));
	add_object(new JoystickButton(A_X - 100, A_Y - 100, "X", 4, joystick_id, "button"));
}

void JoystickConfigState::update(float delta){
	InputManager * inputManager = InputManager::get_instance();

	if(inputManager->key_press(SDLK_ESCAPE)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(inputManager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	update_array(delta);
}

void JoystickConfigState::render(){
	render_array();
}

void JoystickConfigState::pause(){

}

void JoystickConfigState::resume(){

}

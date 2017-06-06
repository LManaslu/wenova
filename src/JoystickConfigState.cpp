#include "JoystickConfigState.h"

#include "InputManager.h"
#include "MenuState.h"
#include "JoystickButton.h"
#include "Game.h"

JoystickConfigState::JoystickConfigState(int joystick_id){
	add_object(new JoystickButton(100, 100, "A", 5, joystick_id, "button"));
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

#include "JoystickConfigState.h"

#include "InputManager.h"
#include "MenuState.h"
#include "JoystickButton.h"
#include "Game.h"

#define A_X 800
#define A_Y 500
#define LB_X A_X - 500
#define LB_Y A_Y - 300
#define SELECT_X A_X - 300
#define SELECT_Y A_Y - 200

JoystickConfigState::JoystickConfigState(int joystick_id){
	//ABXY
	add_object(new JoystickButton(A_X, A_Y, "A", 5, joystick_id, "button"));
	add_object(new JoystickButton(A_X + 100, A_Y - 100, "B", 6, joystick_id, "button"));
	add_object(new JoystickButton(A_X, A_Y - 200, "Y", 7, joystick_id, "button"));
	add_object(new JoystickButton(A_X - 100, A_Y - 100, "X", 4, joystick_id, "button"));

	//Triggers
	add_object(new JoystickButton(LB_X, LB_Y, "LB", 8, joystick_id, "trigger"));
	add_object(new JoystickButton(LB_X + 500, LB_Y, "RB", 9, joystick_id, "trigger"));
	add_object(new JoystickButton(LB_X, LB_Y - 75, "LT", 10, joystick_id, "back_trigger"));
	add_object(new JoystickButton(LB_X + 500, LB_Y - 75, "RT", 11, joystick_id, "back_trigger"));

	//Select e start
	add_object(new JoystickButton(SELECT_X, SELECT_Y, "select", 12, joystick_id, "select_start"));
	add_object(new JoystickButton(SELECT_X + 150, SELECT_Y, "start", 13, joystick_id, "select_start"));


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

#include "JoystickConfigState.h"

#include "InputManager.h"
#include "MenuState.h"
#include "JoystickButton.h"
#include "Game.h"

#define MIDDLE_X 550
#define BOTTOM_Y 500
#define A_X MIDDLE_X + 250
#define A_Y BOTTOM_Y
#define LB_X MIDDLE_X - 250
#define LB_Y BOTTOM_Y - 300
#define DOWN_X LB_X
#define DOWN_Y BOTTOM_Y - 25
#define SELECT_X MIDDLE_X - 75
#define SELECT_Y BOTTOM_Y - 200

JoystickConfigState::JoystickConfigState(int joystick_id){
	//D-Pad
	add_object(new JoystickButton(DOWN_X, DOWN_Y - 100, "UP", InputManager::JoystickButton::UP, joystick_id, "up"));
	add_object(new JoystickButton(DOWN_X + 50, DOWN_Y - 50, "RIGHT", InputManager::JoystickButton::RIGHT, joystick_id, "right"));
	add_object(new JoystickButton(DOWN_X, DOWN_Y, "DOWN", InputManager::JoystickButton::DOWN, joystick_id, "down"));
	add_object(new JoystickButton(DOWN_X - 50, DOWN_Y - 50, "LEFT", InputManager::JoystickButton::LEFT, joystick_id, "left"));

	//ABXY
	add_object(new JoystickButton(A_X - 75, A_Y - 75, "X", InputManager::JoystickButton::X, joystick_id, "button"));
	add_object(new JoystickButton(A_X, A_Y, "A", InputManager::JoystickButton::A, joystick_id, "button"));
	add_object(new JoystickButton(A_X + 75, A_Y - 75, "B", InputManager::JoystickButton::B, joystick_id, "button"));
	add_object(new JoystickButton(A_X, A_Y - 150, "Y", InputManager::JoystickButton::Y, joystick_id, "button"));

	//Triggers
	add_object(new JoystickButton(LB_X, LB_Y, "LB", InputManager::JoystickButton::LB, joystick_id, "trigger"));
	add_object(new JoystickButton(LB_X + 500, LB_Y, "RB", InputManager::JoystickButton::RB, joystick_id, "trigger"));
	add_object(new JoystickButton(LB_X, LB_Y - 75, "LT", InputManager::JoystickButton::LT, joystick_id, "back_trigger"));
	add_object(new JoystickButton(LB_X + 500, LB_Y - 75, "RT", InputManager::JoystickButton::RT, joystick_id, "back_trigger"));

	//Select e start
	add_object(new JoystickButton(SELECT_X, SELECT_Y, "select", InputManager::JoystickButton::SELECT, joystick_id, "select_start"));
	add_object(new JoystickButton(MIDDLE_X + 75, SELECT_Y, "start", InputManager::JoystickButton::START, joystick_id, "select_start"));
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

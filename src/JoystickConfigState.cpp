#include "SDL_mixer.h"

#include "JoystickConfigState.h"
#include "InputManager.h"
#include "MenuState.h"
#include "OptionsState.h"
#include "JoystickButton.h"
#include "Game.h"

#define MIDDLE_X 250
#define BOTTOM_Y 275
#define A_X MIDDLE_X + 150
#define A_Y BOTTOM_Y
#define LB_X MIDDLE_X - 150
#define LB_Y BOTTOM_Y - 200
#define DOWN_X LB_X
#define DOWN_Y BOTTOM_Y - 15
#define SELECT_X MIDDLE_X - 40
#define SELECT_Y BOTTOM_Y - 100
#define OFFSET_X 700
#define OFFSET_Y 400
#define BUTTON_DISTANCE 50
#define DPAD_DISTANCE 35
#define TRIGGERS_X_DISTANCE 300
#define TRIGGERS_Y_DISTANCE 40
#define DISTANCE_START 40

#define WHITE { 255, 255, 255, 255 }
#define LIGHT_GREEN { 181, 201, 60, 1 }


JoystickConfigState::JoystickConfigState(int joystick_id){
	Mix_AllocateChannels(50);

	background = Sprite("joysticks/help/background.png");
	joystick_help = Sprite("joysticks/help/controls_help.png");
	test_btn = Sprite("joysticks/help/a.png");
	back_btn = Sprite("joysticks/help/b.png");
	analogic = Sprite("joysticks/help/analogic.png");
	test_txt = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "TEST", WHITE);
	back_txt = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "BACK", WHITE);

	press_both = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "PRESS BOTH", LIGHT_GREEN, 640, 280);
	to_go_back = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "TO GO BACK", LIGHT_GREEN, 640, 450);

	test_txt->set_pos(970, 650);
	back_txt->set_pos(1154, 650);

	blocked = Sound("menu/sound/cancel.ogg");
	selected = Sound("menu/sound/select.ogg");
	changed = Sound("menu/sound/cursor.ogg");

	on_test = false;

	//D-Pad
	for(int i = 0; i < 4; ++i){
		joystick_id = i;
		int offset_x = OFFSET_X * (i % 2) + 20;
		int offset_y = OFFSET_Y * (i > 1);
		add_object(new JoystickButton(offset_x + DOWN_X, offset_y + DOWN_Y - 2 * DPAD_DISTANCE, "UP", InputManager::UP, joystick_id, "up"));
		add_object(new JoystickButton(offset_x + DOWN_X + DPAD_DISTANCE, offset_y + DOWN_Y - DPAD_DISTANCE, "RIGHT", InputManager::RIGHT, joystick_id, "right"));
		add_object(new JoystickButton(offset_x + DOWN_X, offset_y + DOWN_Y, "DOWN", InputManager::DOWN, joystick_id, "down"));
		add_object(new JoystickButton(offset_x + DOWN_X - DPAD_DISTANCE, offset_y + DOWN_Y - DPAD_DISTANCE, "LEFT", InputManager::LEFT, joystick_id, "left"));

		//ABXY
		add_object(new JoystickButton(offset_x + A_X - BUTTON_DISTANCE, offset_y + A_Y - BUTTON_DISTANCE, "X", InputManager::X, joystick_id, "button"));
		add_object(new JoystickButton(offset_x + A_X, offset_y + A_Y, "A", InputManager::A, joystick_id, "button"));
		add_object(new JoystickButton(offset_x + A_X + BUTTON_DISTANCE, offset_y + A_Y - BUTTON_DISTANCE, "B", InputManager::B, joystick_id, "button"));
		add_object(new JoystickButton(offset_x + A_X, offset_y + A_Y - 2 * BUTTON_DISTANCE, "Y", InputManager::Y, joystick_id, "button"));

		//Triggers
		add_object(new JoystickButton(offset_x + LB_X, offset_y + LB_Y, "LB", InputManager::LB, joystick_id, "trigger"));
		add_object(new JoystickButton(offset_x + LB_X + TRIGGERS_X_DISTANCE, offset_y + LB_Y, "RB", InputManager::RB, joystick_id, "trigger"));
		add_object(new JoystickButton(offset_x + LB_X, offset_y + LB_Y - TRIGGERS_Y_DISTANCE, "LT", InputManager::LT, joystick_id, "back_trigger"));
		add_object(new JoystickButton(offset_x + LB_X + TRIGGERS_X_DISTANCE, offset_y + LB_Y - TRIGGERS_Y_DISTANCE, "RT", InputManager::RT, joystick_id, "back_trigger"));

		//Select and start
		add_object(new JoystickButton(offset_x + SELECT_X, offset_y + SELECT_Y, "select", InputManager::SELECT, joystick_id, "select_start"));
		add_object(new JoystickButton(offset_x + MIDDLE_X + DISTANCE_START, offset_y + SELECT_Y, "start", InputManager::START, joystick_id, "select_start"));

	}

	InputManager::get_instance()->set_analogic_value(20000);
}

void JoystickConfigState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->key_press(InputManager::K_X) ||
		input_manager->joystick_button_press(InputManager::A, 0)
	){
		selected.play();
		on_test = true;
	}

	// keyboard
	if(input_manager->key_press(InputManager::K_SELECT) ||
		input_manager->key_press(InputManager::K_LB)
	){
		if(on_test){
			selected.play();
			on_test = false;
		}
		else{
			selected.play();
			m_quit_requested = true;
			Game::get_instance().push(new OptionsState());
			return;
		}
	}

	// joystick
	if((input_manager->is_joystick_button_down(InputManager::R3, 0) &&
		input_manager->is_joystick_button_down(InputManager::L3, 0))
	){
		selected.play();
		on_test = false;
	}

	if(input_manager->joystick_button_press(InputManager::B, 0)){
		selected.play();
		m_quit_requested = true;
		Game::get_instance().push(new OptionsState());
		return;
	}

	update_array(delta);
}

void JoystickConfigState::render(){
	if(not on_test){
		joystick_help.render(0, 0);
		test_btn.render(914, 642);
		back_btn.render(1094, 642);
		test_txt->render();
		back_txt->render();
	}
	else{
		background.render(0, 0);
		render_array();
		press_both->render();
		analogic.render(590, 310);
		to_go_back->render();
	}
}

void JoystickConfigState::pause(){

}

void JoystickConfigState::resume(){

}

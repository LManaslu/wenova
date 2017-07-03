#include "MenuState.h"
#include "OptionsState.h"
#include "InputManager.h"
#include "BattleState.h"
#include "JoystickConfigState.h"
#include "EditState.h"
#include "Game.h"
#include "Resources.h"
#include "StageSelectState.h"

#define OPTION_OFFSET 50

#define FONT_X 640
#define FONT_Y 680

#define LIGHT_GREEN { 181, 201, 60, 1 }
#define WHITE { 255, 255, 255, 255 }

#define FRAME_TIME 7.5
#define TEXT_TIMER_COOLDOWN 50

MenuState::MenuState(bool main_menu){
	current_option = 0;
	start_pressed = main_menu;
	show_text = true;

	background = Sprite("menu/background.jpg");
	title = Sprite("menu/title.png", 5, FRAME_TIME);
	planet = Sprite("menu/planet.png", 8, FRAME_TIME);
	green_ship = Sprite("menu/green_ship.png", 8, FRAME_TIME, 0, 2);
	red_ship = Sprite("menu/red_ship.png", 8, FRAME_TIME);

	start_option = new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "PRESS START", LIGHT_GREEN, FONT_X, FONT_Y);

	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "START", WHITE, FONT_X, FONT_Y));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "EDIT", WHITE, FONT_X, FONT_Y));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "OPTIONS", WHITE, FONT_X, FONT_Y));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "EXIT", WHITE, FONT_X, FONT_Y));

	InputManager::get_instance()->set_analogic_value(32000);
}

void MenuState::update(float delta){
	title.update(delta);
	planet.update(delta);
	green_ship.update(delta);
	red_ship.update(delta);

	InputManager * input_manager = InputManager::get_instance();

	// handling general inputs
	if(input_manager->quit_requested() ||
		input_manager->key_press(InputManager::K_SELECT) ||
		input_manager->joystick_button_press(InputManager::B, 0)
	){
		m_quit_requested = true;
		return;
	}

	// handling options input
	if(input_manager->key_press(InputManager::K_LEFT) ||
		input_manager->joystick_button_press(InputManager::LEFT, 0)
	){
		if(current_option != 0)
			current_option--;
	}

	if(input_manager->key_press(InputManager::K_RIGHT) ||
		input_manager->joystick_button_press(InputManager::RIGHT, 0)
	){
		if(current_option != (int)options.size() - 1)
			current_option++;
	}

	if((input_manager->is_key_down(InputManager::K_LB) and
		input_manager->is_key_down(InputManager::K_RB) and
		input_manager->is_key_down(InputManager::K_Y))
		||
		(input_manager->is_joystick_button_down(InputManager::LB, 0) and input_manager->is_joystick_button_down(InputManager::RT, 0) and input_manager->is_joystick_button_down(InputManager::Y, 0))
	){
		m_quit_requested = true;
		Game::get_instance().push(new EditState("2"));
		return;
	}

	if(input_manager->key_press(InputManager::K_START) ||
		input_manager->key_press(InputManager::K_X) ||
		input_manager->joystick_button_press(InputManager::START, 0) ||
		input_manager->joystick_button_press(InputManager::A, 0)
	){
		if(not start_pressed){
			start_pressed = true;
			current_option = 0;
		}
		else{
			m_quit_requested = true;

			if(current_option == 0)
				Game::get_instance().push(new StageSelectState());
			else if(current_option == 1)
				Game::get_instance().push(new StageSelectState(true));
			else if(current_option == 2)
				Game::get_instance().push(new OptionsState());

			return;
		}
	}

	if(start_pressed){
		// handling options positioning
		options[current_option]->set_pos(FONT_X, FONT_Y, true, true);
		options[current_option]->set_color(LIGHT_GREEN);

		// positioning options before current option
		for(int idx = 0; idx < current_option; idx++){
			Text* next_option = options[idx + 1];

			int new_x = next_option->get_x() - options[idx]->get_width() - OPTION_OFFSET;
			options[idx]->set_pos(new_x, FONT_Y, false, true);
			options[idx]->set_color(WHITE);
		}

		// positioning options after current option
		for(unsigned int idx = current_option + 1; idx < options.size(); idx++){
			Text* prev_option = options[idx - 1];

			int new_x = prev_option->get_x() + prev_option->get_width() + OPTION_OFFSET;
			options[idx]->set_pos(new_x, FONT_Y, false, true);
			options[idx]->set_color(WHITE);
		}
	}

	if(text_timer.get() > TEXT_TIMER_COOLDOWN){
		show_text = !show_text;
		text_timer.restart();
	}

	text_timer.update(delta);
}

void MenuState::render(){
	background.render(0, 0);

	planet.render(423, 177);
	green_ship.render(805, 405);
	red_ship.render(36, 400);
	title.render(260, 0);

	if(start_pressed){
		for(auto option_text : options){
			option_text->render(0, 0);
		}
	}
	else if(show_text){
		start_option->render(0, 0);
	}
}

void MenuState::pause(){

}

void MenuState::resume(){

}

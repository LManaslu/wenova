#include "MenuState.h"
#include "OptionsState.h"
#include "InputManager.h"
#include "BattleState.h"
#include "JoystickConfigState.h"
#include "EditState.h"
#include "Game.h"
#include "Resources.h"
#include "CharacterSelectState.h"

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
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "OPTIONS", WHITE, FONT_X, FONT_Y));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "EXIT", WHITE, FONT_X, FONT_Y));

	InputManager::get_instance()->set_analogic_value(32000);

	blocked = Sound("menu/sound/cancel.ogg");
	selected = Sound("menu/sound/select.ogg");
	changed = Sound("menu/sound/cursor.ogg");
}

void MenuState::update(float delta){
	process_input();

	title.update(delta);
	planet.update(delta);
	green_ship.update(delta);
	red_ship.update(delta);

	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested() || pressed[BACK]){
		m_quit_requested = true;
		return;
	}

	if(pressed[LEFT]){
		changed.play();
		current_option = (current_option - 1 + options.size()) % options.size();
	}

	if(pressed[RIGHT]){
		changed.play();
		current_option = (current_option + 1) % options.size();
	}

	if(pressed[START] || pressed[A]){
		selected.play();

		if(not start_pressed){
			start_pressed = true;
			current_option = 0;
		}
		else{
			m_quit_requested = true;

			if(current_option == 0)
				Game::get_instance().push(new CharacterSelectState());
			else if(current_option == 1)
				Game::get_instance().push(new OptionsState());

			return;
		}
	}

	if(is_holding[LB] && is_holding[RT] && is_holding[Y]){
		m_quit_requested = true;
		Game::get_instance().push(new EditState("1"));
		return;
	}

	if(start_pressed){
		// handling options positioning
		options[current_option]->set_pos(FONT_X, FONT_Y, true, true);
		options[current_option]->set_color(LIGHT_GREEN);

		for(int idx = 0; idx < (int)options.size(); idx++){
			if(idx == current_option) continue;
			bool before = idx < current_option;
			Text* option = options[idx + (before ? 1 : -1)];

			int factor_prev = - options[idx]->get_width() - OPTION_OFFSET;
			int factor_next = option->get_width() + OPTION_OFFSET;
			int new_x = option->get_x() + (before ? factor_prev : factor_next);
			options[idx]->set_pos(new_x, FONT_Y, false, true);
			options[idx]->set_color(WHITE);
		}
	}

	// text timer
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
			option_text->render();
		}
	}
	else if(show_text){
		start_option->render();
	}
}

void MenuState::pause(){

}

void MenuState::resume(){

}

void MenuState::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	vector< pair<int, int> > buttons = {
		ii(BACK, InputManager::K_SELECT),
		ii(LEFT, InputManager::K_LEFT),
		ii(RIGHT, InputManager::K_RIGHT),
		ii(LB, InputManager::K_LB),
		ii(RT, InputManager::K_RT),
		ii(Y, InputManager::K_Y),
		ii(START, InputManager::K_START),
		ii(A, InputManager::K_X)
	};

	vector< pair<int, int> > joystick_buttons = {
		ii(BACK, InputManager::B),
		ii(LEFT, InputManager::LEFT),
		ii(RIGHT, InputManager::RIGHT),
		ii(LB, InputManager::LB),
		ii(RT, InputManager::RT),
		ii(Y, InputManager::Y),
		ii(START, InputManager::START),
		ii(A, InputManager::A)
	};

	int id = (SDL_NumJoysticks() == 0 ? -1 : 0);

	if(id != -1){
		for(ii button : joystick_buttons){
			pressed[button.first] = input_manager->joystick_button_press(button.second, id);
			is_holding[button.first] = input_manager->is_joystick_button_down(button.second, id);
			released[button.first] = input_manager->joystick_button_release(button.second, id);
		}
	}
	else{
		for(ii button : buttons){
			pressed[button.first] = input_manager->key_press(button.second, true);
			is_holding[button.first] = input_manager->is_key_down(button.second, true);
			released[button.first] = input_manager->key_release(button.second, true);
		}
	}
}

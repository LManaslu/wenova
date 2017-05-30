#include "MenuState.h"
#include "InputManager.h"
#include "TitleState.h"
#include "BattleState.h"
#include "EditState.h"
#include "Game.h"
#include "Resources.h"

#define OPTION_OFFSET 50
#define RED { 236, 0, 46, 1 }
#define WHITE { 255, 255, 255, 255 }

MenuState::MenuState() : current_option(0) {
	background = Sprite("menu/background.png");

	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "START", RED, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "CONTINUE", RED, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "OPTIONS", RED, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "EXIT", RED, 640, 640));
}

void MenuState::update(float){
	InputManager input_manager = InputManager::get_instance();

	// handling general inputs
	if(input_manager.key_press(SDLK_ESCAPE) or input_manager.quit_requested()){
		m_quit_requested = true;
		Game::get_instance().push(new TitleState());
		return;
	}

	// handling options input
	if(input_manager.key_press(SDLK_LEFT) && current_option != 0){
		current_option--;
	}

	if(input_manager.key_press(SDLK_RIGHT) && current_option != (int)options.size() - 1){
		current_option++;
	}

	if(input_manager.is_key_down(SDLK_w) and input_manager.is_key_down(SDLK_r) and input_manager.is_key_down(SDLK_c)){
		m_quit_requested = true;
		Game::get_instance().push(new EditState("1"));
		return;
	}

	// TODO when press space switch case in options
	if(input_manager.key_press(SDLK_RETURN)){
		switch(current_option){
			case 0:
				m_quit_requested = true;
				Game::get_instance().push(new BattleState("1"));
				return;

			case 1:
				printf("CONTINUE SELECTED\n");
				break;

			case 2:
				printf("OPTIONS SELECTED\n");
				break;

			case 3:
				m_quit_requested = true;
				Game::get_instance().push(new TitleState());
				return;
		}
	}

	// handling options positioning
	options[current_option]->set_pos(640, 640, true, true);
	options[current_option]->set_color(WHITE);

	// positioning options before current option
	for(int idx = 0; idx < current_option; idx++){
		Text* next_option = options[idx + 1];

		int new_x = next_option->get_x() - options[idx]->get_width() - OPTION_OFFSET;
		options[idx]->set_pos(new_x, 640, false, true);
		options[idx]->set_color(RED);
	}

	// positioning options after current option
	for(unsigned int idx = current_option + 1; idx < options.size(); idx++){
		Text* prev_option = options[idx - 1];

		int new_x = prev_option->get_x() + prev_option->get_width() + OPTION_OFFSET;
		options[idx]->set_pos(new_x, 640, false, true);
		options[idx]->set_color(RED);
	}
}

void MenuState::render(){
	background.render(0, 0);

	for(auto option_text : options){
		option_text->render(0, 0);
	}
}

void MenuState::pause(){

}

void MenuState::resume(){

}

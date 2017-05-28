#include "MenuState.h"
#include "InputManager.h"
#include "TitleState.h"
#include "BattleState.h"
#include "Game.h"

#define OPTION_OFFSET 50

MenuState::MenuState() : current_option(0) {
	background = Sprite("menu/background.png");

	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "START", {255, 255, 255, 255}, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "CONTINUE", {255, 255, 255, 255}, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "OPTIONS", {255, 255, 255, 255}, 640, 640));
	options.push_back(new Text("font/8-BIT WONDER.ttf", 45, Text::TextStyle::SOLID, "EXIT", {255, 255, 255, 255}, 640, 640));
}

void MenuState::update(float delta){
	printf("CURRENT OPTION: %d\n", current_option);

	InputManager input_manager = InputManager::get_instance();

	// handling general inputs
	if(input_manager.key_press(SDLK_ESCAPE) or input_manager.quit_requested()){
		m_quit_requested = true;
		Game::get_instance().push(new TitleState());
		return;
	}

	if(input_manager.key_press(SDLK_SPACE)){
		m_quit_requested = true;
		Game::get_instance().push(new BattleState("1"));
		return;
	}

	// handling options input
	if(input_manager.key_press(SDLK_LEFT)){
		current_option = (current_option - 1) % options.size();
	}

	if(input_manager.key_press(SDLK_RIGHT)){
		current_option = (current_option + 1) % options.size();
	}

	// TODO when press space switch case in options

	// handling options positioning
	options[current_option]->set_pos(640, 640, true, true);
	options[current_option]->set_color({ 255, 0, 0, 1 });

	int idx = (current_option + 1) % options.size();

	for(int counter = 0; counter < options.size() - 1; counter++){
		int prev_idx = (idx - 1) % options.size();
		Text* prev_option = options[prev_idx];

		// printf("PREV: (x: %f, y: %f, w: %f, h: %f)")

		int new_x = prev_option->get_x() + prev_option->get_width() + OPTION_OFFSET;
		options[idx]->set_pos(new_x, 640, false, true);
		options[idx]->set_color({ 255, 255, 255, 255 });

		idx = (idx + 1) % options.size();
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

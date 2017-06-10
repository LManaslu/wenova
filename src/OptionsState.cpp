#include "OptionsState.h"
#include "MenuState.h"
#include "InputManager.h"
#include "Game.h"

#define FONT_X 640
#define FONT_Y 680
#define TEXT_OFFSET 8
#define TEXT_HEIGHT 30
#define OPTION_OFFSET 70

#define RED { 236, 0, 46, 1 }
#define WHITE { 255, 255, 255, 255 }

OptionsState::OptionsState(){
	current_option = current_sub_option = 0;
	on_submenu = false;

	background = Sprite("menu/background.jpg");

	title = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "OPTIONS", WHITE, FONT_X, 100);
	back_button = new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "BACK", WHITE, FONT_X, FONT_Y);

	build_options();
}

void OptionsState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested() || input_manager->key_press(SDLK_ESCAPE)){
		if(on_submenu){
			on_submenu = false;
		}
		else{
			m_quit_requested = true;
			Game::get_instance().push(new MenuState(true));
			return;
		}
	}

	if(input_manager->key_press(SDLK_UP)){
		if(not on_submenu && current_option != 0){
			current_option--;
		}
		else{
			if(current_sub_option != 0){
				current_sub_option--;
			}
		}
	}

	if(input_manager->key_press(SDLK_DOWN)){
		if(not on_submenu && current_option != options.size() - 1){
			current_option++;
		}
		else{
			string text = options[current_option]->get_text();
			if(current_sub_option != (int)sub_options[text].size() - 1){
				current_sub_option++;
			}
		}
	}

	if(input_manager->key_press(SDLK_RETURN)) {
		if(not on_submenu){
			on_submenu = true;
			current_sub_option = 0;
		}
		else{
			if(current_option == 0){ // screen resolution
				int new_width = 0, new_height = 0;
				switch(current_sub_option){
					case 0:
						new_width = 800;
						new_height = 600;
						break;
					case 1:
						new_width = 1024;
						new_height = 768;
						break;
					case 2:
						new_width = 1280;
						new_height = 720;
						break;
					case 3:
						new_width = 1920;
						new_height = 1080;
						break;
				}

				Game::get_instance().change_resolution(new_width, new_height);
			}
			else if(current_option == 1){ // fullscreen
				// TODO Add fullscreen method on Game
				switch(current_sub_option){
					case 1: // on
						break;
					case 2: // false
						break;
				}
			}
		}
	}

	// positions
	for(int i=0; i<options.size(); i++){
		Text* cur_text = options[i];

		int prev_text_size = (i ? sub_options[options[i-1]->get_text()].size() : 1);
		int prev_text_height = (TEXT_HEIGHT + TEXT_OFFSET * 2) * prev_text_size;

		int text_x = 250;
		int text_y = (i ? options[i-1]->get_y() + prev_text_height : 200);

		cur_text->set_pos(text_x, text_y, false, false);

		for(int j=0; j<sub_options[cur_text->get_text()].size(); j++){
			int x = 800;
			int y = text_y;

			if(j){
				Text* prev_option = sub_options[cur_text->get_text()][j-1];
				y = prev_option->get_y() + prev_option->get_height() + TEXT_OFFSET;
			}

			Text* option = sub_options[cur_text->get_text()][j];
			option->set_pos(x, y);
		}
	}
}

void OptionsState::build_options(){
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "SCREEN RESOLUTION", WHITE, 100, 200));
	options.back()->set_pos(100, 200, false, false);
	options.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "FULLSCREEN", WHITE));

	sub_options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "800 x 600", WHITE));
	sub_options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1024 x 768", WHITE));
	sub_options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1280 x 720", WHITE));
	sub_options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1920 x 1080", WHITE));

	sub_options["FULLSCREEN"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "ON", WHITE));
	sub_options["FULLSCREEN"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "OFF", WHITE));
}

void OptionsState::render(){
	background.render(0, 0);

	title->render();
	back_button->render();

	for(int i=0; i<options.size(); i++){
		if(current_option == i)
			options[i]->set_color(RED);
		else
			options[i]->set_color(WHITE);

		options[i]->render();

		string text = options[i]->get_text();
		for(int j=0; j<sub_options[text].size(); j++){
			// TODO get option selected from db
			if(current_sub_option == j)
				sub_options[text][j]->set_color(RED);
			else
				sub_options[text][j]->set_color(WHITE);

			sub_options[text][j]->render();
		}
	}
}

void OptionsState::pause(){

}

void OptionsState::resume(){

}

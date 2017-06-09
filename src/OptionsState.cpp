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
	current_option_text = current_option = 0;

	background = Sprite("menu/background.jpg");

	title = new Text("font/8-BIT WONDER.ttf", 50, Text::TextStyle::SOLID, "OPTIONS", WHITE, FONT_X, 100);
	back_button = new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "BACK", WHITE, FONT_X, FONT_Y);

	build_options();
}

void OptionsState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested() || input_manager->key_press(SDLK_ESCAPE)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

	// positions
	// FIXME make loop
  for(int i=0; i<options_text.size();i++){
    Text* cur_text = options_text[i];

    int prev_text_size = (i ? options[options_text[i-1]->get_text()].size() : 1);
    int prev_text_height = (TEXT_HEIGHT + TEXT_OFFSET * 2) * prev_text_size;

    int text_x = 250;
    int text_y = (i ? options_text[i-1]->get_y() + prev_text_height : 200);

    cur_text->set_pos(text_x, text_y, false, false);

    for(int j=0; j<options[cur_text->get_text()].size(); j++){
      int x = 800;
      int y = text_y;

      if(j){
        Text* prev_option = options[cur_text->get_text()][j-1];
        y = prev_option->get_y() + prev_option->get_height() + TEXT_OFFSET;
      }

      Text* option = options[cur_text->get_text()][j];
      option->set_pos(x, y);
    }
  }
}

void OptionsState::build_options(){
	// FIXME arrumar posições
	options_text.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "SCREEN RESOLUTION", WHITE, 100, 200));
	options_text.back()->set_pos(100, 200, false, false);
	options_text.push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "FULLSCREEN", WHITE));

	options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "800 x 600", WHITE));
	options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1024 x 768", WHITE));
	options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1280 x 720", WHITE));
	options["SCREEN RESOLUTION"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "1920 x 1080", WHITE));

	options["FULLSCREEN"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "ON", WHITE));
	options["FULLSCREEN"].push_back(new Text("font/8-BIT WONDER.ttf", 30, Text::TextStyle::SOLID, "OFF", WHITE));
}

void OptionsState::render(){
	background.render(0, 0);

	title->render();
	back_button->render();

	for(option_text : options_text){
		option_text->render();

		for(auto option : options[option_text->get_text()]){
			option->render();
		}
	}
}


void OptionsState::pause(){

}

void OptionsState::resume(){

}

#include "EditState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "EditableFloor.h"
#include "MenuState.h"

#define WIDTH 1280
#define HEIGHT 720
#define CONTROL 1073742048

#define N_BACKGROUND 2

using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::to_string;

EditState::EditState(string cstage) : stage(cstage){
	int joystick_id = (SDL_NumJoysticks() == 0 ? -1 : 0);
	test_fighter = new Fighter("edit_state/test_fighter", WIDTH/2, HEIGHT/2 - 200, joystick_id);
	add_object(test_fighter);

	read_level_design();
	InputManager::get_instance()->set_analogic_value(20000);
}

void EditState::update(float delta){
	InputManager * inputManager = InputManager::get_instance();

	if(inputManager->key_press(SDLK_ESCAPE) or inputManager->joystick_button_press(InputManager::SELECT, 0)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(inputManager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(inputManager->mouse_press(InputManager::RIGHT_MOUSE_BUTTON)){
		int x = inputManager->get_mouse_x();
		int y = inputManager->get_mouse_y();
		test_fighter->reset_position(x, y);
	}

	if(inputManager->key_press(SDLK_f) or inputManager->key_press(SDLK_p)){
		int x = inputManager->get_mouse_x();
		int y = inputManager->get_mouse_y();
		bool is_platform = inputManager->key_press(SDLK_p);
		for(auto & go : object_array){
			if(go->is("floor")){
				((EditableFloor *) go.get())->set_selected(false);
			}
		}
		auto go = new EditableFloor(x, y, 0, is_platform);
		go->set_selected(true);
		add_object(go);
	}

	if(inputManager->is_key_down(CONTROL) and inputManager->key_press(SDLK_c)){
		update_level_design();
	}

	for(auto & background : backgrounds)
		background.first.update(delta);

	update_array(delta);
}

void EditState::render(){
	for(auto & background : backgrounds){
		background.first.render(background.second.x, background.second.y);
	}

	render_array();
}

void EditState::pause(){

}

void EditState::resume(){

}

void EditState::read_level_design(){
	float x, y, width, crotation;
	int platform;
	ifstream level_design("res/stage_" + stage + "/level_design.dat");
	if(not level_design.is_open()){
		printf("Level design of stage %s can't be opened\n", stage.c_str());
		exit(-5);
	}
	string s;
	int n_backgrounds, n_sprites, speed, n_columns;

	std::getline(level_design, s);
	for(auto & c : s) c -= 15;
	stringstream n_background_line(s);
	n_background_line >> n_backgrounds;

	for(int i = 0; i < n_backgrounds; ++i){
		std::getline(level_design, s);
		for(auto & c : s) c -= 15;
		stringstream backgrounds_line(s);
		backgrounds_line >> x >> y >> n_sprites >> speed >> n_columns;
		//printf("Dados: %.f %.f %d %d %d\n", x, y, n_sprites, speed, n_columns);
		Sprite background_sprite("stage_" + stage + "/background_" + to_string(i) + ".png", n_sprites, speed, n_columns);
		Vector position(x, y);
		backgrounds.push_back(std::make_pair(background_sprite, position));
	}


	while(std::getline(level_design, s)){
		for(auto & c : s) c -= 15;
		stringstream editable_floors_line(s);
		editable_floors_line >> x >> y >> width >> crotation >> platform;
		//printf("Battle: %.f %.f %.f %.f\n", x, y, width, crotation);
		add_object(new EditableFloor(x, y, width, crotation, (bool) platform));
 	}
	level_design.close();
}


void EditState::update_level_design(){
	ifstream level_design("res/stage_" + stage + "/level_design.dat", std::ios::binary);
	ofstream old_level_design("res/stage_" + stage + "/level_design.dat.old", std::ios::trunc | std::ios::binary);
	old_level_design << level_design.rdbuf();
	level_design.close();
	old_level_design.close();

	ofstream new_level_design("res/stage_" + stage + "/level_design.dat", std::ios::trunc);
	ifstream backup("res/stage_" + stage + "/level_design.dat.old", std::ios::binary);
	string s;
	for(unsigned i = 0; i <= backgrounds.size(); ++i){
		std::getline(backup, s);
		new_level_design << s << std::endl;
	}
	old_level_design.close();

	for(auto & go : object_array){
		if(go->is("floor")){
			new_level_design << ((EditableFloor *) go.get())->get_information() << std::endl;
		}
	}
	new_level_design.close();
}

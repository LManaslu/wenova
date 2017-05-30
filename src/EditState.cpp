#include "EditState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "EditableFloor.h"

#define WIDTH 1280
#define HEIGHT 720

using std::fstream;
using std::stringstream;

EditState::EditState(string stage){
	background = Sprite("stage_" + stage + "/background.png", 6, 1);

	test_fighter = new Fighter("fighter", WIDTH/2, HEIGHT/2 - 200);
	add_object(test_fighter);

	read_level_design(stage);

	//TODO ler os tiles que já tem e colocar
}

void EditState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(inputManager.mouse_press(InputManager::RIGHT_MOUSE_BUTTON)){
		int x = inputManager.get_mouse_x();
		int y = inputManager.get_mouse_y();
		test_fighter->reset_position(x, y);
	}

	if(inputManager.key_press(SDLK_n)){
		int x = inputManager.get_mouse_x();
      	int y = inputManager.get_mouse_y();
		add_object(new EditableFloor(x, y, 0));
	}

	//printf("Floors\n------------------------------------\n");
	update_array(delta);
	//printf("-------------------------------\n");
}

void EditState::render(){
	background.render(0, 0);

	render_array();
}

void EditState::pause(){

}

void EditState::resume(){

}

void EditState::read_level_design(string stage){
	float x, y, width, crotation;
	fstream level_design("res/stage_" + stage + "/level_design.dat");
	if(not level_design.is_open()){
		printf("Level design of stage %s can't be opened\n", stage.c_str());
		exit(-5);
	}
	string s;
	while(std::getline(level_design, s)){
		for(auto & c : s) c -= 10;
		stringstream cim(s);
		cim >> x >> y >> width >> crotation;
		printf("Edit: %.f %.f %.f %.f\n", x, y, width, crotation);
		add_object(new EditableFloor(x, y, width, crotation));
 	}
	level_design.close();
}

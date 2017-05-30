#include "BattleState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "Floor.h"

using std::fstream;
using std::stringstream;

BattleState::BattleState(string stage){
	background = Sprite("stage_" + stage + "/background.png", 6, 1);

	read_level_design(stage);

	add_object(new Fighter("fighter", 100, 100));
}

void BattleState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE) or inputManager.quit_requested()){
		m_quit_requested = true;
		return;
	}

	update_array(delta);
}

void BattleState::render(){
	background.render(0, 0);

	render_array();
}

void BattleState::pause(){

}

void BattleState::resume(){

}

void BattleState::read_level_design(string stage){
	float x, y, width, height, crotation;
	fstream level_design("res/stage_" + stage + "/level_design.dat");
	fstream test("res/stage_1/test.dat");
	if(not level_design.is_open()) exit(-5);
	string s;
	while(std::getline(level_design, s)){
		for(auto & c : s) c -= 10;
		stringstream cim(s);
		cim >> x >> y >> width >> height >> crotation;
		add_object(new Floor(x, y, width, height, crotation));
 	}
}

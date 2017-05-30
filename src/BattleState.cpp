#include "BattleState.h"

#include <fstream>

#include "InputManager.h"
#include "Game.h"
#include "Fighter.h"
#include "Floor.h"

BattleState::BattleState(string stage){
	background = Sprite("stage_" + stage + "/background.png", 6, 1);

	float x, y, width, height, crotation;
	std::fstream level_design("res/stage_" + stage + "/level_design.txt");
	if(not level_design.is_open()) exit(-5);
	while(level_design >> x >> y >> width >> height >> crotation){
		add_object(new Floor(x, y, width, height, crotation));
		printf("%.f %.f %.f %.f %.f\n", x, y, width, height, crotation);
	}

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

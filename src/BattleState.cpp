#include "BattleState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Floor.h"
#include "MenuState.h"
#include "FighterStats.h"

using std::fstream;
using std::stringstream;

BattleState::BattleState(string stage, string cmusic){
	background = Sprite("stage_" + stage + "/background.png", 6, 30);
	music = Music("stage_" + stage + "/" + cmusic);

	read_level_design(stage);


	music.play();

	fighters.push_back(new Fighter("flesh", 100, 100));
	fighters.push_back(new Fighter("flesh", 200, 100));
	fighters.push_back(new Fighter("flesh", 300, 100));
	fighters.push_back(new Fighter("flesh", 400, 100));

	for(unsigned i = 0; i < fighters.size(); i++)
		add_object(fighters[i]);

	add_object(new FighterStats("Timer"));
	add_object(new FighterStats("Life1", fighters[0]));
	add_object(new FighterStats("Life2", fighters[1]));
	add_object(new FighterStats("Life3", fighters[2]));
	add_object(new FighterStats("Life4", fighters[3]));
}

BattleState::~BattleState(){
//	for(unsigned i = 0; i < fighters.size(); i++)
//		delete(fighters[i]);

//	fighters.clear();
}

void BattleState::update(float delta){
	InputManager inputManager = InputManager::get_instance();

	if(inputManager.key_press(SDLK_ESCAPE)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(inputManager.quit_requested()){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	background.update(delta);

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
	float x, y, width, crotation;
	int platform;
	fstream level_design("res/stage_" + stage + "/level_design.dat");
	if(not level_design.is_open()){
		printf("Level design of stage %s can't be opened\n", stage.c_str());
		exit(-5);
	}
	string s;
	while(std::getline(level_design, s)){
		for(auto & c : s) c -= 10;
		stringstream cim(s);
		cim >> x >> y >> width >> crotation >> platform;
		//printf("Battle: %.f %.f %.f %.f\n", x, y, width, crotation);
		add_object(new Floor(x, y, width, crotation, (bool) platform));
 	}
	level_design.close();
}

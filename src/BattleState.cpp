#include "BattleState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Floor.h"
#include "MenuState.h"
#include "FighterStats.h"
#include "TimeCounter.h"

#define N_BACKGROUND 2

using std::fstream;
using std::stringstream;
using std::to_string;

BattleState::BattleState(string stage, string cmusic){
	for(int i = 0; i < N_BACKGROUND; i++)
		background[i] = Sprite("stage_" + stage + "/background_" + to_string(i) + ".png");

	music = Music("stage_" + stage + "/" + cmusic);

	read_level_design(stage);


	music.play();

	int joystick_id = (SDL_NumJoysticks() == 0 ? -1 : 0);
	fighters.push_back(new Fighter("flesh", 100, 100, joystick_id));
	fighters.push_back(new Fighter("flesh", 200, 100, 1));
	fighters.push_back(new Fighter("flesh", 300, 100, 2));
	fighters.push_back(new Fighter("flesh", 400, 100, 3));

	for(unsigned i = 0; i < fighters.size(); i++)
		add_object(fighters[i]);

	add_object(new TimeCounter());

	add_object(new FighterStats(fighters[3], 4, 1, 1147, 679.5));
	add_object(new FighterStats(fighters[2], 3, 1, 1147, 599.5));
	add_object(new FighterStats(fighters[1], 2, 0, 133, 679.5));
	add_object(new FighterStats(fighters[0], 1, 0, 133, 599.5));

	add_object(new TimeCounter());

	InputManager::get_instance()->set_analogic_value(20000);
}

BattleState::~BattleState(){
//	for(unsigned i = 0; i < fighters.size(); i++)
//		delete(fighters[i]);

//	fighters.clear();
}

void BattleState::update(float delta){
	InputManager * inputManager = InputManager::get_instance();

	if(inputManager->key_press(SDLK_ESCAPE) or inputManager->joystick_button_press(InputManager::SELECT, 0)){
		music.stop();
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(inputManager->quit_requested()){
		music.stop();
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	for(int i = 0; i < N_BACKGROUND; i++)
		background[i].update(delta);

	update_array(delta);
}

void BattleState::render(){
	for(int i = 0; i < N_BACKGROUND; i++)
		background[i].render(0, 0);

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

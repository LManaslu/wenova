#include "BattleState.h"

#include <fstream>
#include <sstream>

#include "InputManager.h"
#include "Game.h"
#include "Floor.h"
#include "MenuState.h"
#include "FighterStats.h"
#include "TimeCounter.h"
#include "Config.h"
#include "Blood.h"

#define N_BACKGROUND 2

using std::fstream;
using std::stringstream;
using std::to_string;

BattleState::BattleState(string stage, string cmusic){

	music = Music("stage_" + stage + "/" + cmusic);

	read_level_design(stage);


	music.play();

	Fighter * player_1 = new Blood("default", 177, 313, SDL_NumJoysticks() == 0 ? -1 : 0);
	Fighter * player_2 = new Blood("default", 276, 510, SDL_NumJoysticks() == 1 ? -1 : 1);
	Fighter * player_3 = new Blood("default", 1128, 245, SDL_NumJoysticks() == 2 ? -1 : 2);
	Fighter * player_4 = new Blood("default", 954, 474, SDL_NumJoysticks() == 3 ? -1 : 3);

	player_1->set_partner(player_2);
	player_2->set_partner(player_1);
	player_3->set_partner(player_4);
	player_4->set_partner(player_3);

	add_object(new TimeCounter());

	add_object(new FighterStats(player_4, 4, 1, 1147, 679.5));
	add_object(new FighterStats(player_3, 3, 1, 1147, 599.5));
	add_object(new FighterStats(player_2, 2, 0, 133, 679.5));
	add_object(new FighterStats(player_1, 1, 0, 133, 599.5));

	add_object(player_4);
	add_object(player_3);
	add_object(player_2);
	add_object(player_1);

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

	for(auto & background : backgrounds)
		background.first.update(delta);

	update_array(delta);
}

void BattleState::render(){
	for(auto & background : backgrounds){
		background.first.render(background.second.x, background.second.y);
	}

	render_array();
}

void BattleState::pause(){

}

void BattleState::resume(){

}

void BattleState::read_level_design(string stage){
	float x, y, width, crotation;
	int platform;
	fstream level_design(RES_FOLDER + "stage_" + stage + "/level_design.dat");
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
		stringstream floors_line(s);
		floors_line >> x >> y >> width >> crotation >> platform;
		//printf("Battle: %.f %.f %.f %.f\n", x, y, width, crotation);
		add_object(new Floor(x, y, width, crotation, (bool) platform));
 	}
	level_design.close();
}

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
#define N_PLAYERS 4

using std::fstream;
using std::stringstream;
using std::to_string;

BattleState::BattleState(string stage, string cmusic, vector< pair<string, string> > players_info){

	music = Music("stage_" + stage + "/" + cmusic);

	read_level_design(stage);

	music.play();

	vector< pair<int, int> > char_positions = { ii(177, 313), ii(276, 510), ii(1128, 245), ii(954, 474) };
	vector< pair<int, int> > hud_positions = { ii(133, 599.5), ii(133, 679.5), ii(1147, 599.5), ii(1147, 679.5) };

	// vector<Fighter*> players(N_PLAYERS, nullptr);
	Fighter* players[N_PLAYERS];

	for(int i = 0; i < (int)players_info.size(); i++){
		string char_name = players_info[i].first;
		string skin_name = players_info[i].second;

		if(char_name == "blood"){
			players[i] = new Blood(skin_name, char_positions[i].first, char_positions[i].second,
									SDL_NumJoysticks() == i ? -1 : i);
		}
		else if(char_name == "flesh"){
			// TODO Instantiate Flesh character
		}
	}

	players[0]->set_partner(players[1]);
	players[1]->set_partner(players[0]);
	players[2]->set_partner(players[3]);
	players[3]->set_partner(players[2]);

	for(int i=N_PLAYERS-1;i>=0;i--){
		add_object(new FighterStats(players[i], i + 1, i > 1,
									hud_positions[i].first,
									hud_positions[i].second));
	}

	for(int i=N_PLAYERS-1; i>=0; i--){
		add_object(players[i]);
	}

	add_object(new TimeCounter());

	InputManager::get_instance()->set_analogic_value(20000);
}

BattleState::~BattleState(){
//	for(unsigned i = 0; i < fighters.size(); i++)
//		delete(fighters[i]);

//	fighters.clear();
}

void BattleState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->key_press(SDLK_ESCAPE) or input_manager->joystick_button_press(InputManager::SELECT, 0)){
		music.stop();
		m_quit_requested = true;
		Game::get_instance().push(new MenuState());
		return;
	}

	if(input_manager->quit_requested()){
		m_quit_requested = true;
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

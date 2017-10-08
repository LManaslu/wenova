#include "BattleState.h"

#include <fstream>
#include <sstream>
#include <cstring>

#include "InputManager.h"
#include "Game.h"
#include "Floor.h"
#include "MenuState.h"
#include "FighterStats.h"
#include "Config.h"
#include "Blood.h"
#include "Flesh.h"
#include "BattleEnd.h"

#define N_BACKGROUND 2

using std::fstream;
using std::stringstream;
using std::to_string;

BattleState::BattleState(string stage, vector< pair<string, string> > players_info){
	game_over = false;
	memset(alive, 1, sizeof alive);

	music = Music("stage_" + stage + "/music.ogg");
	sound = Sound("stage_" + stage + "/sound.ogg");

	read_level_design(stage);

	music.play();
	sound.play(-1);

	vector< pair<int, int> > char_positions;
	vector< pair<int, int> > hud_positions = { ii(133, 589.5), ii(133, 679.5), ii(1147, 589.5), ii(1147, 679.5) };

	if(stage == "1")
		char_positions = { ii(177, 313), ii(276, 510), ii(1128, 245), ii(954, 474) };
	else
		char_positions = { ii(116, 227), ii(146, 394), ii(1036, 221), ii(1063, 382) };

	for(int i = 0; i < (int)players_info.size(); i++){
		string char_name = players_info[i].first;
		string skin_name = players_info[i].second;

		if(char_name == "blood"){
			players[i] = new Blood(skin_name, char_positions[i].first, char_positions[i].second, i);
		}
		else if(char_name == "flesh"){
			players[i] = new Flesh(skin_name, char_positions[i].first, char_positions[i].second, i);
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

	time_counter = new TimeCounter();
	add_object(time_counter);

	InputManager::get_instance()->set_analogic_value(20000);
	InputManager::get_instance()->map_keyboard_to_joystick(InputManager::BATTLE_MODE);
}

BattleState::~BattleState(){
}

void BattleState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(input_manager->joystick_button_press(InputManager::SELECT, 0)){
		music.stop();
		Game::get_instance().push(new MenuState(true));
		m_quit_requested = true;
		return;
	}

	for(int i = 0; i < N_PLAYERS; i++){
		if(alive[i]){
			if(players[i]->is("dying")){
				alive[i] = false;
			}
		}
	}

	if(time_counter->is_over() && not game_over){
		game_over = true;

		if(alive[0] + alive[1] > alive[2] + alive[3]){
			battleEnd = new BattleEnd(1);
			add_object(battleEnd);
		}
		else if(alive[2] + alive[3] > alive[0] + alive[1]){
			battleEnd = new BattleEnd(2);
			add_object(battleEnd);
		}else{
			int sum_life_team_1 = 0;
			for(int i = 0; i < N_PLAYERS / 2; i++){
				if(alive[i])
					sum_life_team_1 += players[i]->get_remaining_life();
			}

			int sum_life_team_2 = 0;
			for(int i = N_PLAYERS / 2; i < N_PLAYERS; i++){
				if(alive[i])
					sum_life_team_2 += players[i]->get_remaining_life();
			}

			if(sum_life_team_1 > sum_life_team_2){
				battleEnd = new BattleEnd(1);
				add_object(battleEnd);
			}else if(sum_life_team_2 > sum_life_team_1){
				battleEnd = new BattleEnd(2);
				add_object(battleEnd);
			}else{
				battleEnd = new BattleEnd(3);
				add_object(battleEnd);
			}
		}
	}

	if(not alive[0] && not alive[1] && not alive[2] && not alive[3] && not game_over){
		game_over = true;
		battleEnd = new BattleEnd(3);
		add_object(battleEnd);
	}

	if(not alive[0] && not alive[1] && not game_over){
		game_over = true;
		battleEnd = new BattleEnd(2);
		add_object(battleEnd);
	}else if(not alive[2] && not alive[3] && not game_over){
		game_over = true;
		battleEnd = new BattleEnd(1);
		add_object(battleEnd);
	}

	if(game_over){
		for(int i = 0; i < N_PLAYERS; i++){
			if(alive[i])
				players[i]->add_tags("game_over");
		}
	}

	if(game_over){
		if(battleEnd->quit_requested()){
			music.stop();
			sound.stop();
			m_quit_requested = true;
			Game::get_instance().push(new MenuState());
			return;
		}
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

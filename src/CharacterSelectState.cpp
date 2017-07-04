#include "CharacterSelectState.h"
#include "StageSelectState.h"
#include "Game.h"
#include "MenuState.h"
#include "BattleState.h"

#define FRAME_TIME 7.5

#define N_PLAYERS 4
#define N_BACKGROUNDS 2
#define N_SKINS 4
#define N_CHARS 2
#define FIRST_PLAYER 0
#define COL_SLOTS 2
#define ROW_SLOTS 4

CharacterSelectState::CharacterSelectState(string cselected_stage){
	memset(cur_selection_col, 0, sizeof cur_selection_col);
	memset(cur_selection_row, 0, sizeof cur_selection_row);
	memset(cur_skin, 0, sizeof cur_skin);
	memset(selected, false, sizeof selected);

	planet = Sprite("character_select/planet.png", 8, FRAME_TIME);
	planet.set_scale(1.5);
	character_slots = Sprite("character_select/character_slots.png");
	selected_tag = Sprite("character_select/selected.png");
	ready_to_fight = Sprite("character_select/ready_to_fight.png");

	ready = false;

	selected_stage = cselected_stage;

	for(int i=0;i<N_BACKGROUNDS;i++){
		background[i] = Sprite("character_select/background_" + to_string(i + 1) + ".png");
	}

	for(int i=0;i<N_PLAYERS;i++){
		name_tag[i] = Sprite("character_select/name_tag_" + to_string(i + 1) + ".png");
		number[i] = Sprite("character_select/number_" + to_string(i + 1) + ".png");
	}

	for(int i=0;i<N_CHARS;i++){
		string character_name = get_char_info(i).first;
		int n_frames = get_char_info(i).second;

		available_skin[character_name].assign(N_SKINS, true);
		char_name[character_name] = Sprite("character_select/name_" + character_name + ".png");

		disabled[character_name] = Sprite(character_name + "/disabled.png", n_frames, 13);
		disabled[character_name].set_scale(3);

		// loop to get skins
		for(int j=0;j<N_SKINS;j++){
			char_sprite[character_name].push_back(
				Sprite(character_name + "/" + get_skin_name(j) + "/idle.png", n_frames, 13)
			);
			char_sprite[character_name][j].set_scale(3);
		}
	}

	names = { {"blood",""}, {"flesh",""}, {"",""}, {"",""} };
	name_tag_positions = { ii(91, 234), ii(92, 583), ii(956, 234), ii(955, 583) };
	number_delta = { ii(12, 9), ii(93, 9), ii(12, 101), ii(93, 101) };
	name_delta = { ii(118, 53), ii(117, 55), ii(47, 54), ii(50, 54) };
	sprite_pos = { ii(155, 32), ii(141, 379), ii(923, 34), ii(946, 381) };
	col_slots = { 510, 645 };
	row_slots = { 55, 197, 395, 536 };
}

void CharacterSelectState::update(float delta){
	process_input();

	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested()){
		m_quit_requested = true;
		Game::get_instance().push(new StageSelectState());
		return;
	}

	if(pressed[FIRST_PLAYER][SELECT] ||
		(not selected[FIRST_PLAYER] && pressed[FIRST_PLAYER][B])
	){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

	// only enable start when all players have selected a character
	if(all_players_selected()){
		ready = true;
		if(pressed[FIRST_PLAYER][START] || pressed[FIRST_PLAYER][A]){
			vector< pair<string, string> > p = export_players();
			m_quit_requested = true;
			Game::get_instance().push(new BattleState(selected_stage, "music.ogg", export_players()));
			return;
		}
	}

	for(int i=0;i<N_PLAYERS;i++){
		if(not selected[i]){
			// random character
			if(pressed[i][Y]){
				int rand_col = 0, rand_row = 0, rand_skin = 0;

				do{
					rand_col = rand() % COL_SLOTS;
					rand_row = rand() % ROW_SLOTS;
				}while(not character_enabled(rand_row, rand_col));

				printf("rand_col: %d, rand_row: %d\n", rand_col, rand_row);
				string char_selected = names[rand_col][rand_row];

				do{
					rand_skin = rand() % N_SKINS;
				}while(not available_skin[char_selected][rand_skin]);

				// printf("[RANDOM] PLAYER %d -> [%s] [%s] SKIN\n", i + 1, get_skin_name(rand_skin).c_str(), char_selected.c_str());

				cur_selection_col[i] = rand_col;
				cur_selection_row[i] = rand_row;
				cur_skin[i] = rand_skin;
			}

			int old_col = cur_selection_col[i];
			int old_row = cur_selection_row[i];

			// change character
			if(pressed[i][LEFT]){
				if(cur_selection_col[i] != 0 &&
					character_enabled(cur_selection_row[i], cur_selection_col[i] - 1)
				){
					cur_selection_col[i]--;
				}
			}

			if(pressed[i][RIGHT]){
				if(cur_selection_col[i] + 1 < COL_SLOTS &&
					character_enabled(cur_selection_row[i], cur_selection_col[i] + 1)
				){
					cur_selection_col[i]++;
				}
			}

			if(pressed[i][UP]){
				if(cur_selection_row[i] != 0 &&
					character_enabled(cur_selection_row[i] - 1, cur_selection_col[i])
				){
					cur_selection_row[i]--;
				}
			}

			if(pressed[i][DOWN]){
				if(cur_selection_row[i] + 1 < ROW_SLOTS &&
					character_enabled(cur_selection_row[i] + 1, cur_selection_col[i])
				){
					cur_selection_row[i]++;
				}
			}

			// reset skin if character changed
			if(cur_selection_col[i] != old_col || cur_selection_row[i] != old_row){
				cur_skin[i] = 0;
			}

			// change skin
			if(pressed[i][LT]){
				cur_skin[i] = (cur_skin[i] - 1 + N_SKINS) % N_SKINS;
			}

			if(pressed[i][RT]){
				cur_skin[i] = (cur_skin[i] + 1) % N_SKINS;
			}

			// select character && lock skin
			if(pressed[i][A]){
				int col_sel = cur_selection_col[i];
				int row_sel = cur_selection_row[i];
				string char_selected = names[col_sel][row_sel];

				if(not available_skin[char_selected][cur_skin[i]]){
					// printf("SKIN [%d] of [%s] ALREADY CHOSEN\n", cur_skin[i], char_selected.c_str());
				}
				else{
					// printf("PLAYER %d CHOSE SKIN [%d] of [%s]\n", i + 1, cur_skin[i], char_selected.c_str());
					available_skin[char_selected][cur_skin[i]] = false;
					selected[i] = true;
				}
			}
		}
		else{
			// unselect character
			if(pressed[i][B]){
				int col_sel = cur_selection_col[i];
				int row_sel = cur_selection_row[i];
				string char_selected = names[col_sel][row_sel];

				available_skin[char_selected][cur_skin[i]] = true;
				selected[i] = false;
				ready = false;
				// printf("PLAYER %d UNSELECTED SKIN [%d] of [%s]\n", i + 1, cur_skin[i], char_selected.c_str());
			}
		}
	}

	for(auto it = char_sprite.begin(); it != char_sprite.end(); it++){
		for(int i=0; i < (int)(*it).second.size(); i++){
			(*it).second[i].update(delta);
		}
	}

	for(auto it = disabled.begin(); it != disabled.end(); it++){
		(*it).second.update(delta);
	}

	planet.update(delta);
}

void CharacterSelectState::render(){
	background[0].render(0, 0);
	planet.render(640 - planet.get_width() / 2, 360 - planet.get_height() / 2);
	background[1].render(0, 0);
	character_slots.render(0, 0);

	for(int i=0;i<N_PLAYERS;i++){
		int col_sel = cur_selection_col[i];
		int row_sel = cur_selection_row[i];

		string char_selected = names[col_sel][row_sel];

		SDL_RendererFlip flip = i >= 2 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		char_sprite[char_selected][cur_skin[i]].render(sprite_pos[i].first, sprite_pos[i].second, 0, flip);

		if(not available_skin[char_selected][cur_skin[i]] && not selected[i]){
			disabled[char_selected].render(sprite_pos[i].first, sprite_pos[i].second, 0, flip);
		}

		name_tag[i].render(name_tag_positions[i].first, name_tag_positions[i].second);

		char_name[char_selected].render(
			name_tag_positions[i].first + name_delta[i].first,
			name_tag_positions[i].second + name_delta[i].second
		);

		number[i].render(col_slots[col_sel] + number_delta[i].first, row_slots[row_sel] + number_delta[i].second);

		if(selected[i]){
			selected_tag.render(name_tag_positions[i].first, name_tag_positions[i].second, 0, flip);
		}
	}

	if(ready){
		ready_to_fight.render(0, 0);
	}
}

bool CharacterSelectState::character_enabled(int row, int){
	// Only characters in first row are available
	return row == 0;
}

bool CharacterSelectState::all_players_selected(){
	for(auto cur : selected)
	if(not cur) return false;
	return true;
}

// returns name and number of frames in corresponding sprite
pair<string, int> CharacterSelectState::get_char_info(int idx){
	vector< pair<string, int> > chars = {
		make_pair("blood", 12),
		make_pair("flesh", 8)
	};

	return chars[idx];
}

string CharacterSelectState::get_skin_name(int idx){
	vector<string> skins_names = { "default", "alt1", "alt2", "alt3" };
	return skins_names[idx];
}

vector< pair<string, string> > CharacterSelectState::export_players(){
	vector< pair<string, string> > players;

	for(int i=0;i<N_PLAYERS;i++){
		int col_sel = cur_selection_col[i];
		int row_sel = cur_selection_row[i];

		string char_selected = names[col_sel][row_sel];

		players.push_back(make_pair(char_selected, get_skin_name(cur_skin[i])));
	}

	return players;
}

void CharacterSelectState::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	// enum Button { A, B, Y, LEFT, RIGHT, UP, DOWN, SELECT, START, LT, RT };
	vector< pair<int, int> > buttons = {
		ii(A, InputManager::K_MENU_A),
		ii(B, InputManager::K_MENU_B),
		ii(Y, InputManager::K_MENU_Y),
		ii(LEFT, InputManager::K_LEFT),
		ii(RIGHT, InputManager::K_RIGHT),
		ii(UP, InputManager::K_UP),
		ii(DOWN, InputManager::K_DOWN),
		ii(SELECT, InputManager::K_SELECT),
		ii(START, InputManager::K_START),
		ii(LT, InputManager::K_LT),
		ii(RT, InputManager::K_RT)
	};

	vector< pair<int, int> > joystick_buttons = {
		ii(A, InputManager::A),
		ii(B, InputManager::B),
		ii(Y, InputManager::Y),
		ii(LEFT, InputManager::LEFT),
		ii(RIGHT, InputManager::RIGHT),
		ii(UP, InputManager::UP),
		ii(DOWN, InputManager::DOWN),
		ii(SELECT, InputManager::SELECT),
		ii(START, InputManager::START),
		ii(LT, InputManager::LT),
		ii(RT, InputManager::RT)
	};

	for(int id = 0; id < N_PLAYERS; id++){
		for(ii button : joystick_buttons){
			pressed[id][button.first] = input_manager->joystick_button_press(button.second, id);
		}
	}

	for(ii button : buttons){
		if(input_manager->key_press(button.second, true)){
			// NOTE change this to pressed[i][button.first] to control all players with keyboard
			// NOTE change this to pressed[FIRST_PLAYER][button.first] to control only first player
			for(int i=0;i<N_PLAYERS; i++) pressed[i][button.first] = true;
		}
	}
}

void CharacterSelectState::pause(){

}

void CharacterSelectState::resume(){

}

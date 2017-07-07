#include "SDL_mixer.h"

#include "CharacterSelectState.h"
#include "StageSelectState.h"
#include "Game.h"
#include "MenuState.h"
#include "BattleState.h"

#define FRAME_TIME 7.5

#define N_PLAYERS 4
#define N_BACKGROUNDS 2
#define N_SKINS 4
#define FIRST_PLAYER 0
#define N_COLS 2
#define N_ROWS 4

CharacterSelectState::CharacterSelectState(string cselected_stage){
	Mix_AllocateChannels(50);

	memset(cur_col, 0, sizeof cur_col);
	memset(cur_row, 0, sizeof cur_row);
	memset(cur_skin, 0, sizeof cur_skin);
	memset(selected, false, sizeof selected);

	character_slots = Sprite("character_select/character_slots.png");
	selected_tag = Sprite("character_select/selected.png");
	ready_to_fight = Sprite("character_select/ready_to_fight.png");
	planet = Sprite("character_select/planet.png", 8, FRAME_TIME);
	planet.set_scale(1.5);

	ready = false;

	selected_stage = cselected_stage;
	blocked = Sound("menu/sound/cancel.ogg");
	selected_sound = Sound("menu/sound/select.ogg");
	changed = Sound("menu/sound/cursor.ogg");

	for(int i=0;i<N_BACKGROUNDS;i++){
		background[i] = Sprite("character_select/background_" + to_string(i + 1) + ".png");
	}

	for(int i=0;i<N_PLAYERS;i++){
		name_tag[i] = Sprite("character_select/name_tag_" + to_string(i + 1) + ".png");
		number[i] = Sprite("character_select/number_" + to_string(i + 1) + ".png");
	}

	for(int i=0;i<N_CHARS;i++){
		chars[i] = FighterMenu(get_char_info(i).first, get_char_info(i).second, i < 2);
	}

	name_tag_positions = { ii(91, 145), ii(92, 494), ii(956, 145), ii(955, 494) };
	number_delta = { ii(12, 9), ii(93, 9), ii(12, 101), ii(93, 101) };
	name_delta = { ii(173, 152), ii(172, 154), ii(102, 153), ii(105, 153) };
	sprite_pos = { ii(125, 32), ii(121, 379), ii(943, 34), ii(956, 381) };

	InputManager::get_instance()->map_keyboard_to_joystick(InputManager::MENU_MODE);
}

void CharacterSelectState::update(float delta){
	process_input();

	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(pressed[FIRST_PLAYER][SELECT] ||
		(not selected[FIRST_PLAYER] && pressed[FIRST_PLAYER][B])
	){
		selected_sound.play();
		m_quit_requested = true;
		Game::get_instance().push(new StageSelectState());
		return;
	}

	// only enable start when all players have selected a character
	if(all_players_selected()){
		ready = true;
		if(pressed[FIRST_PLAYER][START] || pressed[FIRST_PLAYER][A]){
			selected_sound.play();
			vector< pair<string, string> > p = export_players();
			m_quit_requested = true;
			Game::get_instance().push(new BattleState(selected_stage, export_players()));
			return;
		}
	}

	for(int i=0;i<N_PLAYERS;i++){
		if(not selected[i]){
			// random character
			if(pressed[i][Y]){
				selected_sound.play();
				int rand_col = 0, rand_row = 0, rand_skin = 0, char_sel = 0;

				do{
					rand_col = rand() % N_COLS;
					rand_row = rand() % N_ROWS;
					char_sel = rand_row * N_COLS + rand_col;
				}while(not chars[char_sel].is_enabled());

				do{
					rand_skin = rand() % N_SKINS;
				}while(not chars[char_sel].is_skin_available(rand_skin));

				cur_col[i] = rand_col;
				cur_row[i] = rand_row;
				cur_skin[i] = rand_skin;
			}

			int old_col = cur_col[i];
			int old_row = cur_row[i];

			// change character
			if(pressed[i][LEFT]){
				changed.play();
				if(cur_col[i] != 0){
					cur_col[i]--;
				}
			}

			if(pressed[i][RIGHT]){
				changed.play();
				if(cur_col[i] + 1 < N_COLS){
					cur_col[i]++;
				}
			}

			if(pressed[i][UP]){
				changed.play();
				if(cur_row[i] != 0){
					cur_row[i]--;
				}
			}

			if(pressed[i][DOWN]){
				changed.play();
				if(cur_row[i] + 1 < N_ROWS){
					cur_row[i]++;
				}
			}

			// reset skin if character changed
			if(cur_col[i] != old_col || cur_row[i] != old_row){
				cur_skin[i] = 0;
			}

			// change skin
			if(pressed[i][LT]){
				changed.play();
				cur_skin[i] = (cur_skin[i] - 1 + N_SKINS) % N_SKINS;
			}

			if(pressed[i][RT]){
				changed.play();
				cur_skin[i] = (cur_skin[i] + 1) % N_SKINS;
			}

			// select character && lock skin
			if(pressed[i][A]){
				int char_sel = cur_row[i] * N_COLS + cur_col[i];

				if(chars[char_sel].is_enabled()){
					if(not chars[char_sel].is_skin_available(cur_skin[i])){
						blocked.play();
					}
					else{
						selected_sound.play();
						chars[char_sel].lock_skin(cur_skin[i]);
						selected[i] = true;
					}
				}
				else{
					blocked.play();
				}
			}
		}
		else{
			// unselect character
			if(pressed[i][B]){
				int char_sel = cur_row[i] * N_COLS + cur_col[i];
				chars[char_sel].unlock_skin(cur_skin[i]);
				selected[i] = false;
				ready = false;
			}
		}
	}

	for(int i=0;i<N_CHARS;i++){
		chars[i].get_disabled().update(delta);

		for(int j=0;j<N_SKINS;j++){
			chars[i].get_skin(j).update(delta);
		}
	}

	planet.update(delta);
}

void CharacterSelectState::render(){
	background[0].render(0, 0);
	planet.render(640 - planet.get_width() / 2, 360 - planet.get_height() / 2);
	background[1].render(0, 0);
	character_slots.render(0, 0);

	for(int i=0;i<N_PLAYERS;i++){
		SDL_RendererFlip flip = i >= 2 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		int row_selected = cur_row[i];
		int col_selected = cur_col[i];
		int char_sel =  row_selected * N_COLS + col_selected;

		FighterMenu char_selected = chars[char_sel];

		char_selected.get_skin(cur_skin[i]).render(sprite_pos[i].first, sprite_pos[i].second, 0, flip);

		if(not char_selected.is_skin_available(cur_skin[i]) && not selected[i]){
			char_selected.get_disabled().render(sprite_pos[i].first, sprite_pos[i].second, 0, flip);
		}

		char_selected.get_name_text()->set_pos(
			name_tag_positions[i].first + name_delta[i].first,
			name_tag_positions[i].second + name_delta[i].second, true, true
		);

		ii slot = get_slot(row_selected, col_selected);
		name_tag[i].render(name_tag_positions[i].first, name_tag_positions[i].second);
		char_selected.get_name_text()->render();
		number[i].render(slot.first + number_delta[i].first, slot.second + number_delta[i].second);

		if(selected[i]){
			selected_tag.render(name_tag_positions[i].first, name_tag_positions[i].second, 0, flip);
		}
	}

	if(ready){
		ready_to_fight.render(0, 0);
	}
}

bool CharacterSelectState::all_players_selected(){
	for(auto cur : selected)
		if(not cur) return false;
	return true;
}

// returns name and number of frames in corresponding sprite
pair<string, int> CharacterSelectState::get_char_info(int idx){
	vector<string> names = { "blood", "flesh", "hookline", "sinker", "trap", "trip", "dusk", "dawn" };
	vector<int> frames = { 12, 8, 8, 7, 4, 4, 7, 5 };

	return make_pair(names[idx], frames[idx]);
}

vector< pair<string, string> > CharacterSelectState::export_players(){
	vector< pair<string, string> > players;

	for(int i=0;i<N_PLAYERS;i++){
		int char_sel = cur_row[i] * N_COLS + cur_col[i];
		players.push_back(make_pair(chars[char_sel].get_name(), chars[char_sel].get_skin_name(cur_skin[i])));
	}

	return players;
}

void CharacterSelectState::process_input(){
	InputManager * input_manager = InputManager::get_instance();

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
}

pair<int, int> CharacterSelectState::get_slot(int row, int col){
	vector<int> x = { 510, 645 }, y = { 55, 197, 395, 536 };
	return ii(x[col], y[row]);
}

void CharacterSelectState::pause(){}

void CharacterSelectState::resume(){}

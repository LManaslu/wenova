#include "CharacterSelectState.h"
#include "Game.h"
#include "MenuState.h"
#include "BattleState.h"

CharacterSelectState::CharacterSelectState(){
	background = Sprite("character_select/background.png");
	character_slots = Sprite("character_select/character_slots.png");

	for(int i=0;i<4;i++){
		name_tag[i] = Sprite("character_select/name_tag_" + to_string(i + 1) + ".png");
		number[i] = Sprite("character_select/number_" + to_string(i + 1) + ".png");
	}


	memset(cur_selection_col, 0, sizeof cur_selection_col);
	memset(cur_selection_row, 0, sizeof cur_selection_row);

	name_tag_positions = { ii(91, 234), ii(92, 583), ii(956, 234), ii(955, 583) };

	col_slots = { 510, 645 };
	row_slots = { 55, 197, 395, 536 };

	number_delta = { ii(12, 9), ii(93, 9), ii(12, 101), ii(93, 101) };
}

void CharacterSelectState::update(float delta){
	InputManager * input_manager = InputManager::get_instance();

	// inputs
	if(input_manager->quit_requested() || input_manager->key_press(SDLK_ESCAPE) || input_manager->joystick_button_press(InputManager::B, 0)){
		m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

	if(input_manager->key_press(SDLK_RETURN) || input_manager->joystick_button_press(InputManager::START, 0)){
		m_quit_requested = true;
		Game::get_instance().push(new BattleState("1", "swamp_song.ogg"));
		return;
	}

	for(int i=0;i<4;i++){
		if((input_manager->key_press(SDLK_LEFT) || input_manager->joystick_button_press(InputManager::LEFT, i)) && cur_selection_col[i] != 0){
			if(enabled(cur_selection_row[i], cur_selection_col[i] - 1))
			cur_selection_col[i]--;
		}

		if((input_manager->key_press(SDLK_RIGHT) || input_manager->joystick_button_press(InputManager::RIGHT, i)) && cur_selection_col[i] != 1){
			if(enabled(cur_selection_row[i], cur_selection_col[i] + 1))
			cur_selection_col[i]++;
		}

		if(input_manager->key_press(SDLK_UP) && cur_selection_row[i] != 0){
			if(enabled(cur_selection_row[i] - 1, cur_selection_col[i]))
			cur_selection_row[i]--;
		}

		if(input_manager->key_press(SDLK_DOWN) && cur_selection_row[i] != 3){
			if(enabled(cur_selection_row[i] + 1, cur_selection_col[i]))
			cur_selection_row[i]++;
		}
	}

}

void CharacterSelectState::render(){
	background.render(0, 0);
	character_slots.render(0, 0);

	for(int i=0;i<4;i++){
		int col_sel = cur_selection_col[i];
		int row_sel = cur_selection_row[i];

		number[i].render(col_slots[col_sel] + number_delta[i].first, row_slots[row_sel] + number_delta[i].second);
	}

	for(int i=0;i<4;i++){
		name_tag[i].render(name_tag_positions[i].first, name_tag_positions[i].second);
	}
}

void CharacterSelectState::pause(){

}

void CharacterSelectState::resume(){

}

bool CharacterSelectState::enabled(int row, int){
	// Only characters in first row are available
	return row == 0;
}

#include "StageSelectState.h"
#include "MenuState.h"
#include "CharacterSelectState.h"
#include "Game.h"
#include <string>

#include <cstdlib>

using std::to_string;

StageSelectState::StageSelectState() {
	planet = Sprite("stage_select/planet.png", 8, FRAME_TIME);
	planet.set_scale(1.5);

	for(int i=0;i<N_BACKGROUNDS;i++){
		background[i] = Sprite("stage_select/background_" + to_string(i) + ".png");
	}

	for(int i=0;i<N_STAGES;i++){
		stage[i] = Sprite("stage_select/stage_" + to_string(i) + ".png");
	}
}

void StageSelectState::update(float delta) {
	process_input();
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(pressed[LEFT_BUTTON]) {
		update_stage_select(-1);
	}
	if(pressed[RIGHT_BUTTON]) {
		update_stage_select(1);
	}

	if(pressed[SELECT_BUTTON]) {
		m_quit_requested = true;
		if(stage_select == 0){
			srand(clock());
			stage_select = rand() % (N_STAGES - 1) + 1;
		}
		Game::get_instance().push(new CharacterSelectState(to_string(stage_select)));
	}

	if(pressed[BACK_BUTTON]) {
		m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

	planet.update(delta);
}

void StageSelectState::render() {
	background[0].render();
	planet.render(640 - planet.get_width() / 2, 360 - planet.get_height() / 2);
	background[1].render();

	for(int i=0;i<N_STAGES;i++){
		stage[i].render(i * 780 - stage_select * 780);
	}
}

void StageSelectState::pause() {

}

void StageSelectState::resume() {

}

void StageSelectState::update_stage_select(int increment) {
	stage_select += increment;
	if(stage_select < 0) stage_select = 0;
	if(stage_select > N_STAGES - 1) stage_select = N_STAGES - 1;
}

void StageSelectState::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	vector< pair<int, int> > buttons = {
		ii(LEFT_BUTTON, InputManager::K_LEFT),
		ii(RIGHT_BUTTON, InputManager::K_RIGHT),
		ii(SELECT_BUTTON, InputManager::K_A),
		ii(BACK_BUTTON, InputManager::K_B)
	};

	vector< pair<int, int> > joystick_buttons = {
		ii(LEFT_BUTTON, InputManager::LEFT),
		ii(RIGHT_BUTTON, InputManager::RIGHT),
		ii(SELECT_BUTTON, InputManager::A),
		ii(BACK_BUTTON, InputManager::B)
	};

	if(SDL_NumJoysticks() != 0){
		for(ii button : joystick_buttons){
			pressed[button.first] = input_manager->joystick_button_press(button.second, 0);
			is_holding[button.first] = input_manager->is_joystick_button_down(button.second, 0);
			released[button.first] = input_manager->joystick_button_release(button.second, 0);
		}
	}else{
		for(ii button : buttons){
			pressed[button.first] = input_manager->key_press(button.second, true);
			is_holding[button.first] = input_manager->is_key_down(button.second, true);
			released[button.first] = input_manager->key_release(button.second, true);
		}
	}
}

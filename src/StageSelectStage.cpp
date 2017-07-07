#include "StageSelectState.h"

#include "MenuState.h"
#include "EditState.h"
#include "CharacterSelectState.h"
#include "Game.h"
#include <string>

#include <cstdlib>

using std::to_string;

StageSelectState::StageSelectState(bool cgo_to_edit) {
	planet = Sprite("stage_select/planet.png", 8, FRAME_TIME);
	planet.set_scale(1.5);
	go_to_edit = cgo_to_edit;
	n_stages = 2 + (go_to_edit ? 0 : 1);

	blocked = Sound("menu/sound/cancel.ogg");
	selected = Sound("menu/sound/select.ogg");
	changed = Sound("menu/sound/cursor.ogg");

	for(int i=0;i<N_BACKGROUNDS;i++){
		background[i] = Sprite("stage_select/background_" + to_string(i) + ".png");
	}

	for(int i=0;i<n_stages;i++){
		stage[i] = Sprite("stage_select/stage_" + to_string(i + 1) + ".png");
	}

	InputManager::get_instance()->map_keyboard_to_joystick(InputManager::MENU_MODE);
}

void StageSelectState::update(float delta) {
	process_input();

	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(pressed[B] || pressed[SELECT]) {
		selected.play();
		m_quit_requested = true;
		Game::get_instance().push(new MenuState(true));
		return;
	}

	if(pressed[LEFT]) {
		update_stage_select(-1);
	}

	if(pressed[RIGHT]) {
		update_stage_select(1);
	}

	if(pressed[A] || pressed[START]) {
		selected.play();
		m_quit_requested = true;
		if(stage_select == 2){
			srand(clock());
			stage_select = rand() % (n_stages - (go_to_edit ? 0 : 1));
		}
		if(go_to_edit)
			Game::get_instance().push(new EditState(to_string(stage_select + 1)));
		else
			Game::get_instance().push(new CharacterSelectState(to_string(stage_select + 1)));

	}

	planet.update(delta);
}

void StageSelectState::render() {
	background[0].render();
	planet.render(640 - planet.get_width() / 2, 360 - planet.get_height() / 2);
	background[1].render();

	for(int i=0;i<n_stages;i++){
		stage[i].render(i * 780 - stage_select * 780);
	}
}

void StageSelectState::update_stage_select(int increment) {
	stage_select += increment;
	if(stage_select < 0){
		blocked.play();
		stage_select = 0;
	}
	else if(stage_select > n_stages - 1){
		blocked.play();
		stage_select = n_stages - 1;
	}
	else{
		selected.play();
	}
}

void StageSelectState::process_input(){
	InputManager * input_manager = InputManager::get_instance();

	//MENU BUTTONS HERE
	vector< pair<int, int> > joystick_buttons = {
		ii(LEFT, InputManager::LEFT),
		ii(RIGHT, InputManager::RIGHT),
		ii(A, InputManager::A),
		ii(B, InputManager::B),
		ii(START, InputManager::START),
		ii(SELECT, InputManager::SELECT)
	};

	for(ii button : joystick_buttons){
		pressed[button.first] = input_manager->joystick_button_press(button.second, 0);
	}
}

void StageSelectState::pause() {

}

void StageSelectState::resume() {

}

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
	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->quit_requested()){
		m_quit_requested = true;
		return;
	}

	if(input_manager->key_press(InputManager::K_LEFT)) {
		update_stage_select(-1);
	}
	if(input_manager->key_press(InputManager::K_RIGHT)) {
		update_stage_select(1);
	}

	if(input_manager->key_press(InputManager::K_A)) {
		m_quit_requested = true;
		if(stage_select == 0){
			srand(clock());
			stage_select = rand() % (N_STAGES - 1) + 1;
		}
		Game::get_instance().push(new CharacterSelectState(to_string(stage_select)));
	}

	if(input_manager->key_press(InputManager::K_B)) {
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

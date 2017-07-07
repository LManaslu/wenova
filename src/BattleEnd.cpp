#include "BattleEnd.h"
#include "InputManager.h"

using std::to_string;

#define WHITE { 255, 255, 255, 255 }

BattleEnd::BattleEnd(int id_winner){
	back_btn = Sprite("victory/buttons/back.png");

	back_txt = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "BACK", WHITE);
	back_txt->set_pos(1154, 650);

	back_selected = false;

	sprite = vector<Sprite>(N_SPRITES + 2);

	string path = "victory/" + to_string(id_winner) + "/"; 

	for(int i = 1; i <= N_SPRITES; i++){
		sprite[i] = Sprite(path + to_string(i) + ".png", 1, 100);
	}

	current_sprite = 1;

	quitRequested = false;
}

void BattleEnd::update(float delta){
	sprite[current_sprite].update(delta);

	if(sprite[current_sprite].is_finished()){
		if(back_selected && current_sprite > 1)
			current_sprite--;	
		else if(not back_selected && current_sprite < N_SPRITES)
			current_sprite++;
	}

	InputManager * input_manager = InputManager::get_instance();

	if(input_manager->joystick_button_press(InputManager::SELECT, 0)){
		for(int i = 1; i <= N_SPRITES; i++)
			sprite[i].restart_count();

		back_selected = true;
	}

	if(back_selected && sprite[current_sprite].is_finished() && current_sprite == 1){
		quitRequested = true;
		return;
	}
}

void BattleEnd::render(){
	sprite[current_sprite].render();
	back_btn.render(1094, 642);
	back_txt->render();
}

bool BattleEnd::is_dead(){
	return false;
}

bool BattleEnd::quit_requested(){
	return quitRequested;
}

void BattleEnd::notify_collision(GameObject &){

}

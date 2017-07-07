#include "BattleEnd.h"
#include "InputManager.h"

using std::to_string;

#define WHITE { 255, 255, 255, 255 }

BattleEnd::BattleEnd(int id_winner){
	back_btn = Sprite("victory/buttons/back.png");

	back_txt = new Text("font/8-BIT WONDER.ttf", 22, Text::TextStyle::SOLID, "BACK", WHITE);
	back_txt->set_pos(1154, 650);

	sprite = vector<Sprite>(N_SPRITES + 2);

	string path = "victory/" + to_string(id_winner) + "/"; 

	for(int i = 1; i <= N_SPRITES; i++){
		sprite[i] = Sprite(path + to_string(i) + ".png", 1, 15);
	}

	current_sprite = 1;
}

void BattleEnd::update(float delta){
	sprite[current_sprite].update(delta);

	if(sprite[current_sprite].is_finished() && current_sprite < N_SPRITES)
		current_sprite++;
}

void BattleEnd::render(){
	sprite[current_sprite].render();
	back_btn.render(1094, 642);
	back_txt->render();
}

bool BattleEnd::is_dead(){
	return false;
}

void BattleEnd::notify_collision(GameObject &){

}
